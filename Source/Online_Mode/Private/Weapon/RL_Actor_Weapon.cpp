// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_Actor_Weapon.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
ARL_Actor_Weapon::ARL_Actor_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.	
	PrimaryActorTick.bCanEverTick = true;
    
    //USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    // 2. 创建你的网格组件
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

    USceneComponent* AttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    // 3. 将网格组件附加到根上
    if(WeaponMesh)
        WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RootComponent = AttachPoint;

    WeaponMesh->SetupAttachment(AttachPoint, TEXT("WeaponSocket"));

    bReplicates = true;
}

