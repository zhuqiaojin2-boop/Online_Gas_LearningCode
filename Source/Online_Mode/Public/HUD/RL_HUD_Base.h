// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RL_HUD_Base.generated.h"

 class URL_UW_BloodSlot;
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
	//����Ѫ����һ�㼶,Ѫ������
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URL_UW_BloodSlot>BloodSlot;

	UPROPERTY(EditAnywhere,Category = "RL_HUD")
	TSubclassOf<URL_UW_BloodSlot>BloodSlotClass;
protected:
	// ���ǽ�������ΪBlueprintReadOnly��������ͼ����Ϳ��Է��������������޸�����
	UPROPERTY(BlueprintReadOnly, Category = "RL")
	TObjectPtr<UWidgetController> WidgetController;
};
