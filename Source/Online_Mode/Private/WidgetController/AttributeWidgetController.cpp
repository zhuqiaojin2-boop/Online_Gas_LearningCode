// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/AttributeWidgetController.h"
#include "AS/RL_AS_Player.h"          // ������������Լ�ͷ�ļ�
#include "PS/RL_PS_Base.h"              // ������������״̬ͷ�ļ�
#include "GameplayTagContainer.h"

URL_AS_Player* UAttributeWidgetController::GetPlayerAttributeSet() const
{
    return Cast<URL_AS_Player>(AttributeSet);
}

void UAttributeWidgetController::ApplyAttributePoint(const FGameplayTag& AttributeTag)
{
    if (AbilitySystemComponent && AttributeTag.IsValid())
    {
        // ��UI�ġ��ӵ㡱���󣬷����һ��GameplayEvent���͸����
        // GA���������¼�����ִ�������ļӵ��߼�
        FGameplayEventData Payload;
        Payload.EventTag = AttributeTag; // ���磺Attributes.Apply.Strength
        AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
    }
}

void UAttributeWidgetController::BroadcastInitialValues()
{
    Super::BroadcastInitialValues();

    URL_AS_Player* AS = GetPlayerAttributeSet();
    if (!AS) return;

    // �㲥�������Եĳ�ʼֵ
    OnStrengthChanged.Broadcast(AS->GetStrength());
    OnDefenceChanged.Broadcast(AS->GetDefence());
    // ... �㲥�������� ...

    //OnAttributePointsChanged.Broadcast(AS->GetAttributePoints());
}

void UAttributeWidgetController::BindCallbacksToDependencies()
{
    Super::BindCallbacksToDependencies();

    URL_AS_Player* AS = GetPlayerAttributeSet();
    if (!AS || !AbilitySystemComponent) return;

    // **���Ĳ���**: �����ǵĻص��������󶨵�ASC�����Ա仯ί����
    // ÿ����˵ġ����������Ա仯ʱ�����ǵ�OnStrengthAttributeChanged�ͻᱻ����
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        AS->GetStrengthAttribute()
    ).AddUObject(this, &UAttributeWidgetController::OnStrengthAttributeChanged);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        AS->GetDefenceAttribute()
    ).AddUObject(this, &UAttributeWidgetController::OnDefenceAttributeChanged);

    // ... ���������� ...

    // AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
    //     AS->GetAttributePointsAttribute()
    // ).AddUObject(this, &URL_AttributeWidgetController::OnAttributePointsAttributeChanged);
}

void UAttributeWidgetController::OnStrengthAttributeChanged(const FOnAttributeChangeData& Data)
{
    // ����˵�Strength���Ա仯ʱ�����ǹ㲥�Լ���ί�У�����ֵ���͸�UI
    OnStrengthChanged.Broadcast(Data.NewValue);
}

void UAttributeWidgetController::OnDefenceAttributeChanged(const FOnAttributeChangeData& Data)
{
    OnDefenceChanged.Broadcast(Data.NewValue);
}

void UAttributeWidgetController::OnAttributePointsAttributeChanged(const FOnAttributeChangeData& Data)
{
    OnAttributePointsChanged.Broadcast(Data.NewValue);
}