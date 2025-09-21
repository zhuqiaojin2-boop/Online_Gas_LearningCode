// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../RL_Enum_AbilityInputID.h"
#include "Container/RL_Container_GEContainer.h"
#include "RL_GA_Base.generated.h"

/**
 * 
 */
 struct FGameplayEffectContainer;
 struct FGameplayEffectHandleSpecContainer;
UCLASS()
class ONLINE_MODE_API URL_GA_Base : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FGameplayEffectContainer> EffectContainerMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	ERL_AbilityInputID AbilityInputID = ERL_AbilityInputID::None; // Ĭ�ϲ����κ�����	

	FGameplayEffectHandleSpecContainer ConstructHandleSpecsFromContainer(FGameplayEffectContainer Container); // ���浱ǰ��AbilitySpecHandle

	TArray<FActiveGameplayEffectHandle> ApplyEffectForEachSpecHandles(FGameplayEffectHandleSpecContainer& HandleSpecContainer);

	void CalculateFinalDamage(float& BaseDamgage, float Strength);

	float GetAttribute(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAttribute Attribute) const;

	
};
