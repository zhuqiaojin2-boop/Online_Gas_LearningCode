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
	// 使用我们的接口来获取ASC，非常干净
	if (GetLocalPlayerStateWithASC())
	{
		return Cast<URL_ASC_Base>(GetLocalPlayerStateWithASC()->GetAbilitySystemComponent()) ? Cast<URL_ASC_Base>(GetLocalPlayerStateWithASC()->GetAbilitySystemComponent()) : NULL;
	}
	return nullptr;
}

void ARL_PC_Base::SendInputToASC(ERL_AbilityInputID InputID, bool bIsPressed) const // 关键函数 , 将要执行的GA	发送到
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
	//TODO:权宜之计, 当获取不到本地玩家状态时, 需要另外的处理方式,让我知道这里出bug了
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

	// 1. 确保在PC蓝图中指定了WidgetController类
	if (!WidgetControllerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ARL_PC_Base: WidgetControllerClass is not set!"));
		return;
	}

	// 2. 创建 WidgetController 的实例
	WidgetController = NewObject<UWidgetController>(this, WidgetControllerClass);

	// 3. 构建参数并设置WidgetController
	ARL_PS_Base* PS = GetPlayerState<ARL_PS_Base>();
	if (PS && WidgetController)
	{
		// 从PlayerState获取ASC和AS
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();
		// 构建参数
		const FWidgetControllerParams WCParams(this, PS, ASC, AS);
		// 设置参数
		WidgetController->SetWidgetControllerParams(WCParams);
		// 绑定回调
		WidgetController->BindCallbacksToDependencies();
		// 广播初始值（这个通常由UI自己调用，但也可以在这里触发）
		// WidgetController->BroadcastInitialValues();
	}

	// 4. 创建并显示主HUD控件
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
	// 1. 获取增强输入本地玩家子系统
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// 2. **核心**: 添加我们的默认输入映射上下文
			// 优先级(Priority)为0，可以有多个不同优先级的上下文
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	CreateHUDWidget();
	// 3. (推荐) 设置输入模式为只处理游戏输入
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
		//TODO: 需要一个当前类的LOG日志
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
