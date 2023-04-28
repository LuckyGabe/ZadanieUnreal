
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShoppingList.h"
#include "NPCHoverInterface.h"
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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shopping")
		UShoppingList* ShoppingList;
private:

	//Functions 
	void MoveForward(float scale);
	void MoveRight(float scale);
	void Turn(float scale);
	void LookUp(float scale);
	bool RayTrace(FHitResult& OutHit);
	void CustomerClick();
	void Interact();
	void MoveCustomer(FVector Position, float DeltaTime);

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
	UPROPERTY()
		bool bForceMove = false;

	FVector targetPosition;
	float DistanceToSeller;

	//Components
	UPROPERTY(VisibleAnywhere,Category = "Components")
		UCameraComponent* CameraComponent;




};
