// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Interfaces/IPluginManager.h"



#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Engine/GameEngine.h"
#include "EngineGlobals.h"
#include "SlateCore.h"
#include "Widgets/SWidget.h"

#include "Misc/Paths.h"
//
//#include "SlateOptMacros.h"
#include "Widgets/Input/SEditableTextBox.h"
//
#include "Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "Framework/Application/SlateApplication.h"

/**
 * 
 */
class PERSONA3D_API SMyCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMyCompoundWidget)
	{}
	SLATE_END_ARGS()
		//aditionalfuncs
		FReply OnPersonaButtonClicked();
		FReply OnPathButtonClicked();
		TArray<FString> outFileNames;
		FString windowName = FString("aa");
		FString fileType = FString(".png");
		FString startPath = FString("C:/");
		TSharedPtr<SEditableTextBox> pathTextBox;
		TSharedPtr<SEditableTextBox> ipTextBox;
		TSharedPtr<SEditableTextBox> nameTextBox;
		




	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void PersonaOpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames);

};