// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "G_Item.generated.h"

class USkeletalMeshComponent;

// 创建一个结构体，用于保存物品信息
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase // 数据表格（Data Table）的行结构体必须严格继承自 FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "物品ID")
	FName ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "物品类型")
	FGameplayTag ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "物品名称")
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "物品描述")
	FText ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "物品重量")
	float ItemWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "使用时恢复数值")
	float ItemUseValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "物品最大堆叠数量")
	int32 ItemMaxStack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "物品UI图标")
	TSoftObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "物品价值")
	int32 ItemValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "是否可交互")
	bool bCanInteract;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "是否可拾取")
	bool bCanPickUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "是否可装备")
	bool bCanEquip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "是否可使用")
	bool bCanUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "是否可堆叠")
	bool bCanStack;
	
};

UCLASS()
class DOOMSDAYSURVIVAL_API AG_Item : public AActor
{
	GENERATED_BODY()
	
public:	
	AG_Item();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "物品骨骼网格体")
	USkeletalMeshComponent* ItemMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品数据")
	FName ItemID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "物品数据")
	FItemData ItemData;
	
};
