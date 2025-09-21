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
    if (!HasAuthority()) // ֻ�ڷ������ϴ��������߼�
    {
        // �ͻ��˵��ӵ�����������ֱ�����ټ���
        Destroy();
        return;
    }

    // ��ȡ����Ŀ���ASC
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
    if (!TargetASC)
    {
        Destroy();
        return;
    }

    // **�ؼ�����**: ����һ�� GameplayEventData "����"������֪ͨ����
    FGameplayEventData Payload;
    Payload.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Projectile.Hit")); // ����һ�����ӵ����С��¼�Tag
    Payload.Instigator = GetInstigator(); // ������
    Payload.Target = OtherActor; // ���е�Ŀ��

    // ����ȷ�����н�����
    FGameplayAbilityTargetDataHandle TargetDataHandle;
    FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
    TargetDataHandle.Add(HitData);
    Payload.TargetData = TargetDataHandle;

    // **֪ͨԴASC (�����ߵ�ASC)**
    // GetInstigator() ���ص��ǿ���Ľ�ɫ(Character)
    UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());
    if (SourceASC)
    {
        // �򿪻�ļ��ܷ��������¼���
        SourceASC->HandleGameplayEvent(Payload.EventTag, &Payload);
    }

    // ���к������ӵ�
    Destroy();
}


