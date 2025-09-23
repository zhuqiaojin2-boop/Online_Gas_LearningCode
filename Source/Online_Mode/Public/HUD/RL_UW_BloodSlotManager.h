// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/WidgetControllerInterface.h"
#include <Components/VerticalBox.h>
#include "RL_UW_BloodSlotManager.generated.h"

/**
 * 
 */
 class URL_UW_BloodSlot;
 struct FPlayerState_AttributeData;
UCLASS()
class ONLINE_MODE_API URL_UW_BloodSlotManager : public UUserWidget,public IWidgetControllerInterface
{
	GENERATED_BODY()
public: 
	
	virtual void SetWidgetController_Implementation(UWidgetController* InWidgetController) override;

	UPROPERTY(EditAnywhere, Category = "RL")
	TSubclassOf<URL_UW_BloodSlot>BloodSlotClass;

	UPROPERTY(BlueprintReadOnly, Category = "RL")
	TObjectPtr<UWidgetController> WidgetController;

    // **在BeginPlay/NativeOnInitialized中开始监听**
    virtual void NativeOnInitialized() override;

    // **当新玩家的PlayerState被创建并复制到本地时，这个函数会被调用**
    UFUNCTION()
    void OnPlayerStateAdded(APlayerState* NewPlayerState);

    // **当一个玩家离开，他的PlayerState被销毁时，这个函数会被调用**
    UFUNCTION()
    void OnPlayerStateRemoved(APlayerState* RemovedPlayerState);

    // **一个在UMG编辑器中放置的VerticalBox，用来自动布局所有血条**
    // 记得在Manager的蓝图中，创建一个叫"BloodSlotBox"的VerticalBox并勾选IsVariable
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> BloodSlotBox;

    UPROPERTY(EditDefaultsOnly, Category = "RL")
    FVector2D BloodSlotPadding;

    UPROPERTY(EditAnywhere, Category = "RL")
    TObjectPtr<UDataTable> AvatarDataTable;
private:

    void CreatePlayerBloodSlots();

    void MakeInitDataFromPS(APlayerState* PS, FPlayerState_AttributeData& InitData);
};
