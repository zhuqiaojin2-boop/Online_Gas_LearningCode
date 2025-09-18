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
	// 检查弹药是否足够
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
			// 5. 现在，我们可以直接从属性集实例中获取属性值了
			const float TestCurrentAmmo = PlayerAttributeSet->GetLeftAmmosInMagazine();

			// 6. 用获取到的值进行逻辑判断
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
	/*LocalCharacter->GetActorEyesViewPoint(TraceStart, ViewRotation);
	if (Controller)
	{
		ViewRotation = Controller->GetControlRotation();
	}*/
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
		// 如果射线命中了某个物体，更新目标点为命中点
		HitLocation = HitResult.Location;
	}

	// =========================================================================
	//               **计算子弹的生成位置和最终朝向**
	// =========================================================================

	// 4. 获取武器枪口的位置作为子弹的生成点
	USkeletalMeshComponent* WeaponMesh = LocalCharacter->GetWeapon()->WeaponMesh;
	//FVector MuzzleLocation = WeaponMesh->GetSocketLocation(FName("MuzzleSocket")); // 假设你的枪口插槽叫 "MuzzleSocket"
	FVector MuzzleLocation{0,0,0};

	if (LocalCharacter->ProjectileSpawnPoint)
	{
		// 如果存在，就用它的世界位置作为生成点。这个位置不受动画影响！
		MuzzleLocation = LocalCharacter->ProjectileSpawnPoint->GetComponentLocation();
	}
	// 添加日志来调试
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Health: %s"), GetHealth()));
	}*/
	// 5. 计算从枪口指向目标点(HitLocation)的最终旋转方向
	FRotator FinalRotation = (HitLocation - MuzzleLocation).Rotation();

	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
		
	if (DamageSpecHandle.IsValid())
	{
		const URL_AS_Base* LocalAttributes = Cast<URL_AS_Base>(ActorInfo->AbilitySystemComponent->GetAttributeSet(URL_AS_Base::StaticClass()));
		const float StrengthBonus = LocalAttributes ? LocalAttributes->GetStrength() * 0.5f : 0.f; // 假设力量对伤害有加成
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
		// 使用 `SpawnActorDeferred` 来确保我们可以在生成后设置参数
			ARL_Actor_Bullet* Ammo = GetWorld()->SpawnActorDeferred<ARL_Actor_Bullet>(
			AmmoActorClass,
			SpawnTransform,
			LocalCharacter,
			LocalCharacter,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		if (Ammo)
		{
		//	 **关键步骤**: 将配置好的 Damage Spec Handle 传递给子弹
			Ammo->SetDamageEffectSpecHandle(DamageSpecHandle);
		//  完成生成
			Ammo->FinishSpawning(SpawnTransform);
		}
	}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

	