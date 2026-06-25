// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "G_PlayerController.generated.h"

class AG_Character; 

UENUM()
enum class EMovementState : uint8
{
	Walk,    // 普通行走
	Run,     // 跑步
	SlowWalk // 慢走
};

UCLASS()
class DOOMSDAYSURVIVAL_API AG_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AG_PlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "G|Movement")
	float RunSpeed = 900.0f;
	
	UPROPERTY(EditAnywhere, Category = "G|Movement")
	float WalkSpeed = 200.0f;
	
	UPROPERTY(EditAnywhere, Category = "G|Movement")
	float NormalSpeed = 600.f;;
	
	UPROPERTY(BlueprintReadWrite)
	EMovementState CurrentMovementMode = EMovementState::Walk;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsSlowingDown = false;
	
	UPROPERTY(BlueprintReadWrite)
	float ReduceTargetSpeed;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	/* ------------------------------IMC------------------------------ */
	UPROPERTY(EditAnywhere, Category = "G|Input")
	TObjectPtr<UInputMappingContext> IMC;
	
	/* ------------------------------IA------------------------------ */
	UPROPERTY(EditAnywhere, Category = "G|Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "G|Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, Category = "G|Input")
	TObjectPtr<UInputAction> ZoomAction;

	
	UPROPERTY(EditAnywhere, Category = "G|Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "G|Input")
	TObjectPtr<UInputAction>RunAction;
	
	UPROPERTY(EditAnywhere, Category = "G|Input")
	TObjectPtr<UInputAction>WalkAction;
	
	UPROPERTY(EditAnywhere, Category = "G|Input")
	TObjectPtr<UInputAction>EquipAction;
	
	UPROPERTY(EditAnywhere, Category = "G|Input")
	TObjectPtr<UInputAction>InteractAction;
	
	/* ------------------------------输入回调函数------------------------------ */
	void MoveInput(const FInputActionValue& InputActionValue);
	void LookInput(const FInputActionValue& InputActionValue);
	void ZoomInput(const FInputActionValue& InputActionValue);
	void JumpInput();
	void StartRunInput();
	void StopRunInput();
	void StartWalkInput();
	void StopWalkInput();
	
	void ReduceSpeedSlowly(float DeltaTime);
	
protected:
	UFUNCTION(BlueprintNativeEvent, DisplayName = "开始交互时(Input)")
	void StartInteractInput();
	UFUNCTION(BlueprintNativeEvent, DisplayName = "停止交互时(Input)")
	void StopInteractInput();
	UFUNCTION(BlueprintNativeEvent, DisplayName = "装备主武器(Input)")
	void EquipInput();
};
