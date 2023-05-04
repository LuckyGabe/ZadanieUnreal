﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerController.h"
#include "NPC.h"
#include <Blueprint/UserWidget.h>
#include "Kismet/GameplayStatics.h"
#include "MyBlueprintFunctionLibrary.h"
#include "ShoppingListItem.h"
#include "EngineUtils.h"
#include "Customer.h"
#include "ShoppingList.h"
ACustomerController::ACustomerController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ACustomerController::BeginPlay()
{
	Super::BeginPlay();

	Customer = Cast<ACustomer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	InitializeWidgets();


	// Hide the mouse cursor
	bShowMouseCursor = false;

}

void ACustomerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	UpdateHoveredNPC();
}


void ACustomerController::UpdateHoveredNPC()
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector Start = CameraLocation;
	FVector End = CameraLocation + CameraRotation.Vector() * 5000.f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Customer);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1, Params))
	{
		ANPC* HitActor = Cast<ANPC>(HitResult.GetActor());
		if (HitActor)
		{
			// Hovering over an NPC
			if (HitActor->DistanceToCustomer() > 200.f)
			{

				UMyBlueprintFunctionLibrary::UpdateTextBoxValue(CursorWidget, "MouseCursor", FText::FromString(TEXT("^")), FSlateColor(FLinearColor::Yellow));
			}
			else
			{
				UMyBlueprintFunctionLibrary::UpdateTextBoxValue(CursorWidget, "MouseCursor", FText::FromString(TEXT("$")), FSlateColor(FLinearColor::Green));
			}

		}
		else
		{
			// Not hovering over an NPC
			UMyBlueprintFunctionLibrary::UpdateTextBoxValue(CursorWidget, "MouseCursor", FText::FromString(TEXT(".")), FSlateColor(FLinearColor::Red));
		}
	}
	else
		// Not hovering over an NPC
		UMyBlueprintFunctionLibrary::UpdateTextBoxValue(CursorWidget, "MouseCursor", FText::FromString(TEXT(".")), FSlateColor(FLinearColor::Red));
}

void ACustomerController::ShowStoreUI(TArray<FShoppingListItem> StoreItems, TSubclassOf<class UUserWidget> WidgetClass)
{
	StoreWidget = CreateWidget<UUserWidget>(this, WidgetClass);
	if (StoreWidget)
	{
		if (!StoreWidget->IsInViewport())
		{
			StoreWidget->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
			Customer->SavePlayerAction("Otwarto Sklep");
		}
		PopulateStoreWidget(StoreItems);
	}
}
void ACustomerController::HideStoreUI()
{
	if (StoreWidget)
	{
		StoreWidget->RemoveFromParent();
		Customer->SavePlayerAction("Zamknieto sklep");
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}

void ACustomerController::ShowHideBasket()
{

	if (!BasketWidget->IsInViewport())
	{
		BasketWidget->AddToViewport(5);
		Customer->SavePlayerAction("Pokaz koszyk podreczny");
	}
	else
	{
		BasketWidget->RemoveFromParent();
		Customer->SavePlayerAction("Ukryj koszyk podreczny");
	}
}

void ACustomerController::ShowHideShoppingList()
{
	if (!ShoppingListWidget->IsInViewport())
	{
		ShoppingListWidget->AddToViewport(5);
		Customer->SavePlayerAction("Pokaz liste zakupow");
	}
	else
	{
		ShoppingListWidget->RemoveFromParent();
		Customer->SavePlayerAction("Ukryj koszyk podreczny");
	}
}

void ACustomerController::BuyItem(FShoppingListItem Item)
{
	if (Item.Quantity == 0 || !Item.bCanBeBought) {return; }
	bool bIsItemOnList = false;
	bool bIsListCompleted = false;
	if (Customer)
	{
		Customer->InteractedNPC->RemoveItemFromTheStore(Item.ItemName);
		PopulateStoreWidget(Customer->InteractedNPC->ItemsForSale);
	}
	if (ShoppingList)
	{
		ShoppingList->CheckAndDecreaseQuantity(Item.ItemName, bIsItemOnList, bIsListCompleted);
	}
	Customer->SavePlayerAction("Kupiono przedmiot: " + Item.ItemName);
	AddItemToBasket(Item);
	PopulateBasketWidget(ItemsInBasket);
	if (bIsItemOnList) { PopulateShoppingList(ShoppingList->Items, bIsListCompleted); }

}
void ACustomerController::AddItemToBasket(FShoppingListItem Item)
{
	for (int i = 0; i < ItemsInBasket.Num(); i++)
	{
		//if the item is in shopping list and the quantity needed is more than 0
		if (ItemsInBasket[i].ItemName.Equals(Item.ItemName))
		{
			ItemsInBasket[i].Quantity++;
			return;
		}
	}
	Item.Quantity = 1;
	ItemsInBasket.Add(Item);
}

void ACustomerController::InitializeWidgets()
{
	if (CursorWidgetClass)
	{
		CursorWidget = CreateWidget<UUserWidget>(this, CursorWidgetClass);
		CursorWidget->AddToViewport();

	}
	if (ShoppingListWidgetClass)
	{
		ShoppingListWidget = CreateWidget<UUserWidget>(this, ShoppingListWidgetClass);
		ShoppingListWidget->AddToViewport(5);

		ShoppingList = Cast<AShoppingList>(FindShoppingListClassInLevel());

		if (ShoppingList && !ShoppingList->Items.IsEmpty())
		{
			PopulateShoppingList(ShoppingList->Items, false);
		}

	}
	if (BasketWidgetClass)
	{
		BasketWidget = CreateWidget<UUserWidget>(this, BasketWidgetClass);
		BasketWidget->AddToViewport(5);
	}

	if (ItemInventoryWidgetClass) 
	{
		ItemIventoryWidget = CreateWidget<UUserWidget>(this, ItemInventoryWidgetClass);
	}
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		HUDWidget->AddToViewport(10);
	}

}

void ACustomerController::ShowHideItemInventory()
{
	if(!ItemIventoryWidget->IsInViewport())
	{
		ItemIventoryWidget->AddToViewport(2);
		bShowMouseCursor = true;
		BasketWidget->RemoveFromParent();
		Customer->SavePlayerAction("Otwarto okno koszykowe");
		PopulateInventoryWidget(ItemsInBasket);
	}
	else
	{
		ItemIventoryWidget->RemoveFromParent();
		BasketWidget->AddToViewport(5);
		bShowMouseCursor = false;
		Customer->SavePlayerAction("Zamknięto okno koszykowe");
	}
}

void ACustomerController::RemoveItemFromBasket(FString ItemName)
{

	for (int i = 0; i < ItemsInBasket.Num(); i++)
	{
		//if the item is in shopping list and the quantity needed is more than 0
		if (ItemsInBasket[i].ItemName.Equals(ItemName))
		{
			
			bool bIsItemOnList = false;
			bool bIsListCompleted = false;
			ItemsInBasket[i].Quantity--;
			if (ItemsInBasket[i].Quantity == 0) { ItemsInBasket.RemoveAt(i);}
			PopulateBasketWidget(ItemsInBasket);
			PopulateInventoryWidget(ItemsInBasket);
			ShoppingList->CheckAndIncreaseQuantity(ItemName, bIsItemOnList, bIsListCompleted);
			PopulateShoppingList(ShoppingList->Items, bIsListCompleted);
			Customer->SavePlayerAction("Wyrzucono przedmiot: " + ItemName);
			return;
		}
	}
}

void ACustomerController::PopulateStoreWidget(TArray<FShoppingListItem> StoreItems)
{

	UFunction* InitializeStoreFunction = StoreWidget->FindFunction(FName("InitializeStore"));
	if (InitializeStoreFunction)
	{
		StoreWidget->ProcessEvent(InitializeStoreFunction, &StoreItems);
	}
}
void ACustomerController::PopulateShoppingList(TArray<FShoppingListItem> ShoppingListItems, bool bIsCompleted)
{

	UFunction* InitializeShoppingListFunction = ShoppingListWidget->FindFunction(FName("InitializeShoppingList"));
	UFunction* SetListStatusFunction = ShoppingListWidget->FindFunction(FName("SetListStatusFunction"));
	if (InitializeShoppingListFunction)
	{
		ShoppingListWidget->ProcessEvent(InitializeShoppingListFunction, &ShoppingListItems);
	}
	if (SetListStatusFunction)
	{
		ShoppingListWidget->ProcessEvent(SetListStatusFunction, &bIsCompleted);
	}
}

void ACustomerController::PopulateBasketWidget(TArray<FShoppingListItem> Items)
{

	UFunction* InitializeBasketFunction = BasketWidget->FindFunction(FName("InitializeBasket"));
	if (InitializeBasketFunction)
	{

		BasketWidget->ProcessEvent(InitializeBasketFunction, &Items);
	}
}
void ACustomerController::PopulateInventoryWidget(TArray<FShoppingListItem> Items)
{

	UFunction* InitializeInventoryFunction = ItemIventoryWidget->FindFunction(FName("InitializeInventory"));
	if (InitializeInventoryFunction)
	{

		ItemIventoryWidget->ProcessEvent(InitializeInventoryFunction, &Items);
	}
}

AShoppingList* ACustomerController::FindShoppingListClassInLevel()
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
