// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/RL_Interface_Death.h"
#include "RL_Character_Base.generated.h"

// ǰ������������ͷ�ļ�����
class ARL_Actor_Weapon;
class UAbilitySystemComponent;
class URL_GA_Base;
class URL_GE_Base;
class URL_AS_Player;

UCLASS()
class ONLINE_MODE_API ARL_Character_Base : public ACharacter, public IAbilitySystemInterface,public IRL_Interface_Death
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARL_Character_Base();

	// ʵ�� IAbilitySystemInterface �Ľӿ�
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// ��ȡ���Լ��ĺ���
	UFUNCTION(BlueprintPure, Category = "Abilities")
	URL_AS_Player* GetAttributeSetFromPS() const;

	// ��ȡ�����ĺ���
	UFUNCTION(BlueprintPure, Category = "Weapon")
	ARL_Actor_Weapon* GetWeapon() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ARL_Actor_Weapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName;

	// ��������
	virtual void HandleDeath_Implementation() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// �����Character��Controllerӵ�е�ʱ��,��������ᱻ����
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// �������ɲ���,����Ĭ�Ͽ�����һ������,Ȼ���ҿ��Ը���Class���������ɲ�ͬ����
	UPROPERTY(Replicated)
	TObjectPtr<ARL_Actor_Weapon> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<URL_GA_Base>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<URL_GE_Base> DefaultAttributesEffect;

	// �����������ɲ�װ�������ĺ���
	void EquipWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeath();

private:
	// ��ʼ�� ASC �ĺ���
	void InitializeAbilitySystem();
};