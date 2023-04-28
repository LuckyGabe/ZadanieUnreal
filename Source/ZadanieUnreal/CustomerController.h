// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NPCHoverInterface.h"
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

	UFUNCTION(BlueprintCallable, Category = "Hover")
		void UpdateHoveredNPC();

private:
	APawn* CustomerPawn;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> BP_UI;
	UPROPERTY()
		class UUserWidget* UI;
};
