
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/WidgetControllerInterface.h"

#include "RL_UW_BloodSlot.generated.h"

/**
 * 
 */
 class UAttributeWidgetController;
 struct FPlayerState_AttributeData;
 class UProgressBar;
 class UImage;
 class UTextBlock;
 class UTexture2D;
 class UDataTable;
UCLASS()
class ONLINE_MODE_API URL_UW_BloodSlot : public UUserWidget,public IWidgetControllerInterface
{
	GENERATED_BODY()
public:
	virtual void SetWidgetController_Implementation(UWidgetController* InWidgetController) override;

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeWidgetController> AttributeWidgetController;

	void OnHealthValueChanged(int32 PlayerId, float NewValue);

	void OnMaxHealthValueChanged(int32 PlayerId, float NewValue);

	void UpdateBloodPercentage();

	void SetPlayerIcon(UTexture2D* Texture);

	void InitializeForPlayer(FPlayerState_AttributeData AttributeData);

	void UpdateAvatar(FName InAvatarID);

	void RequestAvatarLoad(FName InAvatarID, UDataTable* InHeroDataTable);
private:

	UPROPERTY(VisibleAnywhere, Category = "RL")
	float BloodPercentage;

	UPROPERTY(VisibleAnywhere, Category = "RL")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "RL")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "RL",meta = (BindWidget))
	TObjectPtr<UProgressBar>BloodProgressBar;

	UPROPERTY(VisibleAnywhere, Category = "RL", meta = (BindWidget))
	TObjectPtr<UImage>PlayerIcon;

	UPROPERTY(EditAnywhere, Category = "RL")
	FVector2D PlayerIconSize{32.f,32.f};

	UPROPERTY(VisibleAnywhere, Category = "RL", meta = (BindWidget))
	TObjectPtr<UTextBlock>PlayerNameBlock;

	UPROPERTY(VisibleAnywhere, Category = "RL", meta = (BindWidget))
	TObjectPtr<UTextBlock>HealthAndMaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "RL")
	int32 LocalPlayerID;

	void CalculateBloodPercentage();

	UPROPERTY(EditAnywhere, Category = "RL")
	TObjectPtr<UDataTable> AvatarDataTable;

	//TODO:玩家名称和英雄名称区分
	//UPROPERTY(EditAnywhere, Category = "RL")
	//FName PlayerName;

	UPROPERTY(EditAnywhere, Category = "RL")
	TObjectPtr<UTexture2D>DefaultPlayerIcon;
};
