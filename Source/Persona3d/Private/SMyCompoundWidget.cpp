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
            .Text(FText::FromString("Personificate"))
        .OnClicked(FOnClicked::CreateSP(this, &SMyCompoundWidget::OnPersonaButtonClicked))
        ]
    + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        .HAlign(HAlign_Center)
        [
            SNew(SButton)
            .Text(FText::FromString("Import"))
        .OnClicked(FOnClicked::CreateSP(this, &SMyCompoundWidget::OnImportButtonClicked))
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

    FString PythonScript = FullPluginPath + FString(TEXT("/Source/python/main.py"));
    FString Parameters = FString(TEXT("-content \"")) + FullPluginPath + FString(TEXT("\" -ip ")) + ipString + FString(TEXT(" -path \"")) + pathString + FString(TEXT("\" -name \"")) + nameString + FString(TEXT("\""));
    FString FullCommand = FString::Printf(TEXT("py \"%s\" %s"), *PythonScript, *Parameters);
    FString editorPath = FPaths::ConvertRelativePathToFull(FPlatformProcess::BaseDir());
    FString pythonExec = FString(TEXT("/Engine/Binaries/ThirdParty/Python3/Win64/python.exe"));
    pythonExec = editorPath.LeftChop(23) + pythonExec;
 


    FGuid UniqueId = FGuid::NewGuid();
    FString UniqueIdString = UniqueId.ToString(EGuidFormats::DigitsWithHyphensInBraces);
    FString ShortUniqueIdString = UniqueIdString.RightChop(2).Left(6);

    PersonName = nameString + TEXT("_") + ShortUniqueIdString;
    

    FString params = FullPluginPath + FString(TEXT("/Source/python/main_nonue.py -content \"")) + FullPluginPath + FString(TEXT("\" -ip ")) + ipString + FString(TEXT(" -path \"")) + pathString + FString(TEXT("\" -name \"")) + PersonName + FString(TEXT("\" -ppath \"")) + pythonExec + FString(TEXT("\""));
    FString clientCommandPath = pythonExec + FString(TEXT(" ")) + FullPluginPath + FString(TEXT("/Source/python/main_nonue.py -content \"")) + FullPluginPath + FString(TEXT("\" -ip ")) + ipString + FString(TEXT(" -path \"")) + pathString + FString(TEXT("\" -name \"")) + PersonName + FString(TEXT("\" -ppath \"")) + pythonExec + FString(TEXT("\""));
    clientCommandPath = clientCommandPath.Replace(TEXT("/"), TEXT("\\"));
    UE_LOG(LogTemp, Display, TEXT("Command reformated: %s"), *clientCommandPath);
    FProcHandle ProcessHandle = FPlatformProcess::CreateProc(*pythonExec, *params, true, true, false, nullptr, 0, nullptr, nullptr);
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

FReply SMyCompoundWidget::OnImportButtonClicked()
{

    UE_LOG(LogTemp, Display, TEXT("Improt"));
    FString PluginDir = IPluginManager::Get().FindPlugin("Persona3d")->GetBaseDir();
    FString FullPluginPath = FPaths::ConvertRelativePathToFull(PluginDir);
    FString commandPath = (FString(TEXT("py ")) + PluginDir + FString(TEXT("/Source/python/main_ue.py -content \"")) + FullPluginPath + FString(TEXT("\" -name \"")) + PersonName + FString(TEXT("\"")));

    const TCHAR* commandTchar = *commandPath;
    GEngine->Exec(NULL, commandTchar);  
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
