// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonaUtilities.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
FString UPersonaUtilities::ReadStringFromFile(FString FilePath)
{
	FString RetString = "";
	FFileHelper::LoadFileToString(RetString, *FilePath);
	return RetString;
}

TSharedPtr<FJsonObject> UPersonaUtilities::ReadJson(FString JsonFilePath)
{
	FString JsonString = ReadStringFromFile(JsonFilePath);
	TSharedPtr<FJsonObject> RetJsonObject;
	FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), RetJsonObject);
	return RetJsonObject;
}

void UPersonaUtilities::WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject)
{
	FString JsonString;
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0));
	FFileHelper::SaveStringToFile(JsonString, *JsonFilePath);
}



