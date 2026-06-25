// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "G_Character.generated.h"

class USoundCue;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class DOOMSDAYSURVIVAL_API AG_Character : public ACharacter
{
	GENERATED_BODY()

public:
	AG_Character();
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsEquipped = false;

	UFUNCTION(BlueprintCallable, Category = "Animation|Montage")
	void PlayEquipMontage();
	
	UFUNCTION(BlueprintCallable, Category = "Animation|Montage")
	void PlayUnEquipMontage();
	
	UFUNCTION(BlueprintCallable, Category = "Animation|Sound")
	void PlayEquipSoundCue();

	UFUNCTION(BlueprintCallable, Category = "Animation|Sound")
	void PlayUnEquipSoundCue();
	
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComp;
	
	/* --------------------------------------------动画蒙太奇------------------------------------------------ */
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Montage")
	UAnimMontage* EquipAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Montage")
	UAnimMontage* UnEquipAnimMontage;
	
	/* --------------------------------------------动画声效------------------------------------------------ */
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Sound")
	USoundCue* EquipSoundCue;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Sound")
	USoundCue* UnEquipSoundCue;
};