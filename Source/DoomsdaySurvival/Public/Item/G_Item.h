// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "G_Item.generated.h"

class USkeletalMeshComponent;

// 创建一个结构体，用于保存物品信息
USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品ID")
	FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品名称")
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品最大堆叠数量")
	int32 ItemMaxStack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品UI图标")
	UTexture2D* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "是否可拾取")
	bool bCanPickUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "是否可装备")
	bool bCanEquip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "是否可使用")
	bool bCanUse;
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Mesh")
	USkeletalMeshComponent* ItemMesh;
	

};
