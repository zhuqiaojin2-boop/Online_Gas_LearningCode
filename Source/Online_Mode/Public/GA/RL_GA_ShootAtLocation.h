// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/RL_GA_Base.h"
#include "RL_GA_ShootAtLocation.generated.h"

/**
 * 
 */
UCLASS()
class ONLINE_MODE_API URL_GA_ShootAtLocation : public URL_GA_Base
{
	GENERATED_BODY()
	
private:
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, 
		const FGameplayTagContainer* TargetTags = nullptr, 
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly,Category = "RL_GA")
	float AmmoPadding{1.0f};
protected: 

	UPROPERTY(EditDefaultsOnly,Category = "RL_GA")
	TSubclassOf<UGameplayEffect>DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,Category = "RL_GA")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities") 
	TSubclassOf<UGameplayEffect> CostGE;

	UPROPERTY(EditDefaultsOnly,Category = "RL_GA")
	TSubclassOf<UGameplayEffect>AmmoCostEffectClass;

	UPROPERTY(EditDefaultsOnly,Category = "RL_GA")
	TSubclassOf<AActor>AmmoActorClass;
};

