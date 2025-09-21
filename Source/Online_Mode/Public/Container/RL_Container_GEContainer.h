// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "RL_Container_GEContainer.generated.h"
struct FGameplayEffectSpecHandle;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FGameplayEffectContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "RL_Container")
	TArray<TSubclassOf<UGameplayEffect>>EffectClasses;
private:
		
};	

USTRUCT(BlueprintType)
struct FGameplayEffectHandleSpecContainer
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "RL_Container")
	TArray<FGameplayEffectSpecHandle>EffectSpecHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;
private:
	
};