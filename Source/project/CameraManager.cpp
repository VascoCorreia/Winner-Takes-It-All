// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ChangeMaterialComponent.h"
#include "projectCharacter.h"



// Sets default values for this component's properties
UCameraManager::UCameraManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	cameraSpeed = 800;
	maxCameraPanning = 300;
	defaultCameraHeight = 900;
	maxCameraHeight = 1500;
	// ...
}


// Called when the game starts
void UCameraManager::BeginPlay()
{
	Super::BeginPlay();

	PC = GetWorld()->GetFirstPlayerController();

	camera = GetOwner()->FindComponentByClass<UCameraComponent>();

	if (camera)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No camera"));
	}

}


// Called every frame
void UCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PC->GetViewportSize(screenSizeX, screenSizeY);

	objectsBetweenPlayerAndCamera();

	camera->AddWorldOffset(GetCameraPanDirection() * cameraSpeed * DeltaTime);

	ChangeCameraDistanceWithHeight(GetOwner()->GetActorLocation().Z);
}

void UCameraManager::CameraSnapToPlayer()
{
	camera->SetRelativeTransform(FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector::ZeroVector));
}

FVector UCameraManager::GetCameraPanDirection()
{
	float mousePosX;
	float mousePosY;
	float cameraDirectionX = 0;
	float cameraDirectionY = 0;

	PC->GetMousePosition(mousePosX, mousePosY);

	//Condition to fix a bug where when the is the most top left position mouseX turn to screensizeX
	if (mousePosX == screenSizeX)
	{
		mousePosX = 0;
	}



	else if (camera->GetRelativeLocation().Y >= maxCameraPanning || camera->GetRelativeLocation().Y <= -maxCameraPanning)
	{
		cameraDirectionY = 0;
	}
	//down and right
	else if (mousePosY >= screenSizeY - margin && mousePosX >= screenSizeX - margin)
	{
		cameraDirectionX = -1;
		cameraDirectionY = 1;
	}

	//down and left
	else if (mousePosY >= screenSizeY - margin && mousePosX <= margin)
	{
		cameraDirectionX = -1;
		cameraDirectionY = -1;
	}

	//up and right
	else if (mousePosY <= margin && mousePosX >= screenSizeX - margin)
	{
		cameraDirectionX = 1;
		cameraDirectionY = 1;
	}

	//up and left
	if (mousePosY <= margin && mousePosX <= margin)
	{
		cameraDirectionX = 1;
		cameraDirectionY = -1;
	}

	//if mouse is on left side of viewport camera moves to the left (negative Y direction)
	else if (mousePosX <= margin)
	{
		cameraDirectionY = -1;
	}

	//if mouse is on right side of viewport camera moves to the right (positive Y direction)
	else if (mousePosX >= screenSizeX - margin)
	{
		cameraDirectionY = 1;
	}

	//if mouse is on top side of viewport camera moves up (positive X direction)
	else if (mousePosY <= margin)
	{
		cameraDirectionX = 1;
	}

	//if mouse is on down side of viewport camera moves down (negative X direction)
	else if (mousePosY >= screenSizeY - margin)
	{
		cameraDirectionX = -1;
	}

	//if mouse is out of viewport
	if (PC->GetMousePosition(mousePosX, mousePosY) == false) {
		cameraDirectionX = 0;
		cameraDirectionY = 0;
	}

	if (camera->GetRelativeLocation().X > maxCameraPanning)
	{
		camera->SetRelativeLocation(FVector(maxCameraPanning, camera->GetRelativeLocation().Y, camera->GetRelativeLocation().Z));

		if (cameraDirectionX > 0)
			cameraDirectionX = 0;
	}
	else if (camera->GetRelativeLocation().X < -maxCameraPanning)
	{
		camera->SetRelativeLocation(FVector(-maxCameraPanning, camera->GetRelativeLocation().Y, camera->GetRelativeLocation().Z));

		if (cameraDirectionX < 0)
			cameraDirectionX = 0;
	}

	return FVector(cameraDirectionX, cameraDirectionY, 0);
}

void UCameraManager::ChangeCameraDistanceWithHeight(float desiredHeight)
{
	if (desiredHeight < defaultCameraHeight)
	{
		GetOwner()->FindComponentByClass<USpringArmComponent>()->TargetArmLength = defaultCameraHeight;
	}
	else if (desiredHeight > maxCameraHeight)
	{
		GetOwner()->FindComponentByClass<USpringArmComponent>()->TargetArmLength = maxCameraHeight;
	}
	else
	{
		GetOwner()->FindComponentByClass<USpringArmComponent>()->TargetArmLength = desiredHeight;
	}
}

void UCameraManager::objectsBetweenPlayerAndCamera()
{
	const FVector start = camera->GetComponentLocation();
	const FVector end = GetOwner()->GetActorLocation();

	TArray<AActor*> actorsToIgnore;

	TArray<AActor*> allCharacters;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AprojectCharacter::StaticClass(), allCharacters);

	for (AActor* character : allCharacters)
	{
		actorsToIgnore.Add(character);
	}

	TArray<FHitResult> hitArray;

	//Line trace from the camera to the player. 
	bool Hit = UKismetSystemLibrary::LineTraceMulti
	(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		actorsToIgnore,
		EDrawDebugTrace::None,
		hitArray,
		true,
		FLinearColor::Gray,
		FLinearColor::Green,
		2.0f
	);

	//If theres a Hit change the materials of each hit actor
	if (Hit)
	{
		for (const FHitResult HitResult : hitArray)
		{
			HitResult.GetActor()->FindComponentByClass<UChangeMaterialComponent>()->ChangeMaterialOnHit();
		}
	}
}