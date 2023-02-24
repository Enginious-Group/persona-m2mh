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

#include "AssetToolsModule.h"
#include "ObjectTools.h"
#include "PackageTools.h"
#include "Net/UnrealNetwork.h"
#include "PersonaUtilities.h"
#include "Dom/JsonObject.h"



//#include "GroomComponent.h"
// Sets default values for this component's properties
UPersonaComponent::UPersonaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	WeakOwner = TWeakObjectPtr<AActor>(this->GetTypedOuter<AActor>());
	// ...
}

UPersonaComponent::~UPersonaComponent()
{
	//delete presetPaths;
	//delete presetNames;
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
	counterU++;
	if (!bBeingUpdated && counterU == 1) 
	{
		bBeingUpdated = true;
		FString PluginDir = IPluginManager::Get().FindPlugin("Persona3d")->GetBaseDir();
		FString FullPluginPath = FPaths::ConvertRelativePathToFull(PluginDir) + FString(TEXT("/Assets/"));

		//UE_LOG(LogTemp, Display, TEXT("Plugin Path: %s"), *FullPluginPath);

		IFileManager& FileManager = IFileManager::Get();
		TArray<FString> DirectoriesNames;

		//DirectoriesToVisit.Add(FullPluginPath);

		const FString& CurrentDirectory = FullPluginPath;
		TArray<FString> Subdirectories;

		// Get all subdirectories of the current directory
		FileManager.FindFiles(Subdirectories, *(CurrentDirectory + "*"), false, true);
		presetPaths->Reset();
		for (const FString& Subdirectory : Subdirectories)
		{
			//UE_LOG(LogTemp, Display, TEXT("Found directory: %s"), *Subdirectory);
			// Add the subdirectory to the list of directories to visit
			
			presetPaths->Add(FPaths::Combine(CurrentDirectory, Subdirectory));
			DirectoriesNames.Add(Subdirectory);
		}
		TArray<FString>* copyArray = new TArray<FString>();
		copyArray->Append(*presetPaths);
		presetPaths->Append(*copyArray);

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
		bBeingSet = false;
		counterU = 0;
	}
}
void UPersonaComponent::ApplyPreset()
{
	counterA++;
	if (!bBeingSet && counterA == 1)
	{
		UpdateList();
		bBeingSet = true;
		UE_LOG(LogTemp, Display, TEXT("Preset applied"));
		if (bHairColor)
			ApplyHairColor();
		if (bSkinColor)
			ApplySkinColor();
		if (bSkinTexture)
			ApplySkinTexture();
		bBeingSet = false;
		counterA = 0;
	}
}

void UPersonaComponent::ApplySkinColor()
{
	UE_LOG(LogTemp, Display, TEXT("Skin applied"));
}

void UPersonaComponent::ApplyHairColor()
{	
	FString HairName = "Hair";
	USceneComponent* Parent = this->GetAttachParent();
	TArray<USceneComponent*> Parents;
	AActor* ParentActor = WeakOwner.Get();

	TSet<UActorComponent*> ChildrenComponents;
	if (ParentActor != nullptr) {
		
		ChildrenComponents = ParentActor->GetComponents();
		//UE_LOG(LogTemp, Display, TEXT("ParentName: %s"), *(ParentActor->GetName()));
		//UE_LOG(LogTemp, Display, TEXT("ParentName: %d"), ChildrenComponents.Num());
		for (UActorComponent* Component : ChildrenComponents)
		{

			//UE_LOG(LogTemp, Display, TEXT("Component name: %s"), *Component->GetName());
			if (Component->GetName() == HairName)
			{
				//UE_LOG(LogTemp, Display, TEXT("Hair found"));
				UMeshComponent* hairComponent = Cast<UMeshComponent>(Component);
				
				FString name = "/haircolor.json";
				FString skinColorName = "/color_correction.json";
				int32 PresetIndex = static_cast<int32>(currentPreset);
				FString fullPath = (*presetPaths)[PresetIndex] + name;
				FString fieldValue;
				TSharedPtr<FJsonObject> jsonObject = UPersonaUtilities::ReadJson(fullPath);
				jsonObject->TryGetStringField("hair_color", fieldValue);
				FColor currentColor = FColor::FromHex(*fieldValue);
				UE_LOG(LogTemp, Display, TEXT("Skin color: %s"), *fieldValue);
				
				UE_LOG(LogTemp, Display, TEXT("Skin color rgb: %s"), *currentColor.ToString());
				FString PackageName = TEXT("/Game/PeronaMatPackage");
				
				UPackage* MaterialPackage;
				if (FPackageName::DoesPackageExist(PackageName))
				{
					MaterialPackage = LoadPackage(nullptr, *PackageName, LOAD_None);
				}
				else
				{
					MaterialPackage = CreatePackage(nullptr, *PackageName);
				}
				static UMaterialInterface* baseMaterial = hairComponent->GetMaterial(0)->GetBaseMaterial();
				//LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/MetaHumans/Common/Materials/M_hair_v4.M_hair_v4"));
				//UMaterial* baseMaterialInst = LoadObject<UMaterial>(nullptr, *baseMaterial->GetPathName());
				//UMaterialInstanceConstant* NewMaterialInstance = NewObject<UMaterialInstanceConstant>(MaterialPackage, TEXT("M_hair"), RF_Public | RF_Standalone | RF_MarkAsRootSet, baseMaterialInst);
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(baseMaterial, nullptr);
				hairComponent->SetMaterial(0, DynamicMaterial);
				DynamicMaterial->SetVectorParameterValue("hairDye", currentColor);
				//DynamicMaterial->K2_CopyMaterialInstanceParameters(NewMaterialInstance);
				//CopyParameterOverrides
				//MaterialPackage->SetPackageFlag (PKG_EditorOnly | PKG_PlayInEditor);
				MaterialPackage->SetDirtyFlag(true);

				FString PackageFileName = FPackageName::LongPackageNameToFilename(*PackageName, FPackageName::GetAssetPackageExtension());
				bool bSuccess = UPackage::SavePackage(MaterialPackage, nullptr, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone | EObjectFlags::RF_MarkAsRootSet, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);
				FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
				AssetRegistryModule.Get().ScanPathsSynchronous({ MaterialPackage->GetPathName() }, true);
				if (!bSuccess)
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to save package: %s"), *PackageFileName);
					return;
				}

				UE_LOG(LogTemp, Log, TEXT("Package saved: %s"), *PackageFileName);
			};
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Hair applied"));
}

void UPersonaComponent::ApplySkinTexture()
{
	UE_LOG(LogTemp, Display, TEXT("Texture applied"));
}


//// JSON
void UPersonaComponent::ReadJson() 
{
	FString name = "/haircolor.json";
	FString skinColorName = "/color_correction.json";
	for (const FString& path : *presetPaths)
	{
		FString fullPath = path + name;
		FString fieldValue;
		TSharedPtr<FJsonObject> jsonObject = UPersonaUtilities::ReadJson(fullPath);
		jsonObject->TryGetStringField("hair_color", fieldValue);
		//UE_LOG(LogTemp, Display, TEXT("Skin color: %s"), *fieldValue);
		//FColor::FromHex(HexString)
	}
}

