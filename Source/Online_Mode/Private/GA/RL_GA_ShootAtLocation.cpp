// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/RL_GA_ShootAtLocation.h"
#include "../Online_ModeCharacter.h"
#include "AS/RL_AS_Player.h"
#include "Actor/RL_Actor_Bullet.h"
#include "Weapon/RL_Actor_Weapon.h"


bool URL_GA_ShootAtLocation::CanActivateAbility(
const FGameplayAbilitySpecHandle Handle,
const FGameplayAbilityActorInfo* ActorInfo, 
const FGameplayTagContainer* SourceTags, 
const FGameplayTagContainer* TargetTags, 
OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;
	// ��鵯ҩ�Ƿ��㹻
	const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		const float CurrentAmmo = ASC->GetNumericAttribute(URL_AS_Player::GetLeftAmmosInMagazineAttribute());
		//debug
		const AOnline_ModeCharacter* Character = Cast<AOnline_ModeCharacter>(ActorInfo->AvatarActor.Get());
		if (!Character)
		{
			return false;
		}
		const URL_AS_Player* PlayerAttributeSet = Character->GetAttributeSetFromPS();
		if (PlayerAttributeSet)
		{
			// 5. ���ڣ����ǿ���ֱ�Ӵ����Լ�ʵ���л�ȡ����ֵ��
			const float TestCurrentAmmo = PlayerAttributeSet->GetLeftAmmosInMagazine();

			// 6. �û�ȡ����ֵ�����߼��ж�
			return TestCurrentAmmo > 0;
		}
		return CurrentAmmo > 0;
 /*   if (!Character)
    {
        return false;
    }
	}
	return false;*/}
	return false;
}

void URL_GA_ShootAtLocation::ActivateAbility(
const FGameplayAbilitySpecHandle Handle, 
const FGameplayAbilityActorInfo* ActorInfo, 
const FGameplayAbilityActivationInfo ActivationInfo, 
const FGameplayEventData* TriggerEventData)
{
	/*if (!HasAuthority(&ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

		return;
	}*/
	bool bIsPredicting = (ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);

	//TODO:��װ����,̫����
	if (CostGE)
	{
		CostGameplayEffectClass = CostGE;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo,true, bIsPredicting);
		return;
	}
	if (ActorInfo->IsNetAuthority())
	{
	AOnline_ModeCharacter* LocalCharacter = Cast<AOnline_ModeCharacter>(ActorInfo->AvatarActor);
	//USkeletalMeshComponent* WeaponMesh = LocalCharacter->GetWeapon()->WeaponMesh;

	if (!LocalCharacter || !LocalCharacter->GetWeapon())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	FVector TraceStart;
	FRotator ViewRotation;
	FVector TraceEnd;
	FVector HitLocation;

	// 1. ��ȡ��ҵ��ӵ㣨�����λ�úͳ���
	AController* Controller = LocalCharacter->GetController();
	if (Controller)
	{
		Controller->GetPlayerViewPoint(TraceStart, ViewRotation);
	}
	else
	{
		LocalCharacter->GetActorEyesViewPoint(TraceStart, ViewRotation);
	}
	/*LocalCharacter->GetActorEyesViewPoint(TraceStart, ViewRotation);
	if (Controller)
	{
		ViewRotation = Controller->GetControlRotation();
	}*/
	// 2. �������ߵ��յ�
	float TraceDistance = 10000.0f; // ����һ���㹻Զ��׷�پ���
	TraceEnd = TraceStart + ViewRotation.Vector() * TraceDistance;
	HitLocation = TraceEnd; // Ĭ������£����û�������κζ�����Ŀ����������յ�

	// 3. ִ������׷�� (Line Trace)
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(LocalCharacter); // ���Խ�ɫ�Լ�����ֹ���ߴ��Լ�
	CollisionParams.AddIgnoredActor(LocalCharacter->GetWeapon()); // ��������

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
	{
		// �������������ĳ�����壬����Ŀ���Ϊ���е�
		HitLocation = HitResult.Location;
	}

	// =========================================================================
	//               **�����ӵ�������λ�ú����ճ���**
	// =========================================================================

	// 4. ��ȡ����ǹ�ڵ�λ����Ϊ�ӵ������ɵ�
	USkeletalMeshComponent* WeaponMesh = LocalCharacter->GetWeapon()->WeaponMesh;
	//FVector MuzzleLocation = WeaponMesh->GetSocketLocation(FName("MuzzleSocket")); // �������ǹ�ڲ�۽� "MuzzleSocket"
	FVector MuzzleLocation{0,0,0};

	if (LocalCharacter->ProjectileSpawnPoint)
	{
		// ������ڣ�������������λ����Ϊ���ɵ㡣���λ�ò��ܶ���Ӱ�죡
		MuzzleLocation = LocalCharacter->ProjectileSpawnPoint->GetComponentLocation();
	}
	// �����־������
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Health: %s"), GetHealth()));
	}*/
	// 5. �����ǹ��ָ��Ŀ���(HitLocation)��������ת����
	FRotator FinalRotation = (HitLocation - MuzzleLocation).Rotation();

	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
		
	if (DamageSpecHandle.IsValid())
	{
		const URL_AS_Base* LocalAttributes = Cast<URL_AS_Base>(ActorInfo->AbilitySystemComponent->GetAttributeSet(URL_AS_Base::StaticClass()));
		const float StrengthBonus = LocalAttributes ? LocalAttributes->GetStrength() * 0.5f : 0.f; // �����������˺��мӳ�
		const float FinalDamage = BaseDamage + StrengthBonus;

		DamageSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), FinalDamage);
	}

	if (GetWorld() && AmmoActorClass)	
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = LocalCharacter;
		FVector SpawnLocation = MuzzleLocation + FinalRotation.Vector() * AmmoPadding ;


		FTransform SpawnTransform(FinalRotation, SpawnLocation);
		// ʹ�� `SpawnActorDeferred` ��ȷ�����ǿ��������ɺ����ò���
			ARL_Actor_Bullet* Ammo = GetWorld()->SpawnActorDeferred<ARL_Actor_Bullet>(
			AmmoActorClass,
			SpawnTransform,
			LocalCharacter,
			LocalCharacter,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		if (Ammo)
		{
		//	 **�ؼ�����**: �����úõ� Damage Spec Handle ���ݸ��ӵ�
			Ammo->SetDamageEffectSpecHandle(DamageSpecHandle);
		//  �������
			Ammo->FinishSpawning(SpawnTransform);
		}
	}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

	