// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerController.h"
#include "NPC.h"
#include <Blueprint/UserWidget.h>
#include "Kismet/GameplayStatics.h"
#include "MyBlueprintFunctionLibrary.h"

ACustomerController::ACustomerController()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ACustomerController::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); //get player controller
    if (BP_UI)
    {
        UI = CreateWidget<UUserWidget>(PlayerController, BP_UI);
        UI->AddToViewport();
    }
    CustomerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
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
    Params.AddIgnoredActor(CustomerPawn);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1, Params))
    {
        ANPC* HitActor = Cast<ANPC>(HitResult.GetActor());
        if (HitActor)
        {
            // Hovering over an NPC
            if (HitActor->DistanceToCustomer() > 200.f)
            {
            UMyBlueprintFunctionLibrary::UpdateTextBoxValue(UI, "MouseCursor", FText::FromString(TEXT("^")));
            }
            else
            {
                UMyBlueprintFunctionLibrary::UpdateTextBoxValue(UI, "MouseCursor", FText::FromString(TEXT("$")));
            }

        }
        else
        {
            // Not hovering over an NPC
            UMyBlueprintFunctionLibrary::UpdateTextBoxValue(UI, "MouseCursor", FText::FromString(TEXT(".")));
        }
    }
    else 
        // Not hovering over an NPC
        UMyBlueprintFunctionLibrary::UpdateTextBoxValue(UI, "MouseCursor", FText::FromString(TEXT(".")));
}
