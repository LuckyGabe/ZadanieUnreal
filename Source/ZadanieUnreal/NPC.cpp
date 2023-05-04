// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Kismet/GameplayStatics.h"
#include "ShoppingList.h"
#include "CustomerController.h"
#include "EngineUtils.h"
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

	// Create and initialize the ItemsAvailableIndicator component
	ItemsAvailableIndicator = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ItemsAvailableIndicator"));
	ItemsAvailableIndicator->SetupAttachment(RootComponent);
	ItemsAvailableIndicator->SetRelativeLocation(FVector(0, 0, 200)); // Adjust the Z value to position the text above the NPC
	ItemsAvailableIndicator->SetHorizontalAlignment(EHTA_Center);
	ItemsAvailableIndicator->SetText(FText::FromString("")); // Initially set the text to an empty string
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	CustomerController = Cast<ACustomerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); //get player controller
	StoreMaxNumberOfItems();
	GetWorldTimerManager().SetTimer(ResetItemsQuantityTimerHandle, this, &ANPC::ResetItemsQuantity, RestoreMaxNumbOfItemsTime, true);
	UpdateItemsAvailableIndicator();
	ItemsAvailableIndicator->SetVisibility(true);
	ItemsAvailableIndicator->SetWorldSize(100.0f); // Set the world size of the text
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANPC::InteractWithCustomer()
{
	CustomerController->ShowStoreUI(ItemsForSale, StoreWidgetClass);
}

float ANPC::DistanceToCustomer()
{
	APawn* Customer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	float distance = FVector::Dist(GetActorLocation(), Customer->GetActorLocation());
	return distance;
}


void ANPC::RemoveItemFromTheStore(FString ItemName)
{

	for (int i = 0; i < ItemsForSale.Num(); i++)
	{
		if (ItemsForSale[i].ItemName.Equals(ItemName) && ItemsForSale[i].Quantity > 0)
		{
			ItemsForSale[i].Quantity--;
			UpdateItemsAvailableIndicator();
			break;
		}
	}

}

void ANPC::StoreMaxNumberOfItems()
{
	// Clear the MaxNumberOfItems array before adding new elements
	MaxNumberOfItems.Empty();
	for (int i = 0; i < ItemsForSale.Num(); i++)
	{
		MaxNumberOfItems.Add(ItemsForSale[i].Quantity);
	}
}

void ANPC::ResetItemsQuantity()
{
	for (int i = 0; i < ItemsForSale.Num(); i++)
	{
		ItemsForSale[i].Quantity = MaxNumberOfItems[i];
	}
	CustomerController->PopulateStoreWidget(ItemsForSale);
	UpdateItemsAvailableIndicator();
}

#if WITH_EDITOR
void ANPC::AssignNPCReferenceToShoppingList()
{

	AShoppingList* ShoppingList = FindShoppingListClassInLevel();

	if (ShoppingList)
	{
		// Add this NPC instance to the NPCReferences array in the ShoppingList
		ShoppingList->PopulateShoppingListInEditor(this);
	}

}
#endif

//funkcja będzie wywoływana przy dodaniu i poruszaniu aktorem w poziomie
void ANPC::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	AssignNPCReferenceToShoppingList();
}

AShoppingList* ANPC::FindShoppingListClassInLevel()
{
	AShoppingList* FoundActor = nullptr;
	//Iteracja jak "UGameplayStatics::GetAllActorsOfClass" (zbiera wszystkich aktorów klasy z levelu)
	//z tą różnicą że po znalezieniu jednego aktora iteracja się konczy 
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* CurrentActor = *It;

		// Check if the current actor is of the custom Actor class or a derived class
		if (CurrentActor && CurrentActor->IsA(AShoppingList::StaticClass()))
		{
			FoundActor = Cast<AShoppingList>(CurrentActor);
			break;
		}
	}

	return FoundActor;
}

void ANPC::UpdateItemsAvailableIndicator()
{
	if (ItemsForSale.IsEmpty() || !ItemsAvailable())
	{
		ItemsAvailableIndicator->SetText(FText::FromString("!"));
		ItemsAvailableIndicator->SetTextRenderColor(FColor::Red);
		return;
	}
	ItemsAvailableIndicator->SetText(FText::FromString("$"));
	ItemsAvailableIndicator->SetTextRenderColor(FColor::Green);
}

bool ANPC::ItemsAvailable()
{
	bool bAnyItemAvailable = false;
	for (int i = 0; i < ItemsForSale.Num(); i++)
	{
		if (ItemsForSale[i].Quantity > 0) { bAnyItemAvailable = true; break; }
	}
	return bAnyItemAvailable;
}




