// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManager.h"

// Sets default values for this component's properties
UCameraManager::UCameraManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

	camera->AddWorldOffset(GetCameraPanDirection() * cameraSpeed * DeltaTime);
}

void UCameraManager::CameraSnapToPlayer()
{
	camera->SetRelativeTransform(FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector::ZeroVector));
	UE_LOG(LogTemp, Warning, TEXT("Executed"));
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

	//Debug message
	//UE_LOG(LogTemp, Warning, TEXT("MouseX: %f | MouseY: %f | ScreenSizeX: %i | ScreenSizeY: %i"), mousePosX, mousePosY, screenSizeX, screenSizeY); 


	//down and right
	if (mousePosY >= screenSizeY - margin && mousePosX >= screenSizeX - margin)
	{
		cameraDirectionX = -1;
		cameraDirectionY = 1;
	}

	//down and left
	if (mousePosY >= screenSizeY - margin && mousePosX <= margin)
	{
		cameraDirectionX = -1;
		cameraDirectionY = -1;
	}

	//up and right
	if (mousePosY <= margin && mousePosX >= screenSizeX - margin)
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

	return FVector(cameraDirectionX, cameraDirectionY, 0);
}