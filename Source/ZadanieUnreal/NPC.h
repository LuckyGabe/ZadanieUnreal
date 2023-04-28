// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShoppingListItem.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void InteractWithCustomer();
	float DistanceToCustomer();
private:
	UPROPERTY(VisibleAnywhere,Category = "Components")
		USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* StaticMesh;
	USceneComponent* rootComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Items")
		TArray<FShoppingListItem> ItemsForSale;



};
