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

protected:
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComp;
	
	/* --------------------------------------------动画蒙太奇------------------------------------------------ */
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipAnimMontage;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* UnEquipAnimMontage;
	
	/* --------------------------------------------声效------------------------------------------------ */
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* EquipSoundComp;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* UnEquipSoundComp;
};