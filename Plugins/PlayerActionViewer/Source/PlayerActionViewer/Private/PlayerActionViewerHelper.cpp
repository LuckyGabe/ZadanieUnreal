
#include "PlayerActionViewerHelper.h"

TArray<FString> UPlayerActionViewerHelper::GetAllSavedFilesPaths()
{
     TArray<FString> FilePaths;
    FString SaveGamesDir = FPaths::ProjectSavedDir() / TEXT("SavedActions/");
    TArray<FString> FileNames;
    IFileManager::Get().FindFiles(FileNames, *SaveGamesDir, TEXT("json"));

    for (const FString& FileName : FileNames)
    {
        FilePaths.Add(FPaths::Combine(SaveGamesDir, FileName));
    }

    return FilePaths;
}

bool UPlayerActionViewerHelper::ReadTextFile(const FString& FilePath, FString& OutFileContents)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    return FFileHelper::LoadFileToString(OutFileContents, *(PlatformFile.ConvertToAbsolutePathForExternalAppForRead(*FilePath)));
}

bool UPlayerActionViewerHelper::ParseJsonString(const FString& JsonString, TArray<FPlayerActionData>& OutData)
{
    TArray<TSharedPtr<FJsonValue>> JsonArray;
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);

    if (FJsonSerializer::Deserialize(JsonReader, JsonArray))
    {
        for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
        {
            TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
            if (JsonObject.IsValid())
            {
                FPlayerActionData PlayerActionData;
                JsonObject->TryGetStringField("player_action", PlayerActionData.PlayerAction);
                JsonObject->TryGetStringField("timestamp", PlayerActionData.Timestamp);
                OutData.Add(PlayerActionData);
            }
        }
        return true;
    }
    return false;
}

void UPlayerActionViewerHelper::GetJsonFieldNames(const TMap<FString, FString>& JsonData, TArray<FString>& OutFieldNames)
{
    OutFieldNames.Empty();
    for (const auto& Entry : JsonData)
    {
        OutFieldNames.Add(Entry.Key);
    }
}

bool UPlayerActionViewerHelper::GetJsonFieldValue(const TMap<FString, FString>& JsonData, const FString& FieldName, FString& OutFieldValue)
{
    if (JsonData.Contains(FieldName))
    {
        OutFieldValue = JsonData[FieldName];
        return true;
    }
    return false;
}
 bool UPlayerActionViewerHelper::GetFileNameFromPath(const FString& FilePath, FString& Name)
{
    Name = FPaths::GetBaseFilename(FilePath);
    if (!Name.IsEmpty()) { return true; }
    else return false;
}
 bool UPlayerActionViewerHelper::DeleteFile(const FString& FilePath)
 {
     IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
     FString AbsoluteFilePath = PlatformFile.ConvertToAbsolutePathForExternalAppForRead(*FilePath);

     if (PlatformFile.FileExists(*AbsoluteFilePath))
     {
         return PlatformFile.DeleteFile(*AbsoluteFilePath);
     }

     return false;
 }
