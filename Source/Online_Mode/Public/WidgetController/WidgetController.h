	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystemComponent.h" // 需要包含，因为要使用FOnAttributeChangeData
#include "WidgetController/WidgetControllerParams.h" // 包含我们刚创建的结构体
#include "PlayerState_AttributeResult.h"
#include "WidgetController.generated.h"
    
/**
 * 
 */
struct FPlayerState_AttributeResult;
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
    UFUNCTION(BlueprintCallable, Category = "RL")
    void SetWidgetControllerParams(const FPlayerState_AttributeResult& Result);

    /**
     * @brief 当所有参数都设置好后，广播所有属性的初始值给UI。
     *        UI可以在BeginPlay或Construct时调用此函数来获取第一帧的数据。
     */
    UFUNCTION(BlueprintCallable, Category = "RL")
    virtual void BroadcastInitialValues();

    /**
     * @brief 将Controller的回调函数绑定到后端的委托上（例如属性变化）。
     */
    virtual void BindCallbacksToDependencies();

    TArray<APlayerState*> GetPlayerStates() const;
protected:
    UPROPERTY(BlueprintReadOnly, Category = "RL")
    FPlayerState_AttributeResult TransResults;
};
