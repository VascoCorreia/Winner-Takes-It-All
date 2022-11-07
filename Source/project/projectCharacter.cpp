// Copyright Epic Games, Inc. All Rights Reserved.
#pragma optimize("", off)

#include "projectCharacter.h"
#include "Inventory.h"
#include "projectGameMode.h"
#include "projectGameState.h"
#include "projectPlayerState.h"
#include "projectGameInstance.h"
#include "UI/MyUI_InventorySlotBase.h"
#include "UObject/ConstructorHelpers.h"

#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/UniformGridPanel.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include <Net/UnrealNetwork.h>
#include "Engine/DecalActor.h"


AprojectCharacter::AprojectCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character 

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Create Inventory component
	Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
	if (Inventory)
	{
		Inventory->SetIsReplicated(true); // enable replication
	}

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	currentHealth = 10.0f;
}

void AprojectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AprojectCharacter, currentHealth);
	DOREPLIFETIME(AprojectCharacter, currentStamina);
	DOREPLIFETIME(AprojectCharacter, EquippedWeapon);
}

void AprojectCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("PROJECT CHARACTER: Begin Play"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	gameState = Cast<AprojectGameState>(GetWorld()->GetGameState());
	playerState = Cast<AprojectPlayerState>(GetPlayerState());
	characterMesh = FindComponentByClass<USkeletalMeshComponent>();
	GameInstance = Cast<UprojectGameInstance>(UGameplayStatics::GetGameInstance(World));

	if (World->GetMapName().Contains("World"))
	{
		if (IsLocallyControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT("PROJECT CHARACTER: Begin Play | we are in World map, so go on with SetupProjectCharacterForLevel"));
			SetupProjectCharacterForLevel();
		}
	}

	//one for each client
	if (IsLocallyControlled() && staminaRangeDecalReference)
	{
		SpawnStaminaRangeDecal();
		staminaRangeDecalComponent = UGameplayStatics::GetActorOfClass(GetWorld(), staminaRangeDecalReference)->FindComponentByClass<UDecalComponent>();
	}

	characterMovementComponent = FindComponentByClass<UCharacterMovementComponent>();
}

void AprojectCharacter::SetupProjectCharacterForLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("PROJECT CHARACTER: SetupProjectCharacterForLevel | About to check if IsLocallyControlled: %i"), IsLocallyControlled());
	/* Create the user interface and add it to viewport */
	if (IsLocallyControlled() && UserInGameInterfaceWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PROJECT CHARACTER: SetupProjectCharacterForLevel | Starting UI"));
		UserInGameInterfaceWidget = CreateWidget<UUserWidget>(GetWorld(), UserInGameInterfaceWidgetClass);
		UserInGameInterfaceWidget->AddToViewport();

		InventorySlot0 = dynamic_cast<UMyUI_InventorySlotBase*>(UserInGameInterfaceWidget->GetWidgetFromName(TEXT("InventorySlot0")));
		InventorySlot1 = dynamic_cast<UMyUI_InventorySlotBase*>(UserInGameInterfaceWidget->GetWidgetFromName(TEXT("InventorySlot1")));
		InventorySlot2 = dynamic_cast<UMyUI_InventorySlotBase*>(UserInGameInterfaceWidget->GetWidgetFromName(TEXT("InventorySlot2")));
		InventorySlot3 = dynamic_cast<UMyUI_InventorySlotBase*>(UserInGameInterfaceWidget->GetWidgetFromName(TEXT("InventorySlot3")));

		UpdateInventorySlotsInUI();

		UE_LOG(LogTemp, Warning,
			TEXT("CHARACTER: BEGIN_PLAY: Created UI_Game widget for player: %s."),
			*GetFName().ToString());
	}

	/* Bind delegates from inventory */
	if (InventorySlot0 != nullptr)
	{
		InventorySlot0->OnInventorySlotClicked.BindUObject(this, &AprojectCharacter::HandleInventorySlotClicked);
	}
	if (InventorySlot1 != nullptr)
	{
		InventorySlot1->OnInventorySlotClicked.BindUObject(this, &AprojectCharacter::HandleInventorySlotClicked);
	}
	if (InventorySlot2 != nullptr)
	{
		InventorySlot2->OnInventorySlotClicked.BindUObject(this, &AprojectCharacter::HandleInventorySlotClicked);
	}
	if (InventorySlot3 != nullptr)
	{
		InventorySlot3->OnInventorySlotClicked.BindUObject(this, &AprojectCharacter::HandleInventorySlotClicked);
	}

	// Equip default weapon
	// AWeapon is a replicated class, so we want do this setup only on the server - and it will be replicated
	if (!Inventory->IsSlotEmpty(0) && HasAuthority())
	{
		EquipWeaponAtIndex(0);
	}
}

void AprojectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	replicateRotation(GetActorRotation());

	UpdateStaminaRangeDecal();

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

	// TODO: desperate try...
	/*if (HasAuthority() && GetWorld()->GetMapName().Contains("World"))
	{
		AprojectGameState* GameState = dynamic_cast<AprojectGameState*>(GetWorld()->GetGameState());

		UE_LOG(LogTemp, Warning, TEXT("CHARACTER: Begin Play on Server | getting game state: %i"), GameState != nullptr);

		if (GameState && GameState->gamemodeState == EGameModeState::InLobby)
		{
			GameState->SetupGameStateForLevel();
		}
	}*/
	
	if (gameState)
	{
		//Lower player stamina as he moves. He can move for 5 seconds
		if (Cast<AprojectGameState>(gameState)->gamemodeState == EGameModeState::InMovementPhase && GetVelocity().Size() > 0)
		{
			currentStamina -= DeltaSeconds;

			if (currentStamina <= 0)
			{
				//stamina doesnt go under 0
				currentStamina = 0;
			}
		}
	}

	if (currentHealth <= 0)
	{
		PlayerDied();
	}
}

void AprojectCharacter::SetEquippedWeapon(AWeapon* Weapon)
{
	// Destroy previously equipped weapon
	if (EquippedWeapon != nullptr)
	{
		FString oldWeaponName = UEnum::GetDisplayValueAsText(EquippedWeapon->GetName()).ToString();
		UE_LOG(LogTemp, Warning, TEXT("CHARACTER: SET_EQUIPPED_WEAPON(): destroing old weapon: %s"), *oldWeaponName);

		EquippedWeapon->Destroy();
	}

	EquippedWeapon = Weapon;
	OnWeaponEquipped.ExecuteIfBound();

	FString weaponName = UEnum::GetDisplayValueAsText(EquippedWeapon->GetName()).ToString();
	UE_LOG(LogTemp, Warning, TEXT("CHARACTER: SET_EQUIPPED_WEAPON(): Equipped new weapon: %s"), *weaponName);

	if (IsLocallyControlled())
		SpawnWeaponRangeDecal(Weapon);

}

void AprojectCharacter::HandleInventorySlotClicked(int32 SlotIndex)
{
	UE_LOG(LogTemp, Warning,
		TEXT("CHARACTER: HANDLE_INVENTORY_SLOT_CLICK: equipping weapon from slot %i"),
		SlotIndex);

	if (!Inventory->IsSlotEmpty(SlotIndex))
	{
		EquipWeaponAtIndex(SlotIndex);
	}
}

// called on the client
void AprojectCharacter::AddItemToInventory_Implementation(FInventoryItem item)
{
	//UE_LOG(LogTemp, Warning, TEXT("CHARACTER: ADD_ITEM_TO_INVENTORY: Only called on CLIENT. Call Inventory->AddItem()"));
	Inventory->AddItem(item);

	//UE_LOG(LogTemp, Warning, TEXT("CHARACTER: ADD_ITEM_TO_INVENTORY: Call OnUpdateInventory on the client"));
	OnInventoryUpdate();
}

// called on the client
void AprojectCharacter::OnInventoryUpdate_Implementation()
{
	if (IsLocallyControlled())
	{
		// Functions that occur on the Client
		/*UE_LOG(LogTemp, Warning,
			TEXT("CHARACTER: ON_INVENTORY_UPDATE: I'm the CLIENT responding to adding items. My name: %s. My Inventory: %s."),
			*GetFName().ToString(), *Inventory->PrintItems());*/

		UpdateInventorySlotsInUI();
	}

	//Server-specific functionality
	//if (HasAuthority())
	//{
	//	// Functions taht occur on the Server
	//	UE_LOG(LogTemp, Warning, TEXT("CHARACTER: ON_INVENTORY_UPDATE: I'm the SERVER. %s now has %i items in inventory."), *GetFName().ToString(), Inventory->GetItems().Num());
	//}

	//Functions that occur on all machines. 
	/*
		Any special functionality that should occur as a result of updating Inventory should be placed here.
	*/
}

void AprojectCharacter::UpdateInventorySlotsInUI()
{
	InventorySlot0->UpdateSlot(Inventory->GetItemAtIndex(0), 0, Inventory);
	InventorySlot1->UpdateSlot(Inventory->GetItemAtIndex(1), 1, Inventory);
	InventorySlot2->UpdateSlot(Inventory->GetItemAtIndex(2), 2, Inventory);
	InventorySlot3->UpdateSlot(Inventory->GetItemAtIndex(3), 3, Inventory);
}

// called on the client
void AprojectCharacter::EquipWeaponAtIndex_Implementation(int32 InventoryItemIndex)
{
	FInventoryItem WeaponToEquip = Inventory->GetItemAtIndex(InventoryItemIndex);
	//UE_LOG(LogTemp, Warning, TEXT("CHARACTER: EQUIP_WEAPON_AT_INDEX(): printing inventory: %s"), *Inventory->PrintItems());
	if (WeaponToEquip.Item != nullptr)
	{
		const FVector SpawnLocation = GetRootComponent()->GetRelativeLocation();
		const FRotator SpawnRotation = GetRootComponent()->GetRelativeRotation();
		FActorSpawnParameters SpawnParams;
		AWeapon* SpawnedWeapon = Cast<AWeapon>(GetWorld()->SpawnActor(
			WeaponToEquip.Item,
			&SpawnLocation,
			&SpawnRotation,
			SpawnParams));

		if (SpawnedWeapon)
		{
			FString weaponName = UEnum::GetDisplayValueAsText(WeaponToEquip.Name).ToString();
			UE_LOG(LogTemp, Warning, TEXT("CHARACTER: EQUIP_WEAPON_AT_INDEX(): Spawned %s"), *weaponName);

			SpawnedWeapon->Equip(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CHARACTER: EQUIP_WEAPON_AT_INDEX(): Could not equip weapon. Spawning failed."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CHARACTER: EQUIP_WEAPON_AT_INDEX(): Could not equip default weapon. No Item set for Iventory slot %i"), InventoryItemIndex);
	}
}

//Bandage fix for rotation replication
void AprojectCharacter::replicateRotation_Implementation(FRotator rotation)
{
	SetActorRotation(rotation);
}

void AprojectCharacter::PlayerDied_Implementation()
{
	characterMesh->SetSimulatePhysics(true);
	characterMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	characterMesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

	FindComponentByClass<USkeletalMeshComponent>()->bPauseAnims = true;

	if (characterMovementComponent)
		characterMovementComponent->DisableMovement();
}


//Spawns a range indicator decal depending on the weapon the player has equipped
//call inside setequippedweapon()
void AprojectCharacter::SpawnWeaponRangeDecal(AWeapon* equippedWeapon)
{

	//desttoys previous weapon range decal.
	ADecalActor* previousDecal = Cast<ADecalActor>(UGameplayStatics::GetActorOfClass(GetWorld(), weaponRangeDecalReference));

	if (previousDecal)
	{
		previousDecal->Destroy();
	}

	if (weaponRangeDecalReference)
	{
		UWorld* world = GetWorld();

		if (world)
		{
			FActorSpawnParameters spawnParameters;

			spawnParameters.Owner = this;

			const FRotator rotation;
			const FVector location = GetActorLocation();

			world->SpawnActor<ADecalActor>(weaponRangeDecalReference, location, rotation, spawnParameters);

			ADecalActor* currentDecal = Cast<ADecalActor>(UGameplayStatics::GetActorOfClass(GetWorld(), weaponRangeDecalReference));

			if (currentDecal)
			{

				currentDecal->FindComponentByClass<UDecalComponent>()->DecalSize = FVector(300.f, equippedWeapon->GetRange(), equippedWeapon->GetRange());

			}
		}
	}
}

void AprojectCharacter::SpawnStaminaRangeDecal()
{
	if (staminaRangeDecalReference)
	{
		UWorld* world = GetWorld();

		if (world)
		{
			FActorSpawnParameters spawnParameters;

			spawnParameters.Owner = this;

			const FRotator rotation;
			const FVector location = GetActorLocation();

			world->SpawnActor<ADecalActor>(staminaRangeDecalReference, location, rotation, spawnParameters);

		}
	}
}

void AprojectCharacter::UpdateStaminaRangeDecal()
{
	//To get the maximum possible distance depending on the current stamina we use
	//distance = velocity * time

	/*GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Orange, FString::Printf(TEXT("%f"), staminaRangeDecalComponent->DecalSize.Y));*/

	if (staminaRangeDecalComponent && characterMovementComponent)
	{
		staminaRangeDecalComponent->DecalSize.Y = characterMovementComponent->MaxWalkSpeed * currentStamina;
		staminaRangeDecalComponent->DecalSize.Z = characterMovementComponent->MaxWalkSpeed * currentStamina;
	}
}
