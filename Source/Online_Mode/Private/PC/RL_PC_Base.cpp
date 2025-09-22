#include "PC/RL_PC_Base.h"
#include "AbilitySystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"	
#include "ASC/RL_ASC_Base.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Online_ModeCharacter.h"
#include "PS/RL_PS_Base.h"
#include "AS/RL_AS_Player.h"
#include "InputActionValue.h"
#include "HUD/RL_HUD_Base.h"
#include "WidgetController/WidgetController.h"

void ARL_PC_Base::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);
	SetupPlayerInputComponent(InputComponent);
}

void ARL_PC_Base::OnShootInputPressed()
{
	SendInputToASC(ERL_AbilityInputID::Shoot, true);
}

void ARL_PC_Base::OnShootInputReleased()
{
    SendInputToASC(ERL_AbilityInputID::Shoot, false);
}

URL_ASC_Base* ARL_PC_Base::GetPawnASC() const
{
	// ʹ�����ǵĽӿ�����ȡASC���ǳ��ɾ�
	if (GetLocalPlayerStateWithASC())
	{
		return Cast<URL_ASC_Base>(GetLocalPlayerStateWithASC()->GetAbilitySystemComponent()) ? Cast<URL_ASC_Base>(GetLocalPlayerStateWithASC()->GetAbilitySystemComponent()) : NULL;
	}
	return nullptr;
}

void ARL_PC_Base::SendInputToASC(ERL_AbilityInputID InputID, bool bIsPressed) const // �ؼ����� , ��Ҫִ�е�GA	���͵�
{
	URL_ASC_Base* ASC = GetPawnASC();
    if (ASC)
    {
        if (bIsPressed)
        {
	        ASC->AbilityLocalInputPressed(static_cast<int32>(InputID));
			UE_LOG(LogTemp, Warning, TEXT("Sending input to ASC: %d, %d"), static_cast<int32>(InputID), bIsPressed);
        }
        else
        {
            ASC->AbilityLocalInputReleased(static_cast<int32>(InputID));
			UE_LOG(LogTemp, Warning, TEXT("Sending input to ASC: %d, %d"), static_cast<int32>(InputID), bIsPressed);
        }
    }
}

void ARL_PC_Base::UpdateBloodSlot(float Health)
{
	
}

float ARL_PC_Base::GetLocalCharacterHealth() const
{
	if (GetLocalPlayerStateWithASC())
	{
		return GetLocalPlayerStateWithASC()->GetAttributeSet()->GetHealth();
	}
	//TODO:Ȩ��֮��, ����ȡ�����������״̬ʱ, ��Ҫ����Ĵ���ʽ,����֪�������bug��
	return 0.0f;
}

ARL_PS_Base* ARL_PC_Base::GetLocalPlayerStateWithASC() const
{
	if (IAbilitySystemInterface* GASCharacterState = Cast<IAbilitySystemInterface>(PlayerState))
	{
		return Cast<ARL_PS_Base>(GASCharacterState);
	}
	return nullptr;

	UE_LOG(LogTemp, Warning, TEXT("PlayerState Cast to IAbilitySystemInterface is failed!"));
}

void ARL_PC_Base::Reload()
{
	SendInputToASC(ERL_AbilityInputID::Reload, true);
}

void ARL_PC_Base::SetIsAiming()
{
	if (!IsLocalController())return;
	IsAiming = true;
	Cast<AOnline_ModeCharacter>(GetPawn())->Server_SetIsAiming(IsAiming);
}

void ARL_PC_Base::SetIsNotAiming()
{	
	if(!IsLocalController())return;
	IsAiming = false;
	Cast<AOnline_ModeCharacter>(GetPawn())->Server_SetIsAiming(IsAiming);
}

void ARL_PC_Base::CreateHUDWidget()
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	// 1. ȷ����PC��ͼ��ָ����WidgetController��
	if (!WidgetControllerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ARL_PC_Base: WidgetControllerClass is not set!"));
		return;
	}

	// 2. ���� WidgetController ��ʵ��
	WidgetController = NewObject<UWidgetController>(this, WidgetControllerClass);

	// 3. ��������������WidgetController
	ARL_PS_Base* PS = GetPlayerState<ARL_PS_Base>();
	if (PS && WidgetController)
	{
		// ��PlayerState��ȡASC��AS
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();
		// ��������
		const FWidgetControllerParams WCParams(this, PS, ASC, AS);
		// ���ò���
		WidgetController->SetWidgetControllerParams(WCParams);
		// �󶨻ص�
		WidgetController->BindCallbacksToDependencies();
		// �㲥��ʼֵ�����ͨ����UI�Լ����ã���Ҳ���������ﴥ����
		// WidgetController->BroadcastInitialValues();
	}

	// 4. ��������ʾ��HUD�ؼ�
	if (HUDClass)
	{
		HUDWidget = CreateWidget<URL_HUD_Base>(this, HUDClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->SetWidgetController(WidgetController);
		}
	}
}

void ARL_PC_Base::BeginPlay()
{
    Super::BeginPlay();
	// 1. ��ȡ��ǿ���뱾�������ϵͳ
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// 2. **����**: ������ǵ�Ĭ������ӳ��������
			// ���ȼ�(Priority)Ϊ0�������ж����ͬ���ȼ���������
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	CreateHUDWidget();
	// 3. (�Ƽ�) ��������ģʽΪֻ������Ϸ����
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
void ARL_PC_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ARL_PC_Base::HandleJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARL_PC_Base::HandleStopJump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARL_PC_Base::Move);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ARL_PC_Base::OnShootInputPressed);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ARL_PC_Base::OnShootInputReleased);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ARL_PC_Base::SetIsAiming);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ARL_PC_Base::SetIsNotAiming);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ARL_PC_Base::Reload);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARL_PC_Base::Look);
		UE_LOG(LogTemp, Warning, TEXT("ARL_Character_Base::SetupPlayerInputComponent EXECUTED!"));
	}
	else
	{
		//TODO: ��Ҫһ����ǰ���LOG��־
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARL_PC_Base::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (true)
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
		GetPawn()->AddMovementInput(RightDirection, MovementVector.X);

		UE_LOG(LogTemp, Warning, TEXT("Moving"));
	}
}

void ARL_PC_Base::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (true)
	{
		// add yaw and pitch input to controller
		AddYawInput(LookAxisVector.X);
		AddPitchInput(LookAxisVector.Y);
	}
}

void ARL_PC_Base::HandleJump()
{
	ACharacter* LocalCharacter = Cast<ACharacter>(GetPawn());
	if (LocalCharacter)
	{
		LocalCharacter->Jump();
	}
}

void ARL_PC_Base::HandleStopJump()
{
	ACharacter* LocalCharacter = Cast<ACharacter>(GetPawn());
	if (LocalCharacter)
	{
		LocalCharacter->StopJumping();
	}
}
