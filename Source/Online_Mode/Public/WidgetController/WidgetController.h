	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystemComponent.h" // ��Ҫ��������ΪҪʹ��FOnAttributeChangeData
#include "WidgetController/WidgetControllerParams.h" // �������Ǹմ����Ľṹ��
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
     * @brief ����WidgetController��������к��ָ�롣
     *        �������Ӧ���ڴ���WidgetController���������á�
     * @param InParams ����PC, PS, ASC, AS�Ľṹ�塣
     */
    UFUNCTION(BlueprintCallable, Category = "WidgetController")
    void SetWidgetControllerParams(const FWidgetControllerParams& InParams);

    /**
     * @brief �����в��������úú󣬹㲥�������Եĳ�ʼֵ��UI��
     *        UI������BeginPlay��Constructʱ���ô˺�������ȡ��һ֡�����ݡ�
     */
    UFUNCTION(BlueprintCallable, Category = "WidgetController")
    virtual void BroadcastInitialValues();

    /**
     * @brief ��Controller�Ļص������󶨵���˵�ί���ϣ��������Ա仯����
     */
    virtual void BindCallbacksToDependencies();


protected:
    // ָ����ĺ�˶����ָ��
    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<APlayerState> PlayerState;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAttributeSet> AttributeSet;
};
