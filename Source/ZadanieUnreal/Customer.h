
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Customer.generated.h"
class UCameraComponent;

UCLASS()
class ZADANIEUNREAL_API ACustomer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<TSharedPtr<FJsonValue>> PlayerActions;
	FString PlayerActionsFilePath;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SavePlayerAction(const FString& PlayerAction);
	class ANPC* InteractedNPC;
	UPROPERTY()
		bool bLockActions = false;
private:

	//Functions 
	void MoveForward(float scale);
	void MoveRight(float scale);
	void Turn(float scale);
	void LookUp(float scale);
	bool RayTrace(FHitResult& OutHit);
	void ShowHideBasket();
	void Interact();
	void MoveCustomer(FVector Position, float DeltaTime);
	void ShowHideInventory();
	void ShowHideShoppingList();

	void LoadPlayerActions();

	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "Ray Trace")
		float RayTraceRange = 2000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
		float DistanceToBuy = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float TurnRate = 45.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float LookUpRate = 45.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float InterpSpeed = 1000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
		float TargetDistFromNPC = 150.0f;
	UPROPERTY()
		bool bForceMove = false;
	FVector targetPosition;
	class ACustomerController* CustomerController;
	//Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UCameraComponent* CameraComponent;



};
