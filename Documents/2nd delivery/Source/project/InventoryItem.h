// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "InventoryItem.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	FORCEINLINE FInventoryItem();

	explicit FORCEINLINE FInventoryItem(int32 InAmount, FString InType);

	explicit FORCEINLINE FInventoryItem(int32 InAmount, FString InType, UTexture2D* InImage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image = nullptr;
};

FORCEINLINE FInventoryItem::FInventoryItem()
{
}

FORCEINLINE FInventoryItem::FInventoryItem(const int32 InAmount, FString InType): Amount(InAmount), Type(InType), Image(nullptr)
{
}

FORCEINLINE FInventoryItem::FInventoryItem(const int32 InAmount, FString InType, UTexture2D* InImage) : Amount(InAmount), Type(InType), Image(InImage)
{
}