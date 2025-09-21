// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RL_Character_Base.h"
#include "PS/RL_PS_Base.h"
#include "ASC/RL_ASC_Base.h"
#include "GE/RL_GE_Base.h"
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetSystemLibrary.h>
#include "Weapon/RL_Actor_Weapon.h"
#include "AS/RL_AS_Base.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
ARL_Character_Base::ARL_Character_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    WeaponSocketName = FName(TEXT("WeaponSlot"));
    bUseControllerRotationYaw = true;
}
void ARL_Character_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARL_Character_Base,Weapon);
}

ARL_Actor_Weapon* ARL_Character_Base::GetWeapon() const
{
    return Weapon;
}

void ARL_Character_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
    InitializeAbilitySystem();
}

void ARL_Character_Base::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
    InitializeAbilitySystem();
}

void ARL_Character_Base::EquipWeapon()
{
    if (!HasAuthority())
    {
        return;
    }

    // 如果已经有武器了，可以先销毁旧的 (可选)
    if (Weapon)
    {
        Weapon->Destroy();
    }

    if (WeaponClass)
    {
        // 1. 生成武器 Actor
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;       // 设置所有者
        SpawnParams.Instigator = this;  // 设置发起者

        Weapon = GetWorld()->SpawnActor<ARL_Actor_Weapon>(WeaponClass, SpawnParams);

        UE_LOG(LogTemp, Warning, TEXT("Spawned Weapon: %s"), *Weapon->GetName());

        UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Spawned Weapon"));
        if (Weapon)
        {
            // 2. **核心绑定逻辑**
            // 定义附加规则：位置和旋转都对齐到插槽，但保持武器自身的缩放
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
            // 上面的写法等同于 FAttachmentTransformRules::SnapToTargetNotIncludingScale，但更明确
            // SnapToTargetNotIncludingScale 的内部实现是 KeepWorld 的 Scale Rule

            // 将武器 Actor 的根组件附加到我们角色的骨骼网格组件上
            Weapon->AttachToComponent(
                GetMesh(),                  // 父组件：角色的 SkeletalMeshComponent
                AttachmentRules,            // 附加规则
                WeaponSocketName            // 插槽名字 (在 .h 中定义，可在蓝图中修改)
            );
        }
    }
}

void ARL_Character_Base::HandleDeath_Implementation()
{
    if (HasAuthority())
    {
        // 1. 在这里处理服务器的“游戏性”死亡逻辑
        //    - 停止AI行为树
        //    - 禁用角色的碰撞（比如只保留一个胶囊体用于拖拽）
        //    - 取消所有正在激活的技能 (通过ASC)

        // 2. **最重要的一步**: 调用多播RPC，通知所有客户端
        Multicast_OnDeath();

        // 3. (可选) 设置一个定时器，在一段时间后销毁Actor或处理重生
        //FTimerHandle TimerHandle;
        //GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARL_Character_Base::DestroySelf, 5.0f, false);
    }
}

void ARL_Character_Base::Multicast_OnDeath_Implementation()
{
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
    GetCharacterMovement()->DisableMovement();

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC && PC->IsLocalController())
    {
        DisableInput(PC);
    }
    if(Weapon) Weapon->Destroy();
    Destroy();
}

UAbilitySystemComponent* ARL_Character_Base::GetAbilitySystemComponent() const
{
    return nullptr;
}

URL_AS_Base* ARL_Character_Base::GetAttributeSet() const
{
    return nullptr;
}
//TODO:Debug Test 后续需要完善武器拾取切换逻辑
/*void ARL_Character_Base::OnRep_Weapon()
{
    if (Weapon)
    {
        // 客户端也需要执行附加逻辑
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true); // SnapToTarget 简写
        Weapon->AttachToComponent(GetMesh(), AttachmentRules, WeaponSocketName);
    }
    // 如果武器被设置为空 (比如卸载了)，就分离旧的武器
 /*   else if (OldWeapon)
    {
        OldWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }*/

//}

void ARL_Character_Base::BeginPlay()
{
	Super::BeginPlay();
    EquipWeapon();//TODO:Debug Test 后续需要完善武器拾取逻辑
    //GetAttributeSet()->OnPlayerDeath.AddDynamic(this, &ARL_Character_Base::HandlePlayerDeath);
}

void ARL_Character_Base::ApplyDefaultAttributesEffect(UAbilitySystemComponent* ASC)
{
    if (DefaultAttributesEffect)
    {
        FGameplayEffectContextHandle ContextHandle;

        if (ASC) { ContextHandle = ASC->MakeEffectContext(); }

        ContextHandle.AddSourceObject(this);
        FGameplayEffectSpecHandle SpecHandle;
        if (ASC) SpecHandle = ASC->MakeOutgoingSpec(DefaultAttributesEffect, 1, ContextHandle);

        if (SpecHandle.IsValid())
        {
            if (SpecHandle.Data.Get())
                ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}

void ARL_Character_Base::InitializeAbilitySystem()
{
    if (!HasAuthority()) return;

    UAbilitySystemComponent* ASC = nullptr;
    if (!ASC)
    {
        return;
    }
    AActor* OwnerActor = nullptr;
    // 1. 尝试获取任何类型的PlayerState
    APlayerState* PS = GetPlayerState();
    if (PS)
    {
        OwnerActor = PS;
        // ASC也必须从PS获取，这才是正确的、唯一的来源。
        ASC= Cast<IAbilitySystemInterface>(PS)->GetAbilitySystemComponent();
    }
    else
    {
        OwnerActor = this;
        // ASC也从自己身上获取。
        ASC = GetAbilitySystemComponent();
    }
    ASC->InitAbilityActorInfo(OwnerActor, this);
    // 5. 用最终确定的Owner和Avatar进行初始化 (客户端和服务器都必须执行!)
    if (DefaultAttributesEffect)
    {
        ApplyDefaultAttributesEffect(ASC);
    }
}

