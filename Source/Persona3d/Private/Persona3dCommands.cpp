// Copyright Epic Games, Inc. All Rights Reserved.

#include "Persona3dCommands.h"

#define LOCTEXT_NAMESPACE "FPersona3dModule"

void FPersona3dCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Persona3d", "Bring up Persona3d window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
