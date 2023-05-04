// Fill out your copyright notice in the Description page of Project Settings.


#include "ShoppingList.h"
#include "NPC.h"
// Sets default values
AShoppingList::AShoppingList()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShoppingList::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].Quantity == 0) { Items.RemoveAt(i); }
	}
}

// Called every frame
void AShoppingList::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShoppingList::CheckAndDecreaseQuantity(FString ItemName, bool& bIsItemOnList, bool& bListCompleted)
{
	bIsItemOnList = ItemInList(ItemName);
	if (!bIsItemOnList) { return; }
	int ItemsNeeded = 0;

	for (int i = 0; i < Items.Num(); i++)
	{
		ItemsNeeded += Items[i].Quantity;
		//if the item is in shopping list and the quantity needed is more than 0
		if (Items[i].ItemName.Equals(ItemName) && Items[i].Quantity > 0)
		{
			//remove item
			Items[i].Quantity--;
			ItemsNeeded--;
		}
	}
	if (ItemsNeeded == 0) { bListCompleted = true; }
	else { bListCompleted = false; }
}
void AShoppingList::CheckAndIncreaseQuantity(FString ItemName, bool& bIsItemOnList, bool& bListCompleted)
{
	bIsItemOnList = ItemInList(ItemName);
	if (!bIsItemOnList) { return; }
	int ItemsNeeded = 0;

	for (int i = 0; i < Items.Num(); i++)
	{
		ItemsNeeded += Items[i].Quantity;
		//if the item is in shopping list and the quantity needed is more than 0
		if (Items[i].ItemName.Equals(ItemName) && Items[i].Quantity >= 0)
		{
			//remove item
			Items[i].Quantity++;
			ItemsNeeded++;
		}
	}
	if (ItemsNeeded == 0) { bListCompleted = true; }
	else { bListCompleted = false; }
}

void AShoppingList::PopulateShoppingListInEditor(ANPC* NPCRef)
{

	if (!NPCReferences.Contains(NPCRef)) { NPCReferences.Add(NPCRef); }
	// Clear the existing Items array
	Items.Empty();
	for (ANPC* NPCReference : NPCReferences)
	{
		if (NPCReference)
		{
			for (int i = 0; i < NPCReference->ItemsForSale.Num(); i++)
			{
				if (NPCReference->ItemsForSale[i].bCanBeBought)
				{
					if (ItemInList(NPCReference->ItemsForSale[i].ItemName)) { continue; }
					Items.Add(NPCReference->ItemsForSale[i]);
					Items.Last().Quantity = 0;
				}
			}
		}
	}
}
bool AShoppingList::ItemInList(FString ItemName)
{
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].ItemName.Equals(ItemName)) { return true; }
	}
	return false;
}