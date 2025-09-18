// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RL_Interface_Death.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URL_Interface_Death : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ONLINE_MODE_API IRL_Interface_Death
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RL/Interface/Death")
	void HandleDeath();
};
