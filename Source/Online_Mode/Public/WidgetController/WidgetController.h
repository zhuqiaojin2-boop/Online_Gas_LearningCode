	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystemComponent.h" // 需要包含，因为要使用FOnAttributeChangeData
#include "WidgetController/WidgetControllerParams.h" // 包含我们刚创建的结构体
#include "WidgetController.generated.h"
    
/**
 * 
 */
UCLASS()
class ONLINE_MODE_API UWidgetController : public UObject
{
	GENERATED_BODY()
public:
    /**
     * @brief 设置WidgetController所需的所有后端指针。
     *        这个函数应该在创建WidgetController后立即调用。
     * @param InParams 包含PC, PS, ASC, AS的结构体。
     */
    UFUNCTION(BlueprintCallable, Category = "WidgetController")
    void SetWidgetControllerParams(const FWidgetControllerParams& InParams);

    /**
     * @brief 当所有参数都设置好后，广播所有属性的初始值给UI。
     *        UI可以在BeginPlay或Construct时调用此函数来获取第一帧的数据。
     */
    UFUNCTION(BlueprintCallable, Category = "WidgetController")
    virtual void BroadcastInitialValues();

    /**
     * @brief 将Controller的回调函数绑定到后端的委托上（例如属性变化）。
     */
    virtual void BindCallbacksToDependencies();


protected:
    // 指向核心后端对象的指针
    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<APlayerState> PlayerState;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAttributeSet> AttributeSet;
};
