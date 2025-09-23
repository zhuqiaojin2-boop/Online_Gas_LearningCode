// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController/WidgetController.h"
#include "AttributeWidgetController.generated.h"

/**
 * 
 */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributePointsChangedSignature, int32, NewAttributePoints);
// ��ĳ���������ԣ������������仯ʱ���㲥���ί��
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerAttributeChangedSignature, int32, PlayerID, float, NewValue);

class URL_AS_Player;
UCLASS()
class ONLINE_MODE_API UAttributeWidgetController : public UWidgetController
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void ApplyAttributePoint(const FGameplayTag& AttributeTag);

    //// ** ���ί�� (UI���Զ���) **
    //UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    //FOnAttributePointsChangedSignature OnAttributePointsChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnPlayerAttributeChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnPlayerAttributeChangedSignature OnMaxHealthChanged;

    // ... �������Ե�ί�� ...

    // --- ��д���ຯ�� ---
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;

protected:
    // �ص���������������Ա仯ʱ����Щ�����ᱻ����
    //void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
    //void OnMaxHealthAttributeChanged(const FOnAttributeChangeData& Data);
    //void OnAttributePointsAttributeChanged(const FOnAttributeChangeData& Data);

private:
    // һ���������������ڻ�ȡ������Լ�
    //URL_AS_Player* GetPlayerAttributeSet() const;
};
