// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"
#include "Components/TextBlock.h"
#include <Blueprint/UserWidget.h>
void UMyBlueprintFunctionLibrary::UpdateTextBoxValue(UUserWidget* UserWidget, FName WidgetBindingName, const FText NewText)
{
    if (UserWidget)
    {
        UTextBlock* TextBox = Cast<UTextBlock>(UserWidget->GetWidgetFromName(WidgetBindingName));
        if (TextBox)
        {
            TextBox->SetText(NewText);
        }
    }
}