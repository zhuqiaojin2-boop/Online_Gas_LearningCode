// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../RL_Enum_AbilityInputID.h"
#include "RL_GA_Base.generated.h"

/**
 * 
 */
UCLASS()
class ONLINE_MODE_API URL_GA_Base : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	ERL_AbilityInputID AbilityInputID = ERL_AbilityInputID::None; // 默认不绑定任何输入
};
