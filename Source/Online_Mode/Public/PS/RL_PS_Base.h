#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "PlayerState_AttributeResult.h"
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

    UPROPERTY(VisibleAnywhere, Category = "RL_Gas")
    FPlayerState_AttributeData WidgetControllerData;

    FName GetPlayerName() const { return PlayerName; }

    bool IsLeader() const { return bIsLeader; }

    FName GetAvatarID() const { return AvatarID; }
private:

    UPROPERTY(VisibleAnywhere, Category = "RL_Gas")
    TObjectPtr<URL_ASC_Base> RL_ASC;

    UPROPERTY(VisibleAnywhere, Category = "RL_Gas")
    TObjectPtr<URL_AS_Player> RL_AS;

    UPROPERTY(EditAnywhere, Category = "RL_Gas")
    TArray<TSubclassOf<class URL_GA_Base>>RL_Default_GA {};

    UPROPERTY(EditAnywhere, Category = "RL_Gas")
    TSubclassOf<URL_GE_Base>RL_GE_Class;

    //在大厅进入游戏前需要初始化玩家信息,比如设置队长,设置名字,获取PlayerID(独一无二的玩家ID用于判断UI更新)

    UPROPERTY(EditDefaultsOnly, Category = "RL")
    FName PlayerName = {};

    UPROPERTY(EditDefaultsOnly, Category = "RL")
    TObjectPtr<UTexture2D> PlayerAvatar;

    UPROPERTY(EditDefaultsOnly, Category = "RL")
    bool bIsLeader;

    //void InitializeAbilitySystem();

    UPROPERTY(ReplicatedUsing = OnRep_AvatarID) // 使用OnRep通知UI更新
    FName AvatarID;

    void OnRep_AvatarID();
};
// Called to grant initial abilities and effects
   //这是PlayerCharacter里的函数,如果需要在PlayerCharacter里搭载这个组件ASC,就要重写这两个函数
   //virtual void PossessedBy(AController* NewController) override;

   //virtual void OnRep_PlayerState() override;