// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/G_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Core/G_PlayerState.h"
#include "Core/G_HUD.h"
#include "Core/G_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "AbilitySystemComponent.h"
#include "GAS/AS/AS_Player.h"          // 你的 AttributeSet 头文件
#include "GameplayEffect.h"
#include "Engine/Engine.h"

AG_Character::AG_Character()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// 初始化指针
	AbilitySystemComponent = nullptr;
	AttributeSet = nullptr;
}

// PossessedBy：仅在服务器调用（Pawn 被 Controller 接管）
void AG_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// 1. 设置 Owner/Avatar
	InitAbilityActorInfo();

	// 2. 服务器端完整GAS初始化（属性、技能、初始效果）
	if (HasAuthority())
	{
		InitializeGAS();
	}
}

// OnRep_PlayerState：客户端收到 PlayerState 时调用
void AG_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// 客户端只需设置 Avatar 信息，不需要再创建属性或赋予技能（这些由服务器复制下来）
	InitAbilityActorInfo();
}

// 初始化能力系统组件的 Owner 和 Avatar
void AG_Character::InitAbilityActorInfo()
{
	// 获取 G_PlayerState
	AG_PlayerState* G_PlayerState = GetPlayerState<AG_PlayerState>();
	if (!G_PlayerState) return;

	// 获取 ASC
	UAbilitySystemComponent* ASC = G_PlayerState->GetAbilitySystemComponent();
	if (!ASC) return;
 
	// 关键：设置 Owner 和 Avatar
	ASC->InitAbilityActorInfo(G_PlayerState, this);

	// 缓存 ASC 引用，方便其他地方使用
	AbilitySystemComponent = ASC;
}

// 【服务器】端完成 GAS 的完整初始化
void AG_Character::InitializeGAS()
{
	if (!AbilitySystemComponent) return;

	// ----- 1. 创建 AttributeSet 并注册 -----
	if (!AttributeSet)
	{
		AttributeSet = NewObject<UAS_Player>(this);
		AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet);
	}

	// ----- 2. 赋予默认技能 -----
	for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (!AbilityClass) continue;
		
		// 1. 创建技能规格说明
		// 参数：技能类，等级（1级），输入ID（0表示无输入绑定），此技能的拥有者
		FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, this);
		
		// // 2. (可选) 为这个技能规格添加额外的游戏标签，用于逻辑判断
		// Spec.DynamicAbilityTags.AddTag(FGameplayTag::RequestGameplayTag("YourTag"));
		
		// 3. 赋予技能，可以用这个 Handle 来引用这个技能实例了
		FGameplayAbilitySpecHandle GASpecHandle = AbilitySystemComponent->GiveAbility(Spec);
		
		// // 4. 尝试激活技能，第二个参数 bAllowRemoteActivation 表示是否允许远程激活
		//  if (GASpecHandle.IsValid())
		//  {
		// 		AbilitySystemComponent->TryActivateAbility(GASpecHandle, false);
		//  }
		
	}

	// ----- 3. 应用初始效果（如满血、满蓝等） -----
	if (DefaultInitialEffect)
	{
		// 1. 创建 GameplayEffectContext
		FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
		
		// 2. 当属性（如生命值）发生变化时，系统知道“这是角色自己给自己加的初始状态“
		Context.AddSourceObject(this);
		
		// 3. 创建 GameplayEffectSpec
		FGameplayEffectSpecHandle GESpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultInitialEffect, 1, Context);
		
		// 4. 应用 GameplayEffectSpec
		if (GESpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());
		}
	}
}

void AG_Character::PlayEquipMontage()
{
	PlayAnimMontage(EquipAnimMontage, 1.0, FName("Equip"));
}

void AG_Character::PlayUnEquipMontage()
{
	PlayAnimMontage(UnEquipAnimMontage, 1.0, FName("UnEquip"));
}

void AG_Character::PlayEquipSoundCue()
{
	UGameplayStatics::PlaySoundAtLocation(this, EquipSoundCue, GetActorLocation(), GetActorRotation());
}

void AG_Character::PlayUnEquipSoundCue()
{
	UGameplayStatics::PlaySoundAtLocation(this, UnEquipSoundCue, GetActorLocation(), GetActorRotation());
}