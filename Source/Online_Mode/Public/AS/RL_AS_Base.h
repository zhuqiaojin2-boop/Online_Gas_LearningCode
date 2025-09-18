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

	// GameplayEffect ִ�к���ô˺������Ǵ����˺������Ƶ�Ԫ���Ե����λ��
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// �������縴��
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//���ඨ���������:����,Ѫ��,�����ֵ,������,�ٶ�,����
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

	//---------Ԫ����---------//
	//���洢״̬,������GameEffect��PostGameplayEffectExecute֮��˲ʱ����
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
	

	//��ֻ�Ǹ���ת����,�������Ҫ�㲥ί��
	UFUNCTION()
	virtual void BroadcastPlayerDeath();
};
