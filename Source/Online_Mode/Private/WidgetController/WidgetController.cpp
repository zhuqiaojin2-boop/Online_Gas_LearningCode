// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/WidgetController.h"

void UWidgetController::SetWidgetControllerParams(const FPlayerState_AttributeResult& Result)
{
    TransResults = Result;
}

void UWidgetController::BroadcastInitialValues()
{
    
}
void UWidgetController::BindCallbacksToDependencies()
{

}

TArray<APlayerState*> UWidgetController::GetPlayerStates() const
{
    return TransResults.PSDataResults;
}
