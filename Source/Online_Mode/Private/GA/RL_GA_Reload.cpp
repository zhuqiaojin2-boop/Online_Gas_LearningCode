// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/RL_GA_Reload.h"
#include "AS/RL_AS_Player.h"
#include "ASC/RL_ASC_Base.h"

bool URL_GA_Reload::CanActivateAbility(
const FGameplayAbilitySpecHandle Handle, 
const FGameplayAbilityActorInfo* ActorInfo, 
const FGameplayTagContainer* SourceTags, 
const FGameplayTagContainer* TargetTags, 
OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) 
		return false;
	// 检查弹药是否足够
	const URL_ASC_Base* ASC = Cast<URL_ASC_Base>(ActorInfo->AbilitySystemComponent.Get());
	if (ASC)
	{
		const float TotalLeftAmmo = ASC->GetNumericAttribute(URL_AS_Player::GetMaxAmmoCountsAttribute());
		return TotalLeftAmmo > 0;
	}
	return false;
}

void URL_GA_Reload::ActivateAbility(
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
	//TODO:封装函数,太乱了
	if (CostGE)
	{
		CostGameplayEffectClass = CostGE;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	float ReloadAmmo;
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	const URL_AS_Player* LocalAttributes = Cast<URL_AS_Player>(ActorInfo->AbilitySystemComponent->GetAttributeSet(URL_AS_Player::StaticClass()));
	const float MaxAmmo = LocalAttributes->GetMaxAmmosInMagazine();
	const float CurrentAmmo = LocalAttributes->GetLeftAmmosInMagazine();
	const float NeedAmmo = MaxAmmo - CurrentAmmo;
	ReloadAmmo = FMath::Min(NeedAmmo, LocalAttributes->GetMaxAmmoCounts());
	

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

	// 1b. 使用 MakeOutgoingSpec 创建 Spec 的句柄 (Handle)
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ReloadGEClass, GetAbilityLevel(), ContextHandle);

	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Reload.Ammo")), ReloadAmmo);

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
