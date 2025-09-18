// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RL_Actor_Bullet.h"
#include <Components/SphereComponent.h>
#include "ASC/RL_ASC_Base.h"
#include "AbilitySystemInterface.h"

// Sets default values
ARL_Actor_Bullet::ARL_Actor_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ARL_Actor_Bullet::OnHit);
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

void ARL_Actor_Bullet::OnHit(
UPrimitiveComponent* HitComp,
AActor* OtherActor, 
UPrimitiveComponent* OtherComp, 
FVector NormalImpulse, 
const FHitResult& Hit)
{
	if(!OtherActor) 
	{	
		return;
	}
	const IAbilitySystemInterface* LocalInterface = Cast<IAbilitySystemInterface>(OtherActor) ? Cast<IAbilitySystemInterface>(OtherActor) : nullptr;
	if(!LocalInterface) 
	{
		Destroy();
		return;
	}
	/*TODO : AI µÄ ASC ×é¼þ*/
	URL_ASC_Base* TargetASC = Cast<URL_ASC_Base>(LocalInterface->GetAbilitySystemComponent());

	if (TargetASC && DamageEffectSpecHandle.IsValid())
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
	}

	Destroy();
}


