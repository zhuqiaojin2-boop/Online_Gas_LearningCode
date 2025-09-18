// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbility.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "RL_Actor_Bullet.generated.h"

class USphereComponent;
UCLASS()
class ONLINE_MODE_API ARL_Actor_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARL_Actor_Bullet();

	void SetDamageEffectSpecHandle(FGameplayEffectSpecHandle NewDamageEffectSpecHandle) { DamageEffectSpecHandle = NewDamageEffectSpecHandle;}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UFUNCTION()
	void OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, 
	const FHitResult& Hit);
};
