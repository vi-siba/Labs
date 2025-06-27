// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Json.h"   
#include "JsonUtilities.h"
#include "HAL/PlatformFilemanager.h"

TArray<FString> UMyGameModeBase::LoadWordsFromJson()
{
    TArray<FString> Words;

    //сделать название файла переменной под конфигурацию, минимум четко объ€вленных переменных;
   //words.json;
    FString FilePath = FPaths::ProjectContentDir() / TEXT("words.json");
    FString JsonString;

    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("JSON file not found: %s"), *FilePath);
        
        //return Words;
        // возвращать указатель на массив, а не массив;
    }
    
  

    if (FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);


        if (!FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            return null;
        }
     
        {
            const TArray<TSharedPtr<FJsonValue>>* JsonWords;

            if (JsonObject->TryGetArrayField(TEXT("Words"), JsonWords))
            {
                for (const TSharedPtr<FJsonValue>& Value : *JsonWords)
                {
                    FString Word = Value->AsString();
                    Words.Add(Word);
                }
            }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load file: %s"), *FilePath);
    }

    return Words;
}