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
#include "Serialization/ArchiveSaveCompressedProxy.h"
#include "Misc/FileHelper.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "AssetViewUtils.h"

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

		IFileManager& FileManager = IFileManager::Get();
		TArray<FString> DirectoriesNames;

		const FString& CurrentDirectory = FullPluginPath;
		TArray<FString> Subdirectories;

		FileManager.FindFiles(Subdirectories, *(CurrentDirectory + "*"), false, true);
		presetPaths->Reset();
		for (const FString& Subdirectory : Subdirectories)
		{
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
			DirNames.Add(MakeTuple(FName(*String), static_cast<int64>(index)));
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
		for (UActorComponent* Component : ChildrenComponents)
		{
			if (Component->GetName() == HairName)
			{
				UMeshComponent* hairComponent = Cast<UMeshComponent>(Component);
				hairComponent->SetActive(false);
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
				

				static UMaterialInterface* baseMaterial = hairComponent->GetMaterial(0)->GetBaseMaterial();

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
				EObjectFlags Flags = RF_Public | RF_Standalone;
				FString targetPath = folderPath + "/Persona" + "/M_" + metahumanName + "_HAIR";
				FString targetName = "M_" + metahumanName + "_HAIR";
				UPackage* Package = CreatePackage(*targetPath);
				Package->FullyLoad();

				UMaterialInstanceDynamic* TemplateInstance = UMaterialInstanceDynamic::Create(baseMaterial, nullptr);
				hairComponent->SetMaterial(0, nullptr);
				UMaterialInstanceDynamic* InstancedMaterial = NewObject<UMaterialInstanceDynamic>(Package, *targetName, RF_Public | RF_Standalone | RF_MarkAsRootSet, TemplateInstance);
				InstancedMaterial->SetVectorParameterValue("hairDye", currentColor);
				InstancedMaterial->ForceRecompileForRendering();

				Package->FullyLoad();
				InstancedMaterial->SetFlags(Flags);
				//DynamicMaterial->Rename(*targetName, Package);
				InstancedMaterial->AddToRoot();
				FAssetRegistryModule::AssetCreated(InstancedMaterial);
				Package->MarkPackageDirty();
				FAssetRegistryModule::AssetCreated(InstancedMaterial);

				FString PackageFileName = FPackageName::LongPackageNameToFilename(targetPath, FPackageName::GetAssetPackageExtension());
				bool bSuccess = UPackage::SavePackage(Package, InstancedMaterial, Flags, *PackageFileName);
				hairComponent->SetMaterial(0, InstancedMaterial);
				hairComponent->SetActive(true);

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
	if (tRoughnessObject) 
	{
		tRoughnessObject->CompositeTexture = nTexture;
	}
	

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

					//Target Setup
					folderPath = WeakOwner.Get()->GetClass()->GetClassPathName().ToString();
					className = WeakOwner.Get()->GetClass()->GetName();
					classNamenoc = className.LeftChop(2);
					metahumanName = classNamenoc.RightChop(3);
					classSubstring = TEXT("/") + classNamenoc + TEXT(".") + className;
					indexToRemove = folderPath.Find(classSubstring);
					if (indexToRemove != INDEX_NONE) {
						folderPath = folderPath.Left(indexToRemove) + folderPath.Mid(indexToRemove + classSubstring.Len());
					}
					// Game / MetaHumans / Nicole /
					EObjectFlags Flags = RF_Public | RF_Standalone;
					FString targetPath = folderPath + "/Persona" + "/M_" + metahumanName + "_FACE";
					FString targetName = "M_" + metahumanName + "_FACE";



					UPackage* Package = CreatePackage(*targetPath);
					Package->FullyLoad();

					UMaterialInstanceDynamic* TemplateInstance = UMaterialInstanceDynamic::Create(mainMat, nullptr);
					//
					UMaterialInstanceDynamic* DynamicMaterial = NewObject<UMaterialInstanceDynamic>(Package, *targetName, RF_Public | RF_Standalone | RF_MarkAsRootSet, TemplateInstance);

					UTexture2D* tTexture = ImportAndSaveTexture(TEXT("t"));

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
					DynamicMaterial->SetFlags(Flags);
					//DynamicMaterial->Rename(*targetName, Package);
					DynamicMaterial->AddToRoot();
					FAssetRegistryModule::AssetCreated(DynamicMaterial);
					Package->MarkPackageDirty();
					FAssetRegistryModule::AssetCreated(DynamicMaterial);

					FString PackageFileName = FPackageName::LongPackageNameToFilename(targetPath, FPackageName::GetAssetPackageExtension());
					bool bSuccess = UPackage::SavePackage(Package, DynamicMaterial, Flags, *PackageFileName);
				}
			};
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Texture applied"))

}



// Sign t, /n_, /r_
UTexture2D* UPersonaComponent::ImportAndSaveTexture(FString Sign)
{
	bool rgb = false;
	if (Sign == TEXT("n")) {
		rgb = true;
	}
	Sign = TEXT("/") + Sign + TEXT("_");
	int32 PresetIndex = static_cast<int32>(currentPreset);
	//Source setup
	FString fullPath = (*presetPaths)[PresetIndex] + Sign + (*presetNames)[PresetIndex] + TEXT(".png");
	UE_LOG(LogTemp, Display, TEXT("fullAsset Path: %s"), *fullPath);

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

	FString targetPath = folderPath + "/Persona" + Sign + metahumanName;
	FString targetAbsolute = FPackageName::LongPackageNameToFilename(targetPath, FPackageName::GetAssetPackageExtension());

	FString TextureName = Sign.RightChop(1) + metahumanName;
	UE_LOG(LogTemp, Display, TEXT("target Path: %s"), *targetPath);
	
	EObjectFlags Flags = RF_Public | RF_Standalone | RF_MarkAsRootSet;

	UTexture2D* TMPTexture = NewObject<UTexture2D>();
	TArray64<uint8> Buffer;
	if (FFileHelper::LoadFileToArray(Buffer, *fullPath))
	{
		TMPTexture = FImageUtils::ImportBufferAsTexture2D(Buffer);
	}

	const uint8* FormatedImageData = static_cast<const uint8*>(TMPTexture->PlatformData->Mips[0].BulkData.LockReadOnly());
	int TextureWidth = TMPTexture->PlatformData->SizeX;
	int TextureHeight = TMPTexture->PlatformData->SizeY;
	TMPTexture->PlatformData->Mips[0].BulkData.Unlock();
	UPackage* Package = CreatePackage(NULL, *targetPath);
	Package->FullyLoad();
	UTexture2D* TargetTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	TargetTexture->AddToRoot();			
	TargetTexture->PlatformData = new FTexturePlatformData();
	TargetTexture->PlatformData->SizeX = TextureWidth;
	TargetTexture->PlatformData->SizeY = TextureHeight;
	
	TargetTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;

	FTexture2DMipMap* Mip = new(TargetTexture->PlatformData->Mips) FTexture2DMipMap();
	Mip->SizeX = TextureWidth;
	Mip->SizeY = TextureHeight;

	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(TextureWidth * TextureHeight * 4);
	Mip->BulkData.Unlock();
	FMemory::Memcpy(TextureData, FormatedImageData, sizeof(uint8) * TextureHeight * TextureWidth * 4);
	//Mip->BulkData.Unlock();
	TargetTexture->Source.Init(TextureWidth, TextureHeight, 1, 1, ETextureSourceFormat::TSF_BGRA8, FormatedImageData);
	if (rgb)
	{
		TargetTexture->SRGB = false;
	}
	else
	{
		TargetTexture->SRGB = true;
	}
	TargetTexture->UpdateResource();
	//Mip->BulkData.Unlock();
	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(TargetTexture);
	


	//FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	bool bSaved = UPackage::SavePackage(Package, TargetTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *targetAbsolute, GError, nullptr, true, true, SAVE_NoError);

	return TargetTexture;
}


