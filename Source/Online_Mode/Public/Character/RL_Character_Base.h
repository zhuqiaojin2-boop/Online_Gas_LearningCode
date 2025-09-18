// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/RL_Interface_Death.h"
#include "RL_Character_Base.generated.h"

// 前向声明，减少头文件依赖
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

	// 实现 IAbilitySystemInterface 的接口
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 获取属性集的函数
	UFUNCTION(BlueprintPure, Category = "Abilities")
	URL_AS_Player* GetAttributeSetFromPS() const;

	// 获取武器的函数
	UFUNCTION(BlueprintPure, Category = "Weapon")
	ARL_Actor_Weapon* GetWeapon() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ARL_Actor_Weapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName;

	// 死亡处理
	virtual void HandleDeath_Implementation() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 当这个Character被Controller拥有的时候,这个函数会被调用
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 武器生成策略,首先默认可以有一个武器,然后我可以根据Class的设置生成不同武器
	UPROPERTY(Replicated)
	TObjectPtr<ARL_Actor_Weapon> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<URL_GA_Base>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<URL_GE_Base> DefaultAttributesEffect;

	// 服务器上生成并装备武器的函数
	void EquipWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeath();

private:
	// 初始化 ASC 的函数
	void InitializeAbilitySystem();
};