// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PersonaUtilities.generated.h"

/**
 * 
 */
UCLASS()
class PERSONA3D_API UPersonaUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//utils
	static FString ReadStringFromFile(FString FilePath);
	//Json
	static TSharedPtr<FJsonObject> ReadJson(FString JsonFilePath);
	static void WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject);

	
};
