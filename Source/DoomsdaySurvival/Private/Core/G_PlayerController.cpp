// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/G_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/G_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AG_PlayerController::AG_PlayerController()
{
	// 打开网络复制
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;   // 启用 Tick
	
}

void AG_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	if (bIsSlowingDown)
	{
		ReduceSpeedSlowly(DeltaTime);   // 传入真实帧时间
	}
}

void AG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(IMC);
	
	// 获取增强输入本地玩家子系统，用于管理增强输入映射和绑定 
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		// 添加映射上下文到本地玩家子系统
		Subsystem->AddMappingContext(IMC,0);
	}
	
}

void AG_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// 将 InputComponent 强制转换为增强输入组件类型
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AG_PlayerController::MoveInput);
	EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&AG_PlayerController::LookInput);
	EnhancedInputComponent->BindAction(ZoomAction,ETriggerEvent::Triggered,this,&AG_PlayerController::ZoomInput);
	EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered,this,&AG_PlayerController::JumpInput);
	EnhancedInputComponent->BindAction(RunAction,ETriggerEvent::Triggered,this,&AG_PlayerController::StartRunInput);
	EnhancedInputComponent->BindAction(RunAction,ETriggerEvent::Completed,this,&AG_PlayerController::StopRunInput);
	EnhancedInputComponent->BindAction(WalkAction,ETriggerEvent::Triggered,this,&AG_PlayerController::StartWalkInput);
	EnhancedInputComponent->BindAction(WalkAction,ETriggerEvent::Completed,this,&AG_PlayerController::StopWalkInput);
	EnhancedInputComponent->BindAction(EquipAction,ETriggerEvent::Started,this,&AG_PlayerController::EquipInput);
	EnhancedInputComponent->BindAction(InteractAction,ETriggerEvent::Started,this,&AG_PlayerController::StartInteractInput);
	EnhancedInputComponent->BindAction(InteractAction,ETriggerEvent::Completed,this,&AG_PlayerController::StopInteractInput);
	
	
}

void AG_PlayerController::MoveInput(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// 获取控制旋转并提取偏航角，忽略俯仰和翻滚
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);
	
	// 根据偏航角计算前方向和右方向单位向量
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 将输入轴值转换为世界空间移动输入应用到被控制的Pawn
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AG_PlayerController::LookInput(const FInputActionValue& InputActionValue)
{
	// 视角旋转
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AG_PlayerController::ZoomInput(const FInputActionValue& InputActionValue)
{
	// 鼠标滚轮缩放弹簧臂长度
	const float ZoomValue = InputActionValue.Get<float>();
	
	if (AG_Character* G_Character = Cast<AG_Character>(GetCharacter()))
	{
		if (USpringArmComponent* SpringArm = G_Character->FindComponentByClass<USpringArmComponent>())
		{
			SpringArm->TargetArmLength += ZoomValue * 100.f;
			SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, 200.f, 1000.f);
		}
	}
}

void AG_PlayerController::JumpInput()
{
	// 实现跳跃逻辑：
	if (AG_Character* G_Character = Cast<AG_Character>(GetCharacter()))
	{
		G_Character->Jump();
	}
	
	
}

void AG_PlayerController::StartRunInput()
{
	if (AG_Character* G_Character = Cast<AG_Character>(GetCharacter()))
	{
		CurrentMovementMode = EMovementState::Run;
		bIsSlowingDown = false;// 取消任何减速
		G_Character->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void AG_PlayerController::StopRunInput()
{
	if (AG_Character* G_Character = Cast<AG_Character>(GetCharacter()))
	{
		// 如果当前是跑步模式，才切换到普通行走
		if (CurrentMovementMode == EMovementState::Run)
		{
			CurrentMovementMode = EMovementState::Walk;
			bIsSlowingDown = true;
			ReduceTargetSpeed = NormalSpeed;
		}
	}
}

void AG_PlayerController::StartWalkInput()  // 假设慢走按键按下
{
	if (AG_Character* G_Character = Cast<AG_Character>(GetCharacter()))
	{
		CurrentMovementMode = EMovementState::SlowWalk;
		bIsSlowingDown = true; // 开始减速至慢走速度
		ReduceTargetSpeed = WalkSpeed;
	}
}

void AG_PlayerController::StopWalkInput()// 慢走按键松开
{
	if (AG_Character* G_Character = Cast<AG_Character>(GetCharacter()))
	{
		// 只有当前是慢走模式才恢复到普通行走
		if (CurrentMovementMode == EMovementState::SlowWalk)
		{
			CurrentMovementMode = EMovementState::Walk;
			bIsSlowingDown = true;
			ReduceTargetSpeed = NormalSpeed;
		}
	}
}

void AG_PlayerController::ReduceSpeedSlowly(float DeltaTime)
{
	if (AG_Character* G_Character = Cast<AG_Character>(GetCharacter()))
	{
		float CurrentSpeed = G_Character->GetCharacterMovement()->MaxWalkSpeed;
		// 如果已经非常接近目标，直接设定精确值并停止减速
		if (FMath::IsNearlyEqual(CurrentSpeed, ReduceTargetSpeed, 25.f))
		{
			G_Character->GetCharacterMovement()->MaxWalkSpeed = ReduceTargetSpeed;
			bIsSlowingDown = false;
			return;
		}
		float NewSpeed = FMath::FInterpTo(CurrentSpeed, ReduceTargetSpeed, DeltaTime, 10.0f);
		G_Character->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}

void AG_PlayerController::EquipInput()// 按【E】触发
{
	if (AG_Character* G_Character = Cast<AG_Character>(GetCharacter()))
	{
		if (G_Character->bIsEquipped)
		{
			// 播放取消装备动画蒙太奇
			G_Character->PlayUnEquipMontage();
			UE_LOG(LogTemp, Warning, TEXT("UnEquipping Player Controller"));
			
			G_Character->bIsEquipped = false;
		}
		else
		{
			// 播放装备动画
			G_Character->PlayEquipMontage();
			UE_LOG(LogTemp, Warning, TEXT("Equipping Player Controller"));
			
			G_Character->bIsEquipped = true;
		}
	}
}

void AG_PlayerController::StartInteractInput()
{
	if (AG_Character* G_Character = Cast<AG_Character>(GetCharacter()))
	{
		
	}
}

void AG_PlayerController::StopInteractInput()
{
	if (AG_Character* G_Character = Cast<AG_Character>(GetCharacter()))
	{
		
	}
}
