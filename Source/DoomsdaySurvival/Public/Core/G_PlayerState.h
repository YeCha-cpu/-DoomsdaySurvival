// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "G_PlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class DOOMSDAYSURVIVAL_API AG_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AG_PlayerState();
	
	// 获取能力系统组件的接口
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// 获取属性集的函数
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
protected:
	UPROPERTY(VisibleAnywhere)
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere)
	UAttributeSet* AttributeSet;
};

