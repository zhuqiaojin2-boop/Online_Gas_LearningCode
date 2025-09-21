// Fill out your copyright notice in the Description page of Project Settings.


#include "AS/RL_AS_Base.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Interface/RL_Interface_Death.h"
#include "Character/RL_Character_Base.h"
#include "AbilitySystemComponent.h"
URL_AS_Base::URL_AS_Base()
	: Strength(1.f)
	, MaxHealth(1.f)
	, Health(0.f)
	, Defence(0.f)
	, Attack_frequency(1.0f)
	, Speed(1.0f)
	, Level(1.0f)
	, Damage(0.0f) {
}

void URL_AS_Base::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute,NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxHealth());
		float Healthy = GetMaxHealth();
	}
}



void URL_AS_Base::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float LocalDamage = GetDamage();
		SetDamage(0.f);
		if(LocalDamage <= 0.f)return;
		//如果伤害值小于75%的防御力,则伤害为0
		if(LocalDamage<= 0.75 * GetDefence()) LocalDamage = 0.f;
		//最后伤害值减去75%防御力
		LocalDamage -= 0.75 * GetDefence();

		const float OldHealth = GetHealth();
		float NewHealth = FMath::Clamp(OldHealth - LocalDamage, 0.f, GetMaxHealth());
		SetHealth(NewHealth);

		// 判断是否死亡 (检查旧血量>0是为了防止重复触发)
		if (OldHealth > 0.f && NewHealth <= 0.f)
		{
			// 1. 获取拥有此AS的ASC
			UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
			if (ASC)
			{
				// 2. 通过ASC，获取AvatarActor
				AActor* AvatarActor = ASC->GetAvatarActor();

				if (AvatarActor)
				{
					// 3. 在AvatarActor上检查并调用死亡接口
					IRL_Interface_Death* DeathInterface = Cast<IRL_Interface_Death>(AvatarActor);
					if (DeathInterface)
					{
						DeathInterface->Execute_HandleDeath(AvatarActor);
					}
					else
					{
						// 添加日志
						UE_LOG(LogTemp, Error, TEXT("%s's AvatarActor %s does not implement DeathInterface!"), *GetName(), *AvatarActor->GetName());
					}
				}
			}
		}
	}
}

void URL_AS_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URL_AS_Base, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URL_AS_Base, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URL_AS_Base, Attack_frequency, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URL_AS_Base, Defence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URL_AS_Base, Speed, COND_None, REPNOTIFY_Always);
}

void URL_AS_Base::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	//这个宏在Gas里必须调用, 它的功能 : 确保客户端属性值与服务器权威值一致；处理预测系统修正（回滚错误的预测值）；触发属性更新相关事件（如更新UI）。
	
	/*在GAS中，为了支持预测​（Prediction），客户端可能会提前修改属性值以快速响应玩家操作。然而，服务器的权威值最终可能不同。GAMEPLAYATTRIBUTE_REPNOTIFY 的
	作用就是用服务器的权威值来校正客户端的预测值。如果客户端的预测错了（比如服务器判定技能未命中），这个宏能确保客户端的值被回滚到服务器发来的正确状态，
	从而保持所有玩家状态的一致性和公平性*/

	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Base, Strength, OldStrength);
}
	
void URL_AS_Base::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Base, MaxHealth, OldMaxHealth);
}

void URL_AS_Base::OnRep_Attack_frequency(const FGameplayAttributeData& OldAttack_frequency)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Base, Attack_frequency, OldAttack_frequency);
}

void URL_AS_Base::OnRep_Defence(const FGameplayAttributeData& OldDefence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Base, Defence, OldDefence);
}

void URL_AS_Base::OnRep_Speed(const FGameplayAttributeData& OldSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Base, Speed, OldSpeed);
}

void URL_AS_Base::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Base, Health, OldHealth);
}

void URL_AS_Base::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Base, Level, OldLevel);
}
