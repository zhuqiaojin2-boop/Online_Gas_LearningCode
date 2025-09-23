// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetControllerInterface.generated.h"

class UWidgetController;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class ONLINE_MODE_API IWidgetControllerInterface
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RL")
	void SetWidgetController(UWidgetController* InWidgetController);
};
