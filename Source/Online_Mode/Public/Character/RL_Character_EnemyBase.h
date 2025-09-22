// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RL_Character_Base.h"
#include "RL_Character_EnemyBase.generated.h"

/**
 * 
 */
 class UBehaviorTree;
 class URL_ASC_Base;
 class URL_AS_Enemy;
UCLASS()
class ONLINE_MODE_API ARL_Character_EnemyBase : public ARL_Character_Base
{
	GENERATED_BODY()
public:

	ARL_Character_EnemyBase();

	virtual URL_AS_Base* GetAttributeSet() const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
private:

	UPROPERTY(VisibleAnywhere,Category = "RL")
	TObjectPtr<URL_AS_Enemy>Character_AS;																																			

	UPROPERTY(VisibleAnywhere, Category = "RL")
	TObjectPtr<URL_ASC_Base>Character_ASC;

	TObjectPtr<UBehaviorTree>BehaviorTree;

	void CreateAttributeSet();

	//UPROPERTY(EditDefaultsOnly, Category = "RL")
	//TObjectPtr<URL_ASC_Base>Character_ASC;

	UPROPERTY(EditDefaultsOnly, Category = "RL")
	TSubclassOf<URL_AS_Base> AttributeSetClass;
protected:
};
