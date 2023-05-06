// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/FileManager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "UObject/ObjectMacros.h"

#include "PlayerActionViewerHelper.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerActionData
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Action Data")
		FString PlayerAction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Action Data")
		FString Timestamp;
};
UCLASS()
class PLAYERACTIONVIEWER_API UPlayerActionViewerHelper : public UObject
{
	GENERATED_BODY()
public:
		UFUNCTION(BlueprintCallable, Category = "Files")
		static TArray<FString> GetAllSavedFilesPaths();

		UFUNCTION(BlueprintCallable, Category = "Files")
			static bool ReadTextFile(const FString& FilePath, FString& OutFileContents);
		UFUNCTION(BlueprintCallable, Category = "JSON Helper")
			static bool ParseJsonString(const FString& JsonString, TArray<FPlayerActionData>& OutData);
		UFUNCTION(BlueprintCallable, Category = "JSON Helper")
			static void GetJsonFieldNames(const TMap<FString, FString>& JsonData, TArray<FString>& OutFieldNames);
		UFUNCTION(BlueprintCallable, Category = "JSON Helper")
			static bool GetJsonFieldValue(const TMap<FString, FString>& JsonData, const FString& FieldName, FString& OutFieldValue);
		UFUNCTION(BlueprintCallable, Category = "JSON Helper")
		static bool GetFileNameFromPath(const FString& FilePath, FString& Name);
		UFUNCTION(BlueprintCallable, Category = "File Management")
			static bool DeleteFile(const FString& FilePath);

};
