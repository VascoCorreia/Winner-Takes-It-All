// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeMaterialComponent.h"
#include "Kismet/GameplayStatics.h"
#include "projectCharacter.h"
#include "CameraManager.h"

// Sets default values for this component's properties
UChangeMaterialComponent::UChangeMaterialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChangeMaterialComponent::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();

}


// Called every frame
void UChangeMaterialComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Stops exectuing once we found a player. Needs to be on Tick and not on BeginPlay because players 
	//are spawned after Environment
	if (player == nullptr)
	{
		player = UGameplayStatics::GetActorOfClass(GetWorld(), AprojectCharacter::StaticClass());
	}

	//Stops executing once we get cameraManagerComponent. Needs to be on Tick and not on BeginPlay because players 
	//are spawned after Environment
	if (player && (!playerCameraManager))
	{
		playerCameraManager = Cast<UCameraManager>(player->GetComponentByClass(UCameraManager::StaticClass()));

	}

	//Checks if player is distant to change the material back to normal
	if (player)
	{
		if ((FVector::Dist(owner->GetActorLocation(), player->GetActorLocation())) > 200.0f)
		{
			if (owner->ActorHasTag("Tree"))
				owner->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(1, TreeOnMaterial);

			else if (owner->ActorHasTag("AngelStatue"))
				owner->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, AngelStatueOnMaterial);

			else if (owner->ActorHasTag("HugeRock"))
				owner->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, HugeRockOnMaterial);

			else if (owner->ActorHasTag("StartWall"))
				owner->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, StartWallOnMaterial);
		}
	}
}

//Changes the material depeding on the type of actor that was Hit. This is done using actor tags.
void UChangeMaterialComponent::ChangeMaterialOnHit()
{
	if (owner->ActorHasTag("Tree"))
		owner->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(1, TreeOffMaterial);

	else if (owner->ActorHasTag("AngelStatue"))
		owner->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, AngelStatueOffMaterial);

	else if (owner->ActorHasTag("HugeRock"))
		owner->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, HugeRockOffMaterial);

	else if (owner->ActorHasTag("StartWall"))
		owner->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, StartWallOffMaterial);
}



