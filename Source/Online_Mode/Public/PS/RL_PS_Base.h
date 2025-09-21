#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "RL_PS_Base.generated.h"

 class URL_AS_Player;
 class URL_ASC_Base;
 class URL_GE_Base;
UCLASS()
class ONLINE_MODE_API ARL_PS_Base : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:    
    ARL_PS_Base();
    // 实现IAbilitySystemInterface接口
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    URL_AS_Player* GetAttributeSet()const;

    virtual void BeginPlay() override;

private:

    UPROPERTY(VisibleAnywhere, Category = "RL_Gas")
    TObjectPtr<URL_ASC_Base> RL_ASC;

    UPROPERTY(VisibleAnywhere, Category = "RL_Gas")
    TObjectPtr<URL_AS_Player> RL_AS;

    UPROPERTY(EditAnywhere, Category = "RL_Gas")
    TArray<TSubclassOf<class URL_GA_Base>>RL_Default_GA {};

    UPROPERTY(EditAnywhere, Category = "RL_Gas")
    TSubclassOf<URL_GE_Base>RL_GE_Class;

    //void InitializeAbilitySystem();

};
// Called to grant initial abilities and effects
   //这是PlayerCharacter里的函数,如果需要在PlayerCharacter里搭载这个组件ASC,就要重写这两个函数
   //virtual void PossessedBy(AController* NewController) override;

   //virtual void OnRep_PlayerState() override;