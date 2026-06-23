// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/G_Item.h"
#include "Components/SkeletalMeshComponent.h"

AG_Item::AG_Item()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ItemMesh");
	ItemMesh->SetupAttachment(RootComponent);

}

void AG_Item::BeginPlay()
{
	Super::BeginPlay();
	
}

void AG_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

