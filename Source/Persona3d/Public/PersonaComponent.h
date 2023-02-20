// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/ScriptDelegates.h"
#include "PersonaComponent.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValueChanged);

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
		bool bSkinTexture;
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Persona")
		bool bHairColor;
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Persona")
		bool bSkinColor;
	UPROPERTY(EditAnywhere, Category = "Persona")
		bool bUpdateList;
	UPROPERTY(BlueprintAssignable, Category = "Persona")
		FOnValueChanged OnValueChanged;
	

	virtual void PostInitProperties() override 
	{
		Super::PostInitProperties();
		//OnValueChanged.BindDynamic(this, &UPersonaComponent::HandleValueChanged);
	}

	void HandleValueChanged()
	{
		if (bUpdateList) 
		{
			bUpdateList = false;
			UpdateList();
		}
		if (bApplyPreset)
		{
			bApplyPreset = false;
			ApplyPreset();
		}
	}


	UPROPERTY(EditAnywhere, Category = "Persona")
		bool bApplyPreset;
	UFUNCTION(CallInEditor, Category = "Persona")
		void UpdateList();
	UFUNCTION(CallInEditor, Category = "Persona")
		void ApplyPreset();



	//BlueprintCallable,
private:
	TArray<FString> presetPaths;
	TArray<FString> presetNames;
	bool bBeingSet;
	bool bBeingUpdated;
	void SaveMaterialInstance();
	void ApplySkinColor();
	void ApplyHairColor();
	void ApplySkinTexture();
	
	
	
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	//{
	//	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UPersonaComponent, bUpdateList))
	//	{
	//		bUpdateList = false;
	//		UpdateList();
	//	}
	//	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UPersonaComponent, bApplyPreset))
	//	{
	//		bApplyPreset = false;
	//		ApplyPreset();
	//	}
	//	Super::PostEditChangeProperty(PropertyChangedEvent);
	//}


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
