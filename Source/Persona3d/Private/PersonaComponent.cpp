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

#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "CoreTypes.h"
#include "ImageUtils.h"


//#include "RenderingCompositionGraph.h"
//#include "FRenderingCompositePassContext.h"
//#include "GroomComponent.h"
// Sets default values for this component's properties
UPersonaComponent::UPersonaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	WeakOwner = TWeakObjectPtr<AActor>(this->GetTypedOuter<AActor>());
	UpdateList();
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

		copyArray->Reset();
		copyArray->Append(DirectoriesNames);
		presetNames->Append(*copyArray);

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
				FString PackageName = TEXT("/Game/Persona/PersonaMatPackage");
				
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
	FString ComponentName = "Face";
	USceneComponent* Parent = this->GetAttachParent();
	TArray<USceneComponent*> Parents;
	AActor* ParentActor = WeakOwner.Get();
	//Albedo Setup
	/*FString AlbedoPath = "/Persona3d/t_persona.t_persona";
	
	FStringAssetReference AlbedoRef(AlbedoPath);
	UObject* AlbedoObject = AlbedoRef.TryLoad();
	UTexture* tAlbedoObject = Cast<UTexture>(AlbedoObject);
	if (tAlbedoObject)
	{
		UE_LOG(LogTemp, Display, TEXT("Albedo texture: %s"), *tAlbedoObject->GetName());
	}
	//Normal Setup
	FString NormalPath = "/Persona3d/n_persona.n_persona";
	FStringAssetReference NormalRef(NormalPath);
	UObject* NormalObject = NormalRef.TryLoad();
	UTexture* tNormalObject = Cast<UTexture>(NormalObject);
	if (tNormalObject)
	{
		UE_LOG(LogTemp, Display, TEXT("Albedo texture: %s"), *tNormalObject->GetName());
	}

	//Roughness setup
	;*/
	FString folderPath = WeakOwner.Get()->GetClass()->GetClassPathName().ToString();
	FString className = WeakOwner.Get()->GetClass()->GetName();
	FString classNamenoc = className.LeftChop(2);
	FString metahumanName = classNamenoc.RightChop(3);
	FString classSubstring = TEXT("/") + classNamenoc + TEXT(".") + className;
	int32 indexToRemove = folderPath.Find(classSubstring);
	if (indexToRemove != INDEX_NONE) {
		folderPath = folderPath.Left(indexToRemove) + folderPath.Mid(indexToRemove + classSubstring.Len());
	}
	// Game / MetaHumans / Nicole /
	FString RoughnessPath = folderPath + "/Face/FaceRoughness_MAIN.FaceRoughness_MAIN";
	UE_LOG(LogTemp, Display, TEXT("Skin texture: %s"), *RoughnessPath);
	FStringAssetReference RoughnessRef(RoughnessPath);
	UObject* RoughnessObject = RoughnessRef.TryLoad();
	UTexture* tRoughnessObject = Cast<UTexture>(RoughnessObject);
	UTexture2D* nTexture = ImportAndSaveTexture(TEXT("n"));
	tRoughnessObject->CompositeTexture = nTexture;



	TSet<UActorComponent*> ChildrenComponents;
	if (ParentActor != nullptr) {

		ChildrenComponents = ParentActor->GetComponents();
		for (UActorComponent* Component : ChildrenComponents)
		{
			if (Component->GetName() == ComponentName)
			{
				USkeletalMeshComponent* faceComponent = Cast<USkeletalMeshComponent>(Component);
				if (faceComponent) {
					UMaterialInterface* baseMaterial = faceComponent->GetMaterial(0)->GetBaseMaterial();
					FString materialPath = folderPath + "/Face/Materials/Baked/MI_HeadSynthesized_Baked_LOD1.MI_HeadSynthesized_Baked_LOD1";
					UMaterialInterface* mainMat = LoadObject<UMaterialInterface>(nullptr, *materialPath);
					TArray<FName> TextureParamNames;
					UE_LOG(LogTemp, Display, TEXT("Skin texture: %s"), *baseMaterial->GetName());

					UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(mainMat, nullptr);

					//
					UTexture2D* tTexture = ImportAndSaveTexture(TEXT("t"));
					//nTexture = ImportAndSaveTexture(TEXT("n"));
					//UTexture2D* rTexture = ImportAndSaveTexture(TEXT("r"));
					//

					DynamicMaterial->SetTextureParameterValue("color_MAIN", tTexture);
					DynamicMaterial->SetTextureParameterValue("color_CM1", tTexture);
					DynamicMaterial->SetTextureParameterValue("color_CM2", tTexture);
					DynamicMaterial->SetTextureParameterValue("color_CM3", tTexture);

					DynamicMaterial->SetTextureParameterValue("roughness_MAIN", tRoughnessObject);

					DynamicMaterial->SetTextureParameterValue("normal_MAIN", nTexture);
					DynamicMaterial->SetTextureParameterValue("normal_WM1", nTexture);
					DynamicMaterial->SetTextureParameterValue("normal_WM2", nTexture);
					DynamicMaterial->SetTextureParameterValue("normal_WM3", nTexture);
					DynamicMaterial->SetTextureParameterValue("normal_BAKED", nTexture);

					faceComponent->SetMaterial(0, DynamicMaterial);
					faceComponent->SetMaterial(9, DynamicMaterial);
					faceComponent->SetMaterial(11, DynamicMaterial);	 
					faceComponent->SetMaterial(12, DynamicMaterial);
					faceComponent->SetMaterial(13, DynamicMaterial);
					faceComponent->SetMaterial(14, DynamicMaterial);

					DynamicMaterial->ForceRecompileForRendering();
				}
			};
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Texture applied"))

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

// Sign t, /n_, /r_
UTexture2D* UPersonaComponent::ImportAndSaveTexture(FString Sign)
{
	Sign = TEXT("/") + Sign + TEXT("_");
	int32 PresetIndex = static_cast<int32>(currentPreset);
	//Source setup
	FString fullPath = (*presetPaths)[PresetIndex] + Sign + (*presetNames)[PresetIndex] + TEXT(".png");
	UE_LOG(LogTemp, Display, TEXT("fullAsset Path: %s"), *fullPath);

	UTexture2D* TargetTexture = UTexture2D::CreateTransient(256, 256, PF_B8G8R8A8);
	TargetTexture = FImageUtils::ImportFileAsTexture2D(*fullPath);
	// Set the texture's properties
	TargetTexture->SRGB = true;
	TargetTexture->CompressionSettings = TC_Default;
	TargetTexture->Filter = TF_Default;
	TargetTexture->AddressX = TA_Clamp;
	TargetTexture->AddressY = TA_Clamp;
	TargetTexture->UpdateResource();
	UE_LOG(LogTemp, Display, TEXT("It works: %s"), *TargetTexture->GetName());


	//Target Setup
	FString folderPath = WeakOwner.Get()->GetClass()->GetClassPathName().ToString();
	FString className = WeakOwner.Get()->GetClass()->GetName();
	FString classNamenoc = className.LeftChop(2);
	FString metahumanName = classNamenoc.RightChop(3);
	FString classSubstring = TEXT("/") + classNamenoc + TEXT(".") + className;
	int32 indexToRemove = folderPath.Find(classSubstring);
	if (indexToRemove != INDEX_NONE) {
		folderPath = folderPath.Left(indexToRemove) + folderPath.Mid(indexToRemove + classSubstring.Len());
	}
	// Game / MetaHumans / Nicole /

	FString targetPath = folderPath + "/Persona" + Sign + metahumanName;
	FString targetName = Sign.LeftChop(1) + metahumanName;
	UE_LOG(LogTemp, Display, TEXT("target Path: %s"), *targetPath);


	/*if (TargetTexture)
	{
		//TargetTexture->Source.Init(TargetTexture->GetSizeX(), TargetTexture->GetSizeY(), 1, 1, ETextureSourceFormat::TSF_BGRA8, DataBuffer.GetData());

		//Deletion of Package 
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		if (PlatformFile.FileExists(*targetPath))
		{
			PlatformFile.DeleteFile(*targetPath);
		}


		FString PackageName = targetPath;
		UPackage* Package = CreatePackage(*PackageName);
		Package->FullyLoad();
		TargetTexture->Rename(*PackageName);
		TargetTexture->AddToRoot();
		FAssetRegistryModule::AssetCreated(TargetTexture);
		Package->MarkPackageDirty();
		FAssetRegistryModule::AssetCreated(TargetTexture);
		FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
		UE_LOG(LogTemp, Log, TEXT("Package filename: %s"), *PackageFileName);
		//bool bSuccess = UPackage::SavePackage(Package, nullptr, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone | EObjectFlags::RF_MarkAsRootSet, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);
		bool bSuccess = UPackage::SavePackage(Package, nullptr, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		AssetRegistryModule.Get().ScanPathsSynchronous({ Package->GetPathName() }, true);
		if (!bSuccess)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save package: %s"), *PackageName);
			return TargetTexture;
		}
		UE_LOG(LogTemp, Log, TEXT("Package saved: %s"), *PackageName);
		return TargetTexture;
	}*/
	return TargetTexture;
}


// Texture utils


