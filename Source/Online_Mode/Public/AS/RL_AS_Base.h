// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RL_AS_Base.generated.h"

/**
 * 
 */
 #define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);

UCLASS()
class ONLINE_MODE_API URL_AS_Base : public UAttributeSet
{
	GENERATED_BODY()
public:

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// GameplayEffect 执行后调用此函数，是处理伤害、治疗等元属性的最佳位置
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// 用于网络复制
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//基类定义基础属性:力量,血量,最大经验值,防御力,速度,攻速
	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(URL_AS_Base, Strength)

	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URL_AS_Base, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URL_AS_Base, Health)

	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_Attack_frequency)
	FGameplayAttributeData Attack_frequency;
	ATTRIBUTE_ACCESSORS(URL_AS_Base, Attack_frequency)

	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_Defence)
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(URL_AS_Base, Defence)

	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_Speed)
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(URL_AS_Base, Speed)

	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(URL_AS_Base, Level)

	//---------元属性---------//
	//不存储状态,仅用于GameEffect和PostGameplayEffectExecute之间瞬时传递
	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(URL_AS_Base, Damage);

	/*UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes")
	FGameplayAttributeData CostAmmo;
	ATTRIBUTE_ACCESSORS(URL_AS_Base, CostAmmo);*/

	//FDeathEvent OnPlayerDeath;
protected:
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Attack_frequency(const FGameplayAttributeData& OldAttack_frequency);

	UFUNCTION()
	virtual void OnRep_Defence(const FGameplayAttributeData& OldDefence);

	UFUNCTION()
	virtual void OnRep_Speed(const FGameplayAttributeData& OldSpeed);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);
	

	//这只是个中转函数,大概率需要广播委托
	UFUNCTION()
	virtual void BroadcastPlayerDeath();
};
