// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController/WidgetController.h"
#include "AttributeWidgetController.generated.h"

/**
 * 
 */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributePointsChangedSignature, int32, NewAttributePoints);
// 当某个具体属性（比如力量）变化时，广播这个委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerAttributeChangedSignature, int32, PlayerID, float, NewValue);

class URL_AS_Player;
UCLASS()
class ONLINE_MODE_API UAttributeWidgetController : public UWidgetController
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void ApplyAttributePoint(const FGameplayTag& AttributeTag);

    //// ** 输出委托 (UI可以订阅) **
    //UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    //FOnAttributePointsChangedSignature OnAttributePointsChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnPlayerAttributeChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnPlayerAttributeChangedSignature OnMaxHealthChanged;

    // ... 其他属性的委托 ...

    // --- 重写基类函数 ---
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;

protected:
    // 回调函数，当后端属性变化时，这些函数会被触发
    //void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
    //void OnMaxHealthAttributeChanged(const FOnAttributeChangeData& Data);
    //void OnAttributePointsAttributeChanged(const FOnAttributeChangeData& Data);

private:
    // 一个辅助函数，用于获取玩家属性集
    //URL_AS_Player* GetPlayerAttributeSet() const;
};
