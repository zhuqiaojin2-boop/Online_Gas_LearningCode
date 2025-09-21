// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RL_Actor_Bullet.h"
#include <Components/SphereComponent.h>
#include "ASC/RL_ASC_Base.h"
#include "AbilitySystemInterface.h"
#include <AbilitySystemBlueprintLibrary.h>

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
    if (!HasAuthority()) // 只在服务器上处理命中逻辑
    {
        // 客户端的子弹碰到东西后直接销毁即可
        Destroy();
        return;
    }

    // 获取命中目标的ASC
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
    if (!TargetASC)
    {
        Destroy();
        return;
    }

    // **关键步骤**: 创建一个 GameplayEventData "包裹"，用来通知技能
    FGameplayEventData Payload;
    Payload.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Projectile.Hit")); // 定义一个“子弹命中”事件Tag
    Payload.Instigator = GetInstigator(); // 攻击者
    Payload.Target = OtherActor; // 命中的目标

    // 将精确的命中结果打包
    FGameplayAbilityTargetDataHandle TargetDataHandle;
    FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
    TargetDataHandle.Add(HitData);
    Payload.TargetData = TargetDataHandle;

    // **通知源ASC (开火者的ASC)**
    // GetInstigator() 返回的是开火的角色(Character)
    UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());
    if (SourceASC)
    {
        // 向开火的技能发送命中事件！
        SourceASC->HandleGameplayEvent(Payload.EventTag, &Payload);
    }

    // 命中后销毁子弹
    Destroy();
}


