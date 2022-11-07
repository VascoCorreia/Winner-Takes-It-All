// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseLoot.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "projectCharacter.h"

// Sets default values
ABaseLoot::ABaseLoot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	SphereComponent = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("Sphere Trigger"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore); // ignore all collisions...
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // ... except overlapping with our player
	SphereComponent->SetupAttachment(MeshComponent);

	// mark as replicated on the server
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ABaseLoot::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseLoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseLoot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (HasAuthority())
	{
		AprojectCharacter* projectCharacter = dynamic_cast<AprojectCharacter*>(OtherActor);
		if (projectCharacter != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("LOOT: Loot collected!"));

			FInventoryItem item(Amount, Name, Image, Item);
			projectCharacter->AddItemToInventory(item);

			Destroy();
		}
	}
}



