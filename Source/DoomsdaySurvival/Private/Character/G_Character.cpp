// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/G_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


AG_Character::AG_Character()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 允许角色自动旋转
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);// 旋转速度
	
	// 启用平面约束并将角色初始位置对齐到平面
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	// 禁用控制器旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

}


void AG_Character::PlayEquipMontage()
{
	// 播放装备动画蒙太奇
	PlayAnimMontage(EquipAnimMontage,1.0, FName("Equip"));
}

void AG_Character::PlayUnEquipMontage()
{
	// 播放收回武器动画蒙太奇
	PlayAnimMontage(UnEquipAnimMontage,1.0, FName("UnEquip"));
}