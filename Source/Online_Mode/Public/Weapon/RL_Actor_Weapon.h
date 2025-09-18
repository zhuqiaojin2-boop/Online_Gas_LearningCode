// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RL_Actor_Weapon.generated.h"

class USkeletalMeshComponent;
UCLASS()
class ONLINE_MODE_API ARL_Actor_Weapon : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ARL_Actor_Weapon();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "RL_Weapon")
	TObjectPtr<USkeletalMeshComponent>WeaponMesh;

};
