// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = rootComponent;
    StaticMesh->SetupAttachment(RootComponent);
    SkeletalMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANPC::InteractWithCustomer()
{

        for (const FShoppingListItem& Item : ItemsForSale)
        {
            // Print the available items to the console
            UE_LOG(LogTemp, Warning, TEXT("%s x%d"), *Item.ItemName, Item.Quantity);

            // Add your selling logic here or open a UI widget to display the items for sale
        }
    
}

float ANPC::DistanceToCustomer() 
{
    APawn* Customer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    float distance = FVector::Dist(GetActorLocation(), Customer->GetActorLocation());
    return distance;
}

