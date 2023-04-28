#pragma once

#include "CoreMinimal.h"
#include "ShoppingListItem.generated.h"

USTRUCT(BlueprintType)
struct FShoppingListItem
{
    GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
        FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
        int32 Quantity;
};