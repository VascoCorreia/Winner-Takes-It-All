// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "projectCharacter.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	// mark as replicated on the server
	//SetReplicates(true);
	bReplicates = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Equip_Implementation(AprojectCharacter* Character)
{
	if (Character == nullptr) return;

	FString weaponName = UEnum::GetDisplayValueAsText(Name).ToString();
	UE_LOG(LogTemp, Warning, TEXT("WEAPON: EQUIP(): start equipping weapon %s"), *weaponName);

	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SkeletalMesh->SetSimulatePhysics(false);

	const USkeletalMeshSocket* RightHandSocket = Character->GetMesh()->GetSocketByName("RightHandSocket");

	if (RightHandSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("WEAPON: EQUIP(): RightHandSocket found, attaching weapon to character"));
		RightHandSocket->AttachActor(this, Character->GetMesh());
		this->SetOwner(Character);
		Character->SetEquippedWeapon(this);

	}

	if (OnEquipSound)
	{
		UGameplayStatics::PlaySound2D(this, OnEquipSound);
	}
}
