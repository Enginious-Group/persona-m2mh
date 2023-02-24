// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UObject/ScriptDelegates.h"
#include "PersonaComponent.generated.h"



UENUM()
enum class PersonaPreset : uint8
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
	~UPersonaComponent();
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
	

	//

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UPersonaComponent, bApplyPreset) && counterA == 0 && bApplyPreset && this->GetClass()->GetName() == "PersonaComponent")
		{
			bApplyPreset = false;
			ApplyPreset();
			return;
		}
		else if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UPersonaComponent, bUpdateList) && counterU == 0 && bUpdateList && this->GetClass()->GetName() == "PersonaComponent")
		{
			bUpdateList = false;
			UpdateList();
			return;
		}
		return;
	}

	//BlueprintCallable,
private:
	TArray<FString>* presetPaths = new TArray<FString>;
	TArray<FString>* presetNames = new TArray<FString>;
	int counterU = 0;
	int counterA = 0;
	bool bBeingSet;
	bool bBeingUpdated;
	void ApplySkinColor();
	void ApplyHairColor();
	void ApplySkinTexture();
	void ReadJson();
	void SaveDynamicMatInstance(UMaterialInstanceDynamic* DynamicMaterialInstance, FString AssetName, FString PackagePath);
	TWeakObjectPtr<AActor> WeakOwner;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
