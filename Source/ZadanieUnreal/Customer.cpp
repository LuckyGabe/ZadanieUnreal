
#include "Customer.h"
#include "Components/CapsuleComponent.h"
#include "CustomerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "NPC.h"
#include "Misc/FileHelper.h"
#include "Misc/DateTime.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

// Sets default values
ACustomer::ACustomer()
{
	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent(); //set the capsule as a root component for the character

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent"); // create a camera
	CameraComponent->SetupAttachment(RootComponent); //attach camera to the root component
	CameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void ACustomer::BeginPlay()
{
	Super::BeginPlay();
	CustomerController = Cast<ACustomerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// Initialize the file path
	FString FileName = "PlayerActions.json";
	FString SaveDirectory = FPaths::ProjectSavedDir() / TEXT("SavedActions/");
	PlayerActionsFilePath = SaveDirectory + FileName;

	// Load existing player actions
	LoadPlayerActions();
}

// Called every frame
void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bForceMove)
	{
		MoveCustomer(targetPosition, DeltaTime);
	}
}

// Called to bind functionality to input
void ACustomer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ACustomer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACustomer::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACustomer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACustomer::LookUp);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACustomer::Interact);
	PlayerInputComponent->BindAction("Basket", EInputEvent::IE_Pressed, this, &ACustomer::ShowHideBasket);
	PlayerInputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &ACustomer::ShowHideInventory).bConsumeInput = false;
	PlayerInputComponent->BindAction("ShoppingList", EInputEvent::IE_Pressed, this, &ACustomer::ShowHideShoppingList);
}

void ACustomer::MoveForward(float scale)
{
	if (bLockActions) { return; }
	AddMovementInput(GetActorForwardVector(), scale); // Move the character forward if the scale is 1 and backwards if the scale is -1

}
void ACustomer::MoveRight(float scale)
{
	if (bLockActions) { return; }
	AddMovementInput(GetActorRightVector(), scale); //Move actor right if the scale is 1 and left if it is -1

}
void ACustomer::Turn(float scale)
{
	if (bLockActions) { return; }
	AddControllerYawInput(scale * TurnRate * GetWorld()->GetDeltaSeconds());
}
void ACustomer::LookUp(float scale)
{
	if (bLockActions) { return; }
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(scale * LookUpRate * GetWorld()->GetDeltaSeconds());
}
bool ACustomer::RayTrace(FHitResult& OutHit)
{
	FVector Location;										  //
	FRotator Rotation;										 // Get the location and rotation of player viewpoint
	GetController()->GetPlayerViewPoint(Location, Rotation);//

	const FVector EndPoint = Location + Rotation.Vector() * RayTraceRange; //the end point of the ray

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);



	return GetWorld()->LineTraceSingleByChannel(OutHit, Location, EndPoint, ECollisionChannel::ECC_GameTraceChannel1, Params);
}
void ACustomer::ShowHideBasket()
{
	if (!bLockActions)
	{
		CustomerController->ShowHideBasket();
	}
}

void ACustomer::Interact()
{
	if (bLockActions) { return; }

	// store ray trace results
	FHitResult HitResult;

	if (!RayTrace(HitResult)) { return; }

	ANPC* HitActor = Cast<ANPC>(HitResult.GetActor());
	if (!HitActor) return;

	InteractedNPC = HitActor;
	float Distance = FVector::Dist(GetActorLocation(), InteractedNPC->GetActorLocation());
	if (Distance > DistanceToBuy)
	{
		targetPosition = InteractedNPC->GetActorLocation() - TargetDistFromNPC * InteractedNPC->GetActorForwardVector();
		bForceMove = true;
		SavePlayerAction("Podejscie do sklepu");
	}
	else
	{
		InteractedNPC->InteractWithCustomer();
	}
}

//Automatically move the customer in front of the NPC
void ACustomer::MoveCustomer(FVector Position, float DeltaTime)
{
	if (FVector::Dist(GetActorLocation(), targetPosition) <= 100.f)
	{
		bForceMove = false;
		return;
	}

	FVector NewPosition = FMath::VInterpConstantTo(GetActorLocation(), Position, DeltaTime, InterpSpeed);
	SetActorLocation(NewPosition);
}

void ACustomer::ShowHideInventory()
{
	CustomerController->ShowHideItemInventory();
	bLockActions = !bLockActions;
}

void ACustomer::ShowHideShoppingList()
{
	if (CustomerController->IsInputKeyDown(EKeys::LeftControl))
	{
		CustomerController->ShowHideShoppingList();
	}
}

void ACustomer::SavePlayerAction(const FString& PlayerAction)
{
	// Get the current date and time
	FDateTime CurrentDateTime = FDateTime::Now();

	// Create a new JSON object for the current customer action
	TSharedPtr<FJsonObject> NewActionObject = MakeShareable(new FJsonObject());
	NewActionObject->SetStringField("player_action", PlayerAction);
	NewActionObject->SetStringField("timestamp", CurrentDateTime.ToString());

	// Add the new action object to the array
	PlayerActions.Add(MakeShareable(new FJsonValueObject(NewActionObject)));

	// Serialize the updated array to a string
	FString JsonOutput;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonOutput);
	FJsonSerializer::Serialize(PlayerActions, JsonWriter);

	// Save the serialized JSON array back to the file
	FFileHelper::SaveStringToFile(JsonOutput, *PlayerActionsFilePath);
}

void ACustomer::LoadPlayerActions()
{
	FString JsonInput;
	FFileHelper::LoadFileToString(JsonInput, *PlayerActionsFilePath);

	// Deserialize the JSON input to an array of JSON objects
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonInput);
	FJsonSerializer::Deserialize(JsonReader, PlayerActions);
}
