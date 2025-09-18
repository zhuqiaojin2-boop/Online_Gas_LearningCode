// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RL_Character_Base.h"
#include "RL_Character_EnemyBase.generated.h"

/**
 * 
 */
 class UBehaviorTree;

UCLASS()
class ONLINE_MODE_API ARL_Character_EnemyBase : public ARL_Character_Base
{
	GENERATED_BODY()

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:

	TObjectPtr<UBehaviorTree>BehaviorTree;
};
