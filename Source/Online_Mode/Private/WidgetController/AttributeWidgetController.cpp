// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/AttributeWidgetController.h"
#include "AS/RL_AS_Player.h"          // ������������Լ�ͷ�ļ�
#include "PS/RL_PS_Base.h"              // ������������״̬ͷ�ļ�
#include "GameplayTagContainer.h"

//URL_AS_Player* UAttributeWidgetController::GetPlayerAttributeSet() const
//{
//    return Cast<URL_AS_Player>(TransResults);
//}

void UAttributeWidgetController::ApplyAttributePoint(const FGameplayTag& AttributeTag)
{
    //if (AbilitySystemComponent && AttributeTag.IsValid())
    //{
    //    // ��UI�ġ��ӵ㡱���󣬷����һ��GameplayEvent���͸����
    //    // GA���������¼�����ִ�������ļӵ��߼�
    //    FGameplayEventData Payload;
    //    Payload.EventTag = AttributeTag; // ���磺Attributes.Apply.Strength
    //    AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
    //}
}

void UAttributeWidgetController::BroadcastInitialValues()
{
    // ����������ҵ����� (���AllPlayersData����������һ�����ع��ĸ��� UWidgetController)
    for (APlayerState* PlayerData : TransResults.PSDataResults)
    {
        ARL_PS_Base* LocalPlayerData = Cast<ARL_PS_Base>(PlayerData);

        if (!LocalPlayerData->GetAttributeSet()|| !LocalPlayerData) continue;

        const URL_AS_Player* AS = Cast<URL_AS_Player>(LocalPlayerData->GetAttributeSet());
        if (AS)
        {
            // KEY CHANGE 2: �㲥ʱ���������ĸ�PlayerState
            OnHealthChanged.Broadcast(LocalPlayerData->GetPlayerId(), AS->GetHealth());
            OnMaxHealthChanged.Broadcast(LocalPlayerData->GetPlayerId(), AS->GetMaxHealth());
        }
    }
}

void UAttributeWidgetController::BindCallbacksToDependencies()
{
    // ����������ҵ�����
    for (APlayerState* PlayerData : TransResults.PSDataResults)
    {
        ARL_PS_Base* LocalPlayerData = Cast<ARL_PS_Base>(PlayerData);

        if (!LocalPlayerData->GetAbilitySystemComponent() || !LocalPlayerData->GetAttributeSet()) continue;

        URL_AS_Player* AS = LocalPlayerData->GetAttributeSet();

        UAbilitySystemComponent* ASC = LocalPlayerData->GetAbilitySystemComponent();

        if (AS && ASC)
        {
            // ʹ�� Lambda ����ǰ��ҵ� PlayerState
            ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
                [this, PlayerState = LocalPlayerData](const FOnAttributeChangeData& Data)
                {
                    // ��GAS���Ա仯ʱ���㲥�����Լ��ġ��������Ϣ��ί��
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
//    // ����˵�Strength���Ա仯ʱ�����ǹ㲥�Լ���ί�У�����ֵ���͸�UI
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