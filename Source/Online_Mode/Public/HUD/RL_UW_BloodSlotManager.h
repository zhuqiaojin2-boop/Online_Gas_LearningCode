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

    // **��BeginPlay/NativeOnInitialized�п�ʼ����**
    virtual void NativeOnInitialized() override;

    // **������ҵ�PlayerState�����������Ƶ�����ʱ����������ᱻ����**
    UFUNCTION()
    void OnPlayerStateAdded(APlayerState* NewPlayerState);

    // **��һ������뿪������PlayerState������ʱ����������ᱻ����**
    UFUNCTION()
    void OnPlayerStateRemoved(APlayerState* RemovedPlayerState);

    // **һ����UMG�༭���з��õ�VerticalBox�������Զ���������Ѫ��**
    // �ǵ���Manager����ͼ�У�����һ����"BloodSlotBox"��VerticalBox����ѡIsVariable
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
