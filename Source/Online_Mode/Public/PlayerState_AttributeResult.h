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
      * @brief Ӣ������Ϸ��Ѫ�����Ʒְ�ȴ���ʾ��ͷ��
      * **�ؼ�**: ʹ�� TSoftObjectPtr (��ָ��) ������ UTexture2D* (Ӳָ��)��
      * ��ָ��ֻ�洢�ʲ���·������������Ϸ����ʱ�Ͱ�����Ӣ�۵�ͷ�񶼼��ص��ڴ��
      * �⼫��ؽ�ʡ�˳�ʼ�ڴ棬�����������Ǻ��������첽���أ���ֹUI���١�
      */
     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
     TSoftObjectPtr<UTexture2D> InGameAvatar;

     /**
      * @brief Ӣ���ڽ�ɫѡ�������ʾ��������ߴ�ͼ�ꡣ
      * ͬ��ʹ����ָ�룬��Ϊѡ�������ʲ�ͨ���ܴ󣬸���Ҫ�첽���ء�
      */
     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
     TSoftObjectPtr<UTexture2D> SelectionScreenIcon;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
     FText HeroDisplayName;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI | Lore")
     FText HeroDescription;
 };
