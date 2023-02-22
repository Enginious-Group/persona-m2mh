// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UObject/ScriptDelegates.h"
#include "PersonaComponent.generated.h"



UENUM()
enum class PersonaPreset
{
	none
};
//Previously UActorComponent
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PERSONA3D_API UPersonaComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPersonaComponent();
	UPROPERTY(EditAnywhere, Category = "Persona")
		PersonaPreset currentPreset;
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Persona")
		bool bSkinTexture;
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Persona")
		bool bHairColor;
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Persona")
		bool bSkinColor;
	UPROPERTY(EditAnywhere, Category = "Persona")
		bool bUpdateList;


	UPROPERTY(EditAnywhere, Category = "Persona")
		bool bApplyPreset;
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Persona")
		void UpdateList();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Persona")
		void ApplyPreset();

	

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UPersonaComponent, bApplyPreset))
		{
			ApplyPreset();
			bApplyPreset = false;

		}
		if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UPersonaComponent, bUpdateList))
		{
			UpdateList();
			bUpdateList = false;
		}

		Super::PostEditChangeProperty(PropertyChangedEvent);
	}



	//BlueprintCallable,
private:
	TArray<FString> presetPaths;
	TArray<FString> presetNames;
	bool bBeingSet;
	bool bBeingUpdated;
	void ApplySkinColor();
	void ApplyHairColor();
	void ApplySkinTexture();
	void ReadJson();
	TWeakObjectPtr<AActor> WeakOwner;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
