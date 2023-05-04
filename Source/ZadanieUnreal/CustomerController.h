// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomerController.generated.h"

/**
 * 
 */
UCLASS()
class ZADANIEUNREAL_API ACustomerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ACustomerController();
	virtual void PlayerTick(float DeltaTime) override;
	UFUNCTION()
		void PopulateStoreWidget(TArray<FShoppingListItem> StoreItems);
	UFUNCTION(BlueprintCallable, Category = "Hover")
		void UpdateHoveredNPC();

	UFUNCTION()
		void ShowStoreUI(TArray<FShoppingListItem> StoreItems, TSubclassOf<class UUserWidget> WidgetClass);
	UFUNCTION(BlueprintCallable)
		void HideStoreUI();

	UFUNCTION()
		void ShowHideBasket();
	UFUNCTION()
		void ShowHideShoppingList();
	UFUNCTION(BlueprintCallable, Category = "Shopping")
		void BuyItem(FShoppingListItem Item);
	UFUNCTION(BlueprintCallable)
		void ShowHideItemInventory();
	UFUNCTION(BlueprintCallable)
		void RemoveItemFromBasket(FString ItemName);
private:
	class ACustomer* Customer;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> CursorWidgetClass;
	UPROPERTY()
		class UUserWidget* CursorWidget;

	UPROPERTY()
		class UUserWidget* StoreWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> ShoppingListWidgetClass;
	UPROPERTY()
		class UUserWidget* ShoppingListWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> BasketWidgetClass;
	UPROPERTY()
		class UUserWidget* BasketWidget;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> ItemInventoryWidgetClass;
	UPROPERTY()
		class UUserWidget* ItemIventoryWidget;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> HUDWidgetClass;
	UPROPERTY()
		class UUserWidget* HUDWidget;

	UFUNCTION()
		void PopulateShoppingList(TArray<FShoppingListItem> StoreItems, bool bIsCompleted);
	UFUNCTION()
		void PopulateBasketWidget(TArray<FShoppingListItem> Items);
	UFUNCTION()
		void PopulateInventoryWidget(TArray<FShoppingListItem> Items);
	UFUNCTION()
		AShoppingList* FindShoppingListClassInLevel();
	UFUNCTION()
	void AddItemToBasket(FShoppingListItem Item);

	UFUNCTION()
		void InitializeWidgets();

	class AShoppingList* ShoppingList;
	UPROPERTY(EditAnywhere, Category = "Shopping")
		TArray<FShoppingListItem> ItemsInBasket;



};
