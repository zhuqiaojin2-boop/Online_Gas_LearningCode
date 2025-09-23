// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RL_HUD_Base.generated.h"

 class URL_UW_BloodSlotManager;
 class UWidgetController;
UCLASS()
class ONLINE_MODE_API URL_HUD_Base : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable, Category = "RL")
	void SetWidgetController(UWidgetController* InWidgetController);
private:
	//并非血条第一层级,血条父级
	UPROPERTY(VisibleAnywhere, Category = "RL",meta = (BindWidget))
	TObjectPtr<URL_UW_BloodSlotManager>BloodSlotManager;
protected:
	// 我们将其设置为BlueprintReadOnly，这样蓝图子类就可以访问它，但不能修改它。
	UPROPERTY(BlueprintReadOnly, Category = "RL")
	TObjectPtr<UWidgetController> WidgetController;

	virtual void OnWidgetControllerSet();
};
