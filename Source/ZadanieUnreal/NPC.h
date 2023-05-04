// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShoppingListItem.h"
#include "Components/TextRenderComponent.h"
#include "NPC.generated.h"



UCLASS()
class ZADANIEUNREAL_API ANPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostEditMove(bool bFinished) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		void InteractWithCustomer();
	float DistanceToCustomer();
	void RemoveItemFromTheStore(FString ItemName);
	UPROPERTY(EditDefaultsOnly, Category = "Items")
		TArray<FShoppingListItem> ItemsForSale;

private:
	UPROPERTY(VisibleAnywhere,Category = "Components")
		USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* StaticMesh;
	USceneComponent* rootComponent;

	UPROPERTY()
		TArray<int32>MaxNumberOfItems;
	UPROPERTY()
	class ACustomerController* CustomerController;

	UPROPERTY(EditDefaultsOnly, Category = "Items")
		float RestoreMaxNumbOfItemsTime = 15.f;
	UPROPERTY(EditDefaultsOnly, Category = "Store")
		TSubclassOf<class UUserWidget> StoreWidgetClass;
	UPROPERTY(VisibleAnywhere,Category = "Indicator")
		UTextRenderComponent* ItemsAvailableIndicator;
	UFUNCTION()
		void StoreMaxNumberOfItems();
	FTimerHandle ResetItemsQuantityTimerHandle;
	UFUNCTION()
		void ResetItemsQuantity();

	UFUNCTION()
		AShoppingList* FindShoppingListClassInLevel();
	UFUNCTION()
	void UpdateItemsAvailableIndicator();
	UFUNCTION()
		bool ItemsAvailable();
#if WITH_EDITOR
	UFUNCTION()
		void AssignNPCReferenceToShoppingList();
#endif

};
