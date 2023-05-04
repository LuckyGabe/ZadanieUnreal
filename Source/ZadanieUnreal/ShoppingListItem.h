#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "ShoppingListItem.generated.h"

USTRUCT(BlueprintType)
struct FShoppingListItem
{
    GENERATED_BODY()
public:
        UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item")
        FString ItemName;

        UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
            int32 Quantity;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
         UTexture2D* ItemImage;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
        bool bCanBeBought = true;
};