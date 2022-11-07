// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChangeMaterialComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_API UChangeMaterialComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UChangeMaterialComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void ChangeMaterialOnHit();

private:
	AActor* player = nullptr;
	AActor* owner = nullptr;

	class UCameraManager* playerCameraManager = nullptr;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* TreeOnMaterial;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* TreeOffMaterial;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* AngelStatueOnMaterial;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* AngelStatueOffMaterial;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* HugeRockOnMaterial;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* HugeRockOffMaterial;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* StartWallOnMaterial;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* StartWallOffMaterial;

};
