	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystemComponent.h" // ��Ҫ��������ΪҪʹ��FOnAttributeChangeData
#include "WidgetController/WidgetControllerParams.h" // �������Ǹմ����Ľṹ��
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
     * @brief ����WidgetController��������к��ָ�롣
     *        �������Ӧ���ڴ���WidgetController���������á�
     * @param InParams ����PC, PS, ASC, AS�Ľṹ�塣
     */
    UFUNCTION(BlueprintCallable, Category = "RL")
    void SetWidgetControllerParams(const FPlayerState_AttributeResult& Result);

    /**
     * @brief �����в��������úú󣬹㲥�������Եĳ�ʼֵ��UI��
     *        UI������BeginPlay��Constructʱ���ô˺�������ȡ��һ֡�����ݡ�
     */
    UFUNCTION(BlueprintCallable, Category = "RL")
    virtual void BroadcastInitialValues();

    /**
     * @brief ��Controller�Ļص������󶨵���˵�ί���ϣ��������Ա仯����
     */
    virtual void BindCallbacksToDependencies();

    TArray<APlayerState*> GetPlayerStates() const;
protected:
    UPROPERTY(BlueprintReadOnly, Category = "RL")
    FPlayerState_AttributeResult TransResults;
};
