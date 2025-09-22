// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../RL_Enum_AbilityInputID.h"
#include "RL_PC_Base.generated.h"

class URL_ASC_Base;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class URL_HUD_Base;
class ARL_PS_Base;
class URL_HUD_Base;
class UWidgetController;

UCLASS()
class ONLINE_MODE_API ARL_PC_Base : public APlayerController
{
	GENERATED_BODY()
public:
	URL_ASC_Base* GetPawnASC() const;
protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))

	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RL_PC")
	bool IsAiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RL_PC")
	TSubclassOf<URL_HUD_Base>HUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RL_PC")
	TObjectPtr<URL_HUD_Base>HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RL_PC")
	TObjectPtr<UWidgetController> WidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UWidgetController> WidgetControllerClass;
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void HandleJump();

	void HandleStopJump();

	float GetLocalCharacterHealth() const;

	ARL_PS_Base* GetLocalPlayerStateWithASC() const;
	//TODO: 换弹能力应该要在武器类中携带.
	void Reload();

	UFUNCTION()
	void SetIsAiming();

	UFUNCTION()
	void SetIsNotAiming();

	void CreateHUDWidget();
private:

	void OnShootInputPressed();

	void OnShootInputReleased();

	void SendInputToASC(ERL_AbilityInputID InputID, bool bIsPressed) const;

	void UpdateBloodSlot(float Health);

};
