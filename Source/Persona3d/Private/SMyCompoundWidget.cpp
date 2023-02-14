// Fill out your copyright notice in the Description page of Project Settings.


#include "SMyCompoundWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMyCompoundWidget::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SNew(STextBlock)
            .Text(FText::FromString("IP of Persona Machine:"))
        ]
    + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SAssignNew(ipTextBox, SEditableTextBox)
            .Text(FText::FromString("52.91.135.204"))
        ]
        ]
    + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SNew(STextBlock)
            .Text(FText::FromString("Character Name"))
        ]
    + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SAssignNew(nameTextBox, SEditableTextBox)
            .Text(FText::FromString("Persona"))

        ]
        ]
    + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SNew(STextBlock)
            .Text(FText::FromString("Concept path"))
        ]
    + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SAssignNew(pathTextBox, SEditableTextBox)
            .Text(FText::FromString("D:/"))
            
        ]
    + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SNew(SButton)
            .Text(FText::FromString("..."))
        .OnClicked(FOnClicked::CreateSP(this, &SMyCompoundWidget::OnPathButtonClicked))
        ]
        ]
    + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        .HAlign(HAlign_Center)
        [
            SNew(SButton)
            .Text(FText::FromString("Persona Full"))
        .OnClicked(FOnClicked::CreateSP(this, &SMyCompoundWidget::OnPersonaButtonClicked))
        ]
        ]
        ];
}
;
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SMyCompoundWidget::OnPersonaButtonClicked()
{
    FString PluginDir = IPluginManager::Get().FindPlugin("Persona3d")->GetBaseDir();
    FString FullPluginPath = FPaths::ConvertRelativePathToFull(PluginDir);

    FString pathString = pathTextBox->GetText().ToString();
    FString ipString = ipTextBox->GetText().ToString();
    FString nameString = nameTextBox->GetText().ToString();

    FString commandPath = (FString(TEXT("py ")) + PluginDir + FString(TEXT("/Source/python/main.py -content \"")) + FullPluginPath + FString(TEXT("\" -ip ")) + ipString + FString(TEXT(" -path \"")) + pathString + FString(TEXT("\" -name \"")) + nameString + FString(TEXT("\"")));
   
    const TCHAR* commandTchar = *commandPath;
    GEngine->Exec(NULL, commandTchar);
    return FReply::Handled();
}

FReply SMyCompoundWidget::OnPathButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("PathButtonClicked"));
    SMyCompoundWidget::PersonaOpenFileDialog(windowName, startPath, fileType, outFileNames);
    FString FullPathInput = FPaths::ConvertRelativePathToFull(outFileNames[0]);
    outFileNames.Empty();
    pathTextBox->SetText(FText::FromString(FullPathInput));
    return FReply::Handled();
}

void SMyCompoundWidget::PersonaOpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames) {
    void* ParentWindowPtr = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (DesktopPlatform)
    {
        uint32 SelectionFlag = 1; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
        DesktopPlatform->OpenFileDialog(ParentWindowPtr, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNames);
    }
}



