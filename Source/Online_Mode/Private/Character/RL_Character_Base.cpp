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

    // ����Ѿ��������ˣ����������پɵ� (��ѡ)
    if (Weapon)
    {
        Weapon->Destroy();
    }

    if (WeaponClass)
    {
        // 1. �������� Actor
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;       // ����������
        SpawnParams.Instigator = this;  // ���÷�����

        Weapon = GetWorld()->SpawnActor<ARL_Actor_Weapon>(WeaponClass, SpawnParams);

        UE_LOG(LogTemp, Warning, TEXT("Spawned Weapon: %s"), *Weapon->GetName());

        UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Spawned Weapon"));
        if (Weapon)
        {
            // 2. **���İ��߼�**
            // ���帽�ӹ���λ�ú���ת�����뵽��ۣ��������������������
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
            // �����д����ͬ�� FAttachmentTransformRules::SnapToTargetNotIncludingScale��������ȷ
            // SnapToTargetNotIncludingScale ���ڲ�ʵ���� KeepWorld �� Scale Rule

            // ������ Actor �ĸ�������ӵ����ǽ�ɫ�Ĺ������������
            Weapon->AttachToComponent(
                GetMesh(),                  // ���������ɫ�� SkeletalMeshComponent
                AttachmentRules,            // ���ӹ���
                WeaponSocketName            // ������� (�� .h �ж��壬������ͼ���޸�)
            );
        }
    }
}

void ARL_Character_Base::HandleDeath_Implementation()
{
    if (HasAuthority())
    {
        // 1. �����ﴦ��������ġ���Ϸ�ԡ������߼�
        //    - ֹͣAI��Ϊ��
        //    - ���ý�ɫ����ײ������ֻ����һ��������������ק��
        //    - ȡ���������ڼ���ļ��� (ͨ��ASC)

        // 2. **����Ҫ��һ��**: ���öಥRPC��֪ͨ���пͻ���
        Multicast_OnDeath();

        // 3. (��ѡ) ����һ����ʱ������һ��ʱ�������Actor��������
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
//TODO:Debug Test ������Ҫ��������ʰȡ�л��߼�
/*void ARL_Character_Base::OnRep_Weapon()
{
    if (Weapon)
    {
        // �ͻ���Ҳ��Ҫִ�и����߼�
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true); // SnapToTarget ��д
        Weapon->AttachToComponent(GetMesh(), AttachmentRules, WeaponSocketName);
    }
    // �������������Ϊ�� (����ж����)���ͷ���ɵ�����
 /*   else if (OldWeapon)
    {
        OldWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }*/

//}

void ARL_Character_Base::BeginPlay()
{
	Super::BeginPlay();
    EquipWeapon();//TODO:Debug Test ������Ҫ��������ʰȡ�߼�
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
    // 1. ���Ի�ȡ�κ����͵�PlayerState
    APlayerState* PS = GetPlayerState();
    if (PS)
    {
        OwnerActor = PS;
        // ASCҲ�����PS��ȡ���������ȷ�ġ�Ψһ����Դ��
        ASC= Cast<IAbilitySystemInterface>(PS)->GetAbilitySystemComponent();
    }
    else
    {
        OwnerActor = this;
        // ASCҲ���Լ����ϻ�ȡ��
        ASC = GetAbilitySystemComponent();
    }
    ASC->InitAbilityActorInfo(OwnerActor, this);
    // 5. ������ȷ����Owner��Avatar���г�ʼ�� (�ͻ��˺ͷ�����������ִ��!)
    if (DefaultAttributesEffect)
    {
        ApplyDefaultAttributesEffect(ASC);
    }
}

