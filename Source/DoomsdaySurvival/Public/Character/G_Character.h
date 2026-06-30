// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "G_Character.generated.h"

class USoundCue;
class USpringArmComponent;
class UCameraComponent;
class UAbilitySystemComponent;
class UAS_Player;           // 你的 AttributeSet 类，记得包含对应的头文件
class UGameplayAbility;
class UGameplayEffect;

UCLASS()
class DOOMSDAYSURVIVAL_API AG_Character : public ACharacter
{
	GENERATED_BODY()

public:
	AG_Character();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
protected:
	// 初始化 GAS 的 Actor 信息（Owner/Avatar）
	virtual void InitAbilityActorInfo();
	
	// 初始化属性集、赋予技能、应用初始效果（仅在服务器执行）
	UFUNCTION(BlueprintCallable, Category = "GAS")
	void InitializeGAS();

public:
	// ========== GAS 相关 ==========
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;// 从 PlayerState 取，但可以缓存引用

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAS_Player* AttributeSet;// 缓存的属性集指针

	// 默认赋予的技能类列表（在蓝图中配置）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	// 初始属性效果（比如满血满蓝）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Effects")
	TSubclassOf<UGameplayEffect> DefaultInitialEffect;

	// ========== 原有成员 ==========
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
	
	/* ---------- 动画蒙太奇 ---------- */
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Montage")
	UAnimMontage* EquipAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Montage")
	UAnimMontage* UnEquipAnimMontage;
	
	/* ---------- 声效 ---------- */
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Sound")
	USoundCue* EquipSoundCue;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Sound")
	USoundCue* UnEquipSoundCue;
};