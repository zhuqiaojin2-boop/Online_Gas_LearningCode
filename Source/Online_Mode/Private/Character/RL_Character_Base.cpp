// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RL_Character_Base.h"
#include "PS/RL_PS_Base.h"
#include "ASC/RL_ASC_Base.h"
#include "GE/RL_GE_Base.h"
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetSystemLibrary.h>
#include "Weapon/RL_Actor_Weapon.h"
#include "AS/RL_AS_Player.h"
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
    //Destroy();
}
UAbilitySystemComponent* ARL_Character_Base::GetAbilitySystemComponent() const
{
    ARL_PS_Base* PS = GetPlayerState<ARL_PS_Base>();
    return PS ? PS->GetAbilitySystemComponent() : nullptr;
}

//void ARL_Character_Base::SetAbilitySystemComponent()
//{
//    /*if (const ARL_PS_Base* PS = Cast<ARL_PS_Base>(GetPlayerState()))
//    {
//        LocalASC = Cast<URL_ASC_Base>(PS->GetAbilitySystemComponent());
//    }
//    LocalASC = CreateDefaultSubobject<URL_ASC_Base>(TEXT("AbilitySystemComponent"));*/
//}

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

// Called when the game starts or when spawned
URL_AS_Player* ARL_Character_Base::GetAttributeSetFromPS()const
{
    ARL_PS_Base* PS = GetPlayerState<ARL_PS_Base>();
    if (PS)
    {
        return PS->GetAttributeSet();
    }
    return nullptr;
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

void ARL_Character_Base::BeginPlay()
{
	Super::BeginPlay();
    EquipWeapon();//TODO:Debug Test ������Ҫ��������ʰȡ�߼�
    //GetAttributeSet()->OnPlayerDeath.AddDynamic(this, &ARL_Character_Base::HandlePlayerDeath);
   
}

void ARL_Character_Base::InitializeAbilitySystem()
{
     if(!HasAuthority()) return;

   //TODO:����ǽ�ɫ�����ASC��ʼ���߼�, Ӧ�������˹���ĳ�ʼ���߼�,���ǹ���û��PS, ��Ҫ�޸� 
      ARL_PS_Base* PS = GetPlayerState<ARL_PS_Base>();
     if (!PS)
     {
       return;
     }

   URL_ASC_Base* ASC = Cast<URL_ASC_Base>(PS->GetAbilitySystemComponent());

    if (ASC)
    {
        ASC->InitAbilityActorInfo(PS, this);
    }

	// Ӧ�� GE �����ó�ʼ����ֵ (���� Health=100)
	if (DefaultAttributesEffect)
	{
        FGameplayEffectContextHandle ContextHandle;

		if(ASC){ ContextHandle = ASC->MakeEffectContext();}

		ContextHandle.AddSourceObject(this);
        FGameplayEffectSpecHandle SpecHandle;
        if (ASC) SpecHandle = ASC->MakeOutgoingSpec(DefaultAttributesEffect, 1, ContextHandle);

		if (SpecHandle.IsValid())
		{
            if(SpecHandle.Data.Get())
                ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

