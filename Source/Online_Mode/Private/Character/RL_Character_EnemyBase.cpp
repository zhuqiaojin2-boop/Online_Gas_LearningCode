// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RL_Character_EnemyBase.h"
#include "ASC/RL_ASC_Base.h"
#include "AS/RL_AS_Enemy.h"
#include <Net/UnrealNetwork.h>

ARL_Character_EnemyBase::ARL_Character_EnemyBase()
{
    Character_ASC = CreateDefaultSubobject<URL_ASC_Base>(TEXT("AbilitySystemComponent"));

    CreateAttributeSet();

    Character_ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* ARL_Character_EnemyBase::GetAbilitySystemComponent() const
{
    //使用时要Cast  
	return Character_ASC;
}

URL_AS_Base* ARL_Character_EnemyBase::GetAttributeSet() const
{
    //使用时要Cast
    return Character_AS;
}

void ARL_Character_EnemyBase::CreateAttributeSet()
{
    // 检查子类是否指定了一个有效的AttributeSet类
    if (AttributeSetClass)
    {
        Character_AS = NewObject<URL_AS_Enemy>(this, AttributeSetClass, TEXT("AttributeSet"));

        if (Character_AS)
        {
            UE_LOG(LogTemp, Log, TEXT("%s created AttributeSet of class %s"), *GetName(), *AttributeSetClass->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s has no AttributeSetClass specified. AttributeSet will not be created."), *GetName());
    }
}