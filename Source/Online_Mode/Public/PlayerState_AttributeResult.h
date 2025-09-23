// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerState_AttributeResult.generated.h"
/**
 * 
 */
class UGameplayEffect;
 USTRUCT()
 struct FPlayerState_AttributeData
 {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "RL")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "RL")
	float MaxHealth;

	//UPROPERTY(EditDefaultsOnly, Category = "RL")
	//float Mana;

	UPROPERTY(EditDefaultsOnly, Category = "RL")
	bool IsLeader;

	UPROPERTY(EditDefaultsOnly, Category = "RL")
	FName AvatarID;

	UPROPERTY(EditDefaultsOnly, Category = "RL")
	TObjectPtr<UDataTable> HeroDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "RL")
	FName PlayerName;

	UPROPERTY(EditDefaultsOnly, Category = "RL")
	int32 PlayerID;
 };

 USTRUCT()
 struct FPlayerState_AttributeResult
 {
	 GENERATED_BODY()
public:

	 UPROPERTY(EditDefaultsOnly, Category = "RL")
	 TArray<APlayerState*> PSDataResults;
 };

 USTRUCT()
	 struct FHeroData: public FTableRowBase
 {
	 GENERATED_BODY()
 public:

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
     TSubclassOf<APawn> HeroPawnClass;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
     TSubclassOf<UGameplayEffect> BaseAttributesEffect;

     /**
      * @brief 英雄在游戏内血条、计分板等处显示的头像。
      * **关键**: 使用 TSoftObjectPtr (软指针) 而不是 UTexture2D* (硬指针)。
      * 软指针只存储资产的路径，不会在游戏启动时就把所有英雄的头像都加载到内存里。
      * 这极大地节省了初始内存，并且允许我们后续进行异步加载，防止UI卡顿。
      */
     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
     TSoftObjectPtr<UTexture2D> InGameAvatar;

     /**
      * @brief 英雄在角色选择界面显示的立绘或大尺寸图标。
      * 同样使用软指针，因为选择界面的资产通常很大，更需要异步加载。
      */
     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
     TSoftObjectPtr<UTexture2D> SelectionScreenIcon;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
     FText HeroDisplayName;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI | Lore")
     FText HeroDescription;
 };
