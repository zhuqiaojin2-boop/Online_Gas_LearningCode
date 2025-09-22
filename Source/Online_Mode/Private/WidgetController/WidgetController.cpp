// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/WidgetController.h"

void UWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InParams)
{
    PlayerController = InParams.PlayerController;
    PlayerState = InParams.PlayerState;
    AbilitySystemComponent = InParams.AbilitySystemComponent;
    AttributeSet = InParams.AttributeSet;
}

void UWidgetController::BroadcastInitialValues()
{

}
void UWidgetController::BindCallbacksToDependencies()
{

}