// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "CameraManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_API UCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCameraManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
		int32 screenSizeX;

	UPROPERTY()
		int32 screenSizeY;

	UPROPERTY()
		int32 margin = 10;

	UPROPERTY(EditAnywhere)
		int32 cameraSpeed;

	UPROPERTY(EditAnywhere)
		int32 defaultCameraHeight = 600;

	UPROPERTY(EditAnywhere)
		int32 maxCameraHeight = 1000;

	UPROPERTY(EditAnywhere)
		int32 maxCameraPanning;

	UPROPERTY()
		FTransform cameraInitialPosition;

	UPROPERTY()
		APlayerController* PC = nullptr;

	UPROPERTY()
		UCameraComponent* camera = nullptr;

	UFUNCTION()
		FVector GetCameraPanDirection();

	UFUNCTION()
		void CameraSnapToPlayer();

	void ChangeCameraDistanceWithHeight(float desiredHeight);

	void objectsBetweenPlayerAndCamera();
};
