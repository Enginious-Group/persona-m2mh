// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HexWrapper.generated.h"


/**
 * 
 */
UCLASS()
class PERSONA3D_API UHexWrapper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Converts hex string to color. Supports formats RGB, RRGGBB, RRGGBBAA, RGB, #RRGGBB, #RRGGBBAA */
	UFUNCTION(BlueprintCallable, Category = "Persona")
		static FColor HexToColor(FString HexString);

	/** Converts color to hex string */
	UFUNCTION(BlueprintCallable, Category = "Persona")
		static FString ColorToHex(FColor Color);

	UFUNCTION(BlueprintCallable, Category = "Persona")
		static FString GetPathToPlugin();
};
