// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/RL_GA_ShootAtLocation.h"
#include "../Online_ModeCharacter.h"
#include "AS/RL_AS_Player.h"
#include "Actor/RL_Actor_Bullet.h"
#include "Weapon/RL_Actor_Weapon.h"
#include "Container/RL_Container_GEContainer.h"
#include <Abilities/Tasks/AbilityTask_WaitGameplayEvent.h>
#include "PS/RL_PS_Base.h"

bool URL_GA_ShootAtLocation::CanActivateAbility(
const FGameplayAbilitySpecHandle Handle,
const FGameplayAbilityActorInfo* ActorInfo, 
const FGameplayTagContainer* SourceTags, 
const FGameplayTagContainer* TargetTags, 
OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;
	// ��鵯ҩ�Ƿ��㹻
	const float CurrentAmmo = GetAttribute(ActorInfo,URL_AS_Player::GetLeftAmmosInMagazineAttribute());
		
	return CurrentAmmo > 0;
}

void URL_GA_ShootAtLocation::OnProjectileHit(FGameplayEventData EventData)
{
	FGameplayAbilityTargetDataHandle TargetData = EventData.TargetData;

	// 3. ��Ŀ��������ӵ�SpecContainer��
	AllEffectHandlesContainer.TargetData = TargetData;

	// 4. Ӧ������Ч����
	ApplyEffectForEachSpecHandles(AllEffectHandlesContainer);

	// 5. �˺��߼�������ϣ����ڿ��Խ���������
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void URL_GA_ShootAtLocation::ActivateAbility(
const FGameplayAbilitySpecHandle Handle, 
const FGameplayAbilityActorInfo* ActorInfo, 
const FGameplayAbilityActivationInfo ActivationInfo, 
const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthority(&ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

		return;
	}
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
		HitLocation = HitResult.Location;
	}

	// =========================================================================
	//               **�����ӵ�������λ�ú����ճ���**
	// =========================================================================

	// 4. ��ȡ����ǹ�ڵ�λ����Ϊ�ӵ������ɵ�
	ARL_Actor_Weapon* CurrentWeapon = LocalCharacter->GetWeapon();

	USkeletalMeshComponent* WeaponMesh = CurrentWeapon->WeaponMesh;

	FVector MuzzleLocation{0,0,0};

	if (LocalCharacter->ProjectileSpawnPoint)
	{
		// ������ڣ�������������λ����Ϊ���ɵ㡣���λ�ò��ܶ���Ӱ�죡
		MuzzleLocation = LocalCharacter->ProjectileSpawnPoint->GetComponentLocation();
	}
	// 5. �����ǹ��ָ��Ŀ���(HitLocation)��������ת����
	FRotator FinalRotation = (HitLocation - MuzzleLocation).Rotation();

	FGameplayEffectContainer Container = CurrentWeapon->WeaponEffect;

	FGameplayEffectHandleSpecContainer HandleSpecContainer = ConstructHandleSpecsFromContainer(Container);

	for (FGameplayEffectSpecHandle& SpecHandle : HandleSpecContainer.EffectSpecHandles)
	{
		if (SpecHandle.IsValid() && SpecHandle.Data.IsValid())
		{
			// **�����޸ģ�ʹ����ȷ�� UGameplayEffect ����**
			const UGameplayEffect* EffectDef = SpecHandle.Data->Def;
			if (!EffectDef)
			{
				continue;
			}
			// **ͨ�����GE�����Tag���ж�������**
			// EffectDef->InheritedTags �����GE������ͼ��C++����̳е�����Tag
			if (EffectDef->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage"))))
			{
				// ����һ���˺����͵�GE
				 float FinalDamage; 
				 CalculateFinalDamage(FinalDamage, GetAttribute(ActorInfo, URL_AS_Player::GetStrengthAttribute()));
				 SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), FinalDamage);
			}
			else if (EffectDef->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Heal"))))
			{
				//TODO:
				// ����һ���������͵�GE
				// const float FinalHealing = CalculateFinalHealing(...);
				// SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Healing")), FinalHealing);
			}
			else if (EffectDef->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Slow"))))
			{
				//TODO:
				// ����һ���������͵�GE
			}
			else if (EffectDef->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Fast"))))
			{
				//TODO:
				// ����һ���������͵�GE
			}
		}
	}
	//��HandleSpecContainer���浽AllEffectHandlesContainer��.GA
	AllEffectHandlesContainer = HandleSpecContainer;

	//TODO:��鵱ǰ���������ӵ�����,ɢ�䵯,������,������,�ȵ�,���������ϻ�ȡ�ӵ�����AmmoActorClass,Ȼ������ӵ����ʹ����ӵ�
	if (GetWorld() && CurrentWeapon->AmmoActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = LocalCharacter;
		FVector SpawnLocation = MuzzleLocation + FinalRotation.Vector() * AmmoPadding ;

		FTransform SpawnTransform(FinalRotation, SpawnLocation);
		// ʹ�� `SpawnActorDeferred` ��ȷ�����ǿ��������ɺ����ò���
			ARL_Actor_Bullet* Ammo = GetWorld()->SpawnActorDeferred<ARL_Actor_Bullet>(
			CurrentWeapon->AmmoActorClass,
			SpawnTransform,
			LocalCharacter,
			LocalCharacter,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		UAbilityTask_WaitGameplayEvent* WaitHitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Event.Projectile.Hit")));
		// �����ǵĻص������󶨵�Task��ί����
		WaitHitEventTask->EventReceived.AddDynamic(this, &URL_GA_ShootAtLocation::OnProjectileHit);
		
		WaitHitEventTask->ReadyForActivation();
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
}

