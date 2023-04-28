// ShoppingList.h
#pragma once

#include "CoreMinimal.h"
#include "ShoppingListItem.h"
#include "ShoppingList.generated.h"

UCLASS(Blueprintable)
class UShoppingList : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
        TArray<FShoppingListItem> Items;


};