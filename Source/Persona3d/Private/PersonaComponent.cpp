// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonaComponent.h"
#include "Interfaces/IPluginManager.h"
//
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"
#include "AssetRegistry/AssetRegistryModule.h"

//#include "GroomComponent.h"
// Sets default values for this component's properties
UPersonaComponent::UPersonaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//UPersonaComponent::UpdateList();

	// ...
}


// Called when the game starts
void UPersonaComponent::BeginPlay()
{
	Super::BeginPlay();
	

	// ...
	
}


// Called every frame
void UPersonaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPersonaComponent::UpdateList()
{
	if (!bBeingUpdated) 
	{
		bBeingUpdated = true;
		FString PluginDir = IPluginManager::Get().FindPlugin("Persona3d")->GetBaseDir();
		FString FullPluginPath = FPaths::ConvertRelativePathToFull(PluginDir) + FString(TEXT("/Assets/"));

		UE_LOG(LogTemp, Display, TEXT("Plugin Path: %s"), *FullPluginPath);

		IFileManager& FileManager = IFileManager::Get();
		TArray<FString> DirectoriesToVisit;
		TArray<FString> DirectoriesNames;

		//DirectoriesToVisit.Add(FullPluginPath);

		const FString& CurrentDirectory = FullPluginPath;
		TArray<FString> Subdirectories;

		// Get all subdirectories of the current directory
		FileManager.FindFiles(Subdirectories, *(CurrentDirectory + "*"), false, true);

		for (const FString& Subdirectory : Subdirectories)
		{
			UE_LOG(LogTemp, Display, TEXT("Found directory: %s"), *Subdirectory);
			// Add the subdirectory to the list of directories to visit
			DirectoriesToVisit.Add(FPaths::Combine(CurrentDirectory, Subdirectory));
			DirectoriesNames.Add(Subdirectory);
		}

		TArray<TPair<FName, int64>> DirNames;
		TArray<int64> Values;
		int index = 0;

		for (const FString& String : DirectoriesNames)
		{

			// create an FName object from the FString
			DirNames.Add(MakeTuple(FName(*String), static_cast<int64>(index))); // add the FName object to the output array
			index++;
		}

		UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, TEXT("PersonaPreset"), true);
		Enum->SetEnums(DirNames, UEnum::ECppForm::Regular);
		presetPaths.Empty();
		presetPaths = DirectoriesToVisit;
		bBeingSet = false;
	}


}


void UPersonaComponent::ApplyPreset()
{
	if (!bBeingSet) 
	{
		bBeingSet = true;
		UE_LOG(LogTemp, Display, TEXT("Preset applied"));
		if (bHairColor)
			ApplyHairColor();
		if (bSkinColor)
			ApplySkinColor();
		if (bSkinTexture)
			ApplySkinTexture();
		bBeingSet = false;
	}
}

void UPersonaComponent::SaveMaterialInstance()
{
	//UMaterialInstanceDynamic* DynamicMaterialInstance;
	//UMaterialInstance* MaterialInstanceAsset;

	//MaterialInstanceAsset->SetParentInstance(DynamicMaterialInstance);

}

void UPersonaComponent::ApplySkinColor()
{
	UE_LOG(LogTemp, Display, TEXT("Skin applied"));
}

void UPersonaComponent::ApplyHairColor()
{
	USceneComponent* ParentComponent = Cast<USceneComponent>(this)->GetAttachParent();
	FString HairName = "Hair";

	TArray<USceneComponent*> SceneComponents = ParentComponent->GetAttachChildren();
	for (USceneComponent* Component : SceneComponents)
	{
		if (Component->GetName() == HairName)
		{
			//UGroomComponent* GroomComponent = Cast<UGroomComponent>(Component);
			//Cast<UGroomComponent>(Component)

			UMeshComponent* groomComponent = Cast<UMeshComponent>(Component);
			//UMaterialInterface* currentMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/MyMaterials/MyMaterial"));
			UMaterialInterface* currentGroomMaterial = groomComponent->GetMaterial(0);

			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(currentGroomMaterial, nullptr);

			UMaterialInstanceConstant* NewMaterialInstance = NewObject<UMaterialInstanceConstant>(GetTransientPackage(), TEXT("/Game/MyMaterials/MyNewMaterialInstance"));

			DynamicMaterial->CopyParameterOverrides(NewMaterialInstance);

			// Save the new material instance to disk
			FAssetRegistryModule::AssetCreated(NewMaterialInstance);
			NewMaterialInstance->MarkPackageDirty();

			groomComponent->SetMaterial(0, NewMaterialInstance);
		};
	};
	UE_LOG(LogTemp, Display, TEXT("Hair applied"));

		

}

void UPersonaComponent::ApplySkinTexture()
{
	UE_LOG(LogTemp, Display, TEXT("Texture applied"));
}


