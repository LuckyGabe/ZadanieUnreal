
#include "Customer.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "NPC.h"
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
}

void ACustomer::MoveForward(float scale)
{
	AddMovementInput(GetActorForwardVector(), scale); // Move the character forward if the scale is 1 and backwards if the scale is -1


}
void ACustomer::MoveRight(float scale)
{

	AddMovementInput(GetActorRightVector(), scale); //Move actor right if the scale is 1 and left if it is -1

}
void ACustomer::Turn(float scale)
{
	AddControllerYawInput(scale * TurnRate * GetWorld()->GetDeltaSeconds());
}
void ACustomer::LookUp(float scale)
{
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
	//	Params.AddIgnoredActor(GetOwner());


	return GetWorld()->LineTraceSingleByChannel(OutHit, Location, EndPoint, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void ACustomer::Interact()
{
	//store ray trace results
	FHitResult HitResult;
	FVector PlayerLocation = GetActorLocation();
	//To avoid crashing the engine, or executing the code on wrong object return early
	if (!RayTrace(HitResult)) { return; }
	if (!HitResult.GetActor()) return;

	//As the "RayTrace" function is returning true only if object is set on "ECollisionChannel" and only the sellers will be there,
	//we do not need to compare tag
	ANPC* NPCSeller = Cast<ANPC>(HitResult.GetActor());
	if (NPCSeller)
	{
		if (FVector::Dist(PlayerLocation, NPCSeller->GetActorLocation()) > DistanceToBuy)
		{
			targetPosition = NPCSeller->GetActorLocation() - 150 * NPCSeller->GetActorForwardVector();
			bForceMove = true;

		}
		else
			NPCSeller->InteractWithCustomer();

	}

}

void ACustomer::MoveCustomer(FVector Position, float DeltaTime)
{
	DistanceToSeller = FVector::Dist(GetActorLocation(), targetPosition);
	if (DistanceToSeller <= 100.f) { bForceMove = false; }
	FVector NewPosition = FMath::VInterpConstantTo(GetActorLocation(), Position, DeltaTime, InterpSpeed);
	SetActorLocation(NewPosition);

}

