// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/AttributeWidgetController.h"
#include "AS/RL_AS_Player.h"          // 包含具体的属性集头文件
#include "PS/RL_PS_Base.h"              // 包含具体的玩家状态头文件
#include "GameplayTagContainer.h"

//URL_AS_Player* UAttributeWidgetController::GetPlayerAttributeSet() const
//{
//    return Cast<URL_AS_Player>(TransResults);
//}

void UAttributeWidgetController::ApplyAttributePoint(const FGameplayTag& AttributeTag)
{
    //if (AbilitySystemComponent && AttributeTag.IsValid())
    //{
    //    // 将UI的“加点”请求，翻译成一个GameplayEvent发送给后端
    //    // GA会监听这个事件，并执行真正的加点逻辑
    //    FGameplayEventData Payload;
    //    Payload.EventTag = AttributeTag; // 例如：Attributes.Apply.Strength
    //    AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
    //}
}

void UAttributeWidgetController::BroadcastInitialValues()
{
    // 遍历所有玩家的数据 (这个AllPlayersData来自我们上一问中重构的父类 UWidgetController)
    for (APlayerState* PlayerData : TransResults.PSDataResults)
    {
        ARL_PS_Base* LocalPlayerData = Cast<ARL_PS_Base>(PlayerData);

        if (!LocalPlayerData->GetAttributeSet()|| !LocalPlayerData) continue;

        const URL_AS_Player* AS = Cast<URL_AS_Player>(LocalPlayerData->GetAttributeSet());
        if (AS)
        {
            // KEY CHANGE 2: 广播时，带上是哪个PlayerState
            OnHealthChanged.Broadcast(LocalPlayerData->GetPlayerId(), AS->GetHealth());
            OnMaxHealthChanged.Broadcast(LocalPlayerData->GetPlayerId(), AS->GetMaxHealth());
        }
    }
}

void UAttributeWidgetController::BindCallbacksToDependencies()
{
    // 遍历所有玩家的数据
    for (APlayerState* PlayerData : TransResults.PSDataResults)
    {
        ARL_PS_Base* LocalPlayerData = Cast<ARL_PS_Base>(PlayerData);

        if (!LocalPlayerData->GetAbilitySystemComponent() || !LocalPlayerData->GetAttributeSet()) continue;

        URL_AS_Player* AS = LocalPlayerData->GetAttributeSet();

        UAbilitySystemComponent* ASC = LocalPlayerData->GetAbilitySystemComponent();

        if (AS && ASC)
        {
            // 使用 Lambda 捕获当前玩家的 PlayerState
            ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
                [this, PlayerState = LocalPlayerData](const FOnAttributeChangeData& Data)
                {
                    // 当GAS属性变化时，广播我们自己的、带身份信息的委托
                    OnHealthChanged.Broadcast(PlayerState->GetPlayerId(), Data.NewValue);
                }
            );

            ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
                [this, PlayerState = LocalPlayerData](const FOnAttributeChangeData& Data)
                {
                    OnMaxHealthChanged.Broadcast(PlayerState->GetPlayerId() , Data.NewValue);
                }
            );
        }
    }
}

//void UAttributeWidgetController::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
//{
//    // 当后端的Strength属性变化时，我们广播自己的委托，将新值推送给UI
//    OnHealthChanged.Broadcast(Data.NewValue);
//}
//
//void UAttributeWidgetController::OnMaxHealthAttributeChanged(const FOnAttributeChangeData& Data)
//{
//    OnMaxHealthChanged.Broadcast(Data.NewValue);
//}

//void UAttributeWidgetController::OnAttributePointsAttributeChanged(const FOnAttributeChangeData& Data)
//{
//    OnAttributePointsChanged.Broadcast(Data.NewValue);
//}