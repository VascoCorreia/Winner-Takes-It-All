// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItem.h"
#include "Inventory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory API")
	bool IsSlotEmpty(int32 index) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory API")
	FInventoryItem GetItemAtIndex(int32 index) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory API")
	TArray<FInventoryItem> GetItems();

	UFUNCTION(BlueprintCallable, Category = "Inventory API")
	void AddItem(FInventoryItem item);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	int32 NumberOfSlots;

	// FIXME: This should return a pointer in my honest opinion... but apparently that messes with it being used in Blueprints. 
	// After 2nd delivery - check why and maybe fix the architecture of how it's used in BP's
	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Type"), Category = "Parameters")
	TArray<FInventoryItem> Items;

	FString PrintItems();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
