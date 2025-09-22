// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/AttributeWidgetController.h"
#include "AS/RL_AS_Player.h"          // 包含具体的属性集头文件
#include "PS/RL_PS_Base.h"              // 包含具体的玩家状态头文件
#include "GameplayTagContainer.h"

URL_AS_Player* UAttributeWidgetController::GetPlayerAttributeSet() const
{
    return Cast<URL_AS_Player>(AttributeSet);
}

void UAttributeWidgetController::ApplyAttributePoint(const FGameplayTag& AttributeTag)
{
    if (AbilitySystemComponent && AttributeTag.IsValid())
    {
        // 将UI的“加点”请求，翻译成一个GameplayEvent发送给后端
        // GA会监听这个事件，并执行真正的加点逻辑
        FGameplayEventData Payload;
        Payload.EventTag = AttributeTag; // 例如：Attributes.Apply.Strength
        AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
    }
}

void UAttributeWidgetController::BroadcastInitialValues()
{
    Super::BroadcastInitialValues();

    URL_AS_Player* AS = GetPlayerAttributeSet();
    if (!AS) return;

    // 广播所有属性的初始值
    OnStrengthChanged.Broadcast(AS->GetStrength());
    OnDefenceChanged.Broadcast(AS->GetDefence());
    // ... 广播其他属性 ...

    //OnAttributePointsChanged.Broadcast(AS->GetAttributePoints());
}

void UAttributeWidgetController::BindCallbacksToDependencies()
{
    Super::BindCallbacksToDependencies();

    URL_AS_Player* AS = GetPlayerAttributeSet();
    if (!AS || !AbilitySystemComponent) return;

    // **核心步骤**: 将我们的回调函数，绑定到ASC的属性变化委托上
    // 每当后端的“力量”属性变化时，我们的OnStrengthAttributeChanged就会被调用
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        AS->GetStrengthAttribute()
    ).AddUObject(this, &UAttributeWidgetController::OnStrengthAttributeChanged);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        AS->GetDefenceAttribute()
    ).AddUObject(this, &UAttributeWidgetController::OnDefenceAttributeChanged);

    // ... 绑定其他属性 ...

    // AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
    //     AS->GetAttributePointsAttribute()
    // ).AddUObject(this, &URL_AttributeWidgetController::OnAttributePointsAttributeChanged);
}

void UAttributeWidgetController::OnStrengthAttributeChanged(const FOnAttributeChangeData& Data)
{
    // 当后端的Strength属性变化时，我们广播自己的委托，将新值推送给UI
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