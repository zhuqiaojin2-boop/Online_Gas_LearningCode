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
	// 检查弹药是否足够
	const float CurrentAmmo = GetAttribute(ActorInfo,URL_AS_Player::GetLeftAmmosInMagazineAttribute());
		
	return CurrentAmmo > 0;
}

void URL_GA_ShootAtLocation::OnProjectileHit(FGameplayEventData EventData)
{
	FGameplayAbilityTargetDataHandle TargetData = EventData.TargetData;

	// 3. 将目标数据添加到SpecContainer中
	AllEffectHandlesContainer.TargetData = TargetData;

	// 4. 应用所有效果！
	ApplyEffectForEachSpecHandles(AllEffectHandlesContainer);

	// 5. 伤害逻辑处理完毕，现在可以结束技能了
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

	//TODO:封装函数,太乱了
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

	// 1. 获取玩家的视点（摄像机位置和朝向）
	AController* Controller = LocalCharacter->GetController();
	if (Controller)
	{
		Controller->GetPlayerViewPoint(TraceStart, ViewRotation);
	}
	else
	{
		LocalCharacter->GetActorEyesViewPoint(TraceStart, ViewRotation);
	}
	
	// 2. 计算射线的终点
	float TraceDistance = 10000.0f; // 设置一个足够远的追踪距离
	TraceEnd = TraceStart + ViewRotation.Vector() * TraceDistance;
	HitLocation = TraceEnd; // 默认情况下，如果没有命中任何东西，目标就是射线终点

	// 3. 执行视线追踪 (Line Trace)
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(LocalCharacter); // 忽略角色自己，防止射线打到自己
	CollisionParams.AddIgnoredActor(LocalCharacter->GetWeapon()); // 忽略武器

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
	{
		HitLocation = HitResult.Location;
	}

	// =========================================================================
	//               **计算子弹的生成位置和最终朝向**
	// =========================================================================

	// 4. 获取武器枪口的位置作为子弹的生成点
	ARL_Actor_Weapon* CurrentWeapon = LocalCharacter->GetWeapon();

	USkeletalMeshComponent* WeaponMesh = CurrentWeapon->WeaponMesh;

	FVector MuzzleLocation{0,0,0};

	if (LocalCharacter->ProjectileSpawnPoint)
	{
		// 如果存在，就用它的世界位置作为生成点。这个位置不受动画影响！
		MuzzleLocation = LocalCharacter->ProjectileSpawnPoint->GetComponentLocation();
	}
	// 5. 计算从枪口指向目标点(HitLocation)的最终旋转方向
	FRotator FinalRotation = (HitLocation - MuzzleLocation).Rotation();

	FGameplayEffectContainer Container = CurrentWeapon->WeaponEffect;

	FGameplayEffectHandleSpecContainer HandleSpecContainer = ConstructHandleSpecsFromContainer(Container);

	for (FGameplayEffectSpecHandle& SpecHandle : HandleSpecContainer.EffectSpecHandles)
	{
		if (SpecHandle.IsValid() && SpecHandle.Data.IsValid())
		{
			// **核心修改：使用正确的 UGameplayEffect 类型**
			const UGameplayEffect* EffectDef = SpecHandle.Data->Def;
			if (!EffectDef)
			{
				continue;
			}
			// **通过检查GE自身的Tag来判断其类型**
			// EffectDef->InheritedTags 是这个GE从其蓝图或C++父类继承的所有Tag
			if (EffectDef->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage"))))
			{
				// 这是一个伤害类型的GE
				 float FinalDamage; 
				 CalculateFinalDamage(FinalDamage, GetAttribute(ActorInfo, URL_AS_Player::GetStrengthAttribute()));
				 SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), FinalDamage);
			}
			else if (EffectDef->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Heal"))))
			{
				//TODO:
				// 这是一个治疗类型的GE
				// const float FinalHealing = CalculateFinalHealing(...);
				// SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Healing")), FinalHealing);
			}
			else if (EffectDef->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Slow"))))
			{
				//TODO:
				// 这是一个减速类型的GE
			}
			else if (EffectDef->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Fast"))))
			{
				//TODO:
				// 这是一个减速类型的GE
			}
		}
	}
	//将HandleSpecContainer储存到AllEffectHandlesContainer中.GA
	AllEffectHandlesContainer = HandleSpecContainer;

	//TODO:检查当前武器发射子弹类型,散射弹,单发弹,连发弹,等等,从武器身上获取子弹类型AmmoActorClass,然后根据子弹类型创建子弹
	if (GetWorld() && CurrentWeapon->AmmoActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = LocalCharacter;
		FVector SpawnLocation = MuzzleLocation + FinalRotation.Vector() * AmmoPadding ;

		FTransform SpawnTransform(FinalRotation, SpawnLocation);
		// 使用 `SpawnActorDeferred` 来确保我们可以在生成后设置参数
			ARL_Actor_Bullet* Ammo = GetWorld()->SpawnActorDeferred<ARL_Actor_Bullet>(
			CurrentWeapon->AmmoActorClass,
			SpawnTransform,
			LocalCharacter,
			LocalCharacter,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		UAbilityTask_WaitGameplayEvent* WaitHitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Event.Projectile.Hit")));
		// 将我们的回调函数绑定到Task的委托上
		WaitHitEventTask->EventReceived.AddDynamic(this, &URL_GA_ShootAtLocation::OnProjectileHit);
		
		WaitHitEventTask->ReadyForActivation();
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
}

