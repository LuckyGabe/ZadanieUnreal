// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShoppingListItem.h"
#include "ShoppingList.generated.h"

UCLASS()
class ZADANIEUNREAL_API AShoppingList : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShoppingList();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
		TArray<FShoppingListItem> Items;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
		TArray<class ANPC*> NPCReferences;

	void CheckAndDecreaseQuantity(FString ItemName, bool& bIsItemOnList, bool& bListCompleted);
	void CheckAndIncreaseQuantity(FString ItemName, bool& bIsItemOnList, bool& bListCompleted);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Shopping List")
		void PopulateShoppingListInEditor(ANPC* NPCRef);
private:
	UFUNCTION()
		bool ItemInList(FString ItemName);
};
