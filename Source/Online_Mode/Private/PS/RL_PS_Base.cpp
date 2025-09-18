// Fill out your copyright notice in the Description page of Project Settings.


#include "PS/RL_PS_Base.h"
#include "ASC/RL_ASC_Base.h"
#include "AS/RL_AS_Player.h"
#include "GA/RL_GA_ShootAtLocation.h"
#include "GE/RL_GE_Base.h"
#include "GA/RL_GA_Reload.h"

ARL_PS_Base::ARL_PS_Base()
{
	PrimaryActorTick.bCanEverTick = true;
	if(RL_ASC == nullptr)
		RL_ASC = CreateDefaultSubobject<URL_ASC_Base>(TEXT("RL_AbilitySystemComponent"));
	if(RL_AS == nullptr)	
		RL_AS = CreateDefaultSubobject<URL_AS_Player>(TEXT("RL_AbilitySystem"));
	if(RL_ASC)
	{	
		RL_ASC->SetIsReplicated(true);
	// ���ģʽ���ʺ� PlayerState����ֻ���� GameplayEffect���� GameplayCue �ڿͻ��˱��ش���
		RL_ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	}
}

UAbilitySystemComponent* ARL_PS_Base::GetAbilitySystemComponent() const
{
	return RL_ASC;
}

URL_AS_Player* ARL_PS_Base::GetAttributeSet() const
{
	return RL_AS;
}

void ARL_PS_Base::BeginPlay()
{
	Super::BeginPlay();

    if (HasAuthority())
    {
        if (RL_ASC)
        {
			for(TSubclassOf<URL_GA_Base> AbilityClass : RL_Default_GA)
			{	
				if(AbilityClass)
				{
					URL_GA_Base* AbilityCDO = AbilityClass->GetDefaultObject<URL_GA_Base>();
					FGameplayAbilitySpec AbilitySpec(
					AbilityClass,
					1, 
					static_cast<int32>(AbilityCDO->AbilityInputID),
					this);
					RL_ASC->GiveAbility(AbilitySpec);
				}
			}
        }
	}
}

void ARL_PS_Base::Tick(float DeltaTime)
{
	URL_AS_Player* Test_AS = RL_AS;
}

//void ARL_PS_Base::InitializeAbilitySystem()
//{
//	if (RL_ASC)
//	{
//		RL_ASC->InitAbilityActorInfo(this, this);
//	}
//	if (HasAuthority())
//	{
//		// Ӧ�� GE �����ó�ʼ����ֵ (���� Health=100)
//		if (RL_GE_Class)
//		{
//			FGameplayEffectContextHandle ContextHandle = RL_ASC->MakeEffectContext();
//			ContextHandle.AddSourceObject(this);
//			FGameplayEffectSpecHandle SpecHandle = RL_ASC->MakeOutgoingSpec(RL_GE_Class, 1, ContextHandle);
//			if (SpecHandle.IsValid())
//			{
//				RL_ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
//			}
//		}
//	}
//}


