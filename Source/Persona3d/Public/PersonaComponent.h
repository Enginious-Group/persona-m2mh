// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PersonaComponent.generated.h"

UENUM()
enum class PersonaPreset
{
	none
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PERSONA3D_API UPersonaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPersonaComponent();
	UPROPERTY(EditAnywhere, Category = "Persona")
		PersonaPreset currentPreset;
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Persona")
		bool skinTexture;
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Persona")
		bool hairColor;
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Persona")
		bool skinColor;
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Persona")
		void UpdateList();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Persona")
		void ApplyPreset();


private:
	TArray<FString> presetPaths;
	TArray<FString> presetNames;
	void SaveMaterialInstance();
	void ApplySkinColor();
	void ApplyHairColor();
	void ApplySkinTexture();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
