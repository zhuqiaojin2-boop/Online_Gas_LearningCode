#pragma once

#include "CoreMinimal.h"
#include "AS/RL_AS_Base.h"
#include "RL_AS_Player.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAmmoChangedEvent,float,Ammo);


UCLASS()
class ONLINE_MODE_API URL_AS_Player : public URL_AS_Base
{
	GENERATED_BODY()
public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// GameplayEffect 执行后调用此函数，是处理伤害、治疗等元属性的最佳位置
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// 用于网络复制
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		
	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_MaxExperience)
	FGameplayAttributeData MaxExperience;
	ATTRIBUTE_ACCESSORS(URL_AS_Player, MaxExperience)

	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_Experience)
	FGameplayAttributeData Experience;
	ATTRIBUTE_ACCESSORS(URL_AS_Player, Experience)

	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_MaxAmmoCounts)
	FGameplayAttributeData MaxAmmoCounts;
	ATTRIBUTE_ACCESSORS(URL_AS_Player, MaxAmmoCounts)

	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_LeftAmmosInMagazine)
	FGameplayAttributeData LeftAmmosInMagazine;
	ATTRIBUTE_ACCESSORS(URL_AS_Player, LeftAmmosInMagazine)

	UPROPERTY(BlueprintReadOnly, Category = "RL_Attributes", ReplicatedUsing = OnRep_MaxAmmosInMagazine)
	FGameplayAttributeData MaxAmmosInMagazine;
	ATTRIBUTE_ACCESSORS(URL_AS_Player, MaxAmmosInMagazine)


	FAmmoChangedEvent OnAmmoChanged;
protected:

	UFUNCTION()
	virtual void OnRep_MaxExperience(const FGameplayAttributeData& OldMaxExperience);

	UFUNCTION()
	virtual void OnRep_Experience(const FGameplayAttributeData& OldExperience);

	UFUNCTION()
	virtual void OnRep_MaxAmmoCounts(const FGameplayAttributeData& OldMaxAmmoCounts);

	UFUNCTION()
	virtual void OnRep_LeftAmmosInMagazine(const FGameplayAttributeData& OldLeftAmmosInMagazine);

	UFUNCTION()
	virtual void OnRep_MaxAmmosInMagazine(const FGameplayAttributeData& OldMaxAmmosInMagazine);
};
