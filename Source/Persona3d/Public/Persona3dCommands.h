// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Persona3dStyle.h"

class FPersona3dCommands : public TCommands<FPersona3dCommands>
{
public:

	FPersona3dCommands()
		: TCommands<FPersona3dCommands>(TEXT("Persona3d"), NSLOCTEXT("Contexts", "Persona3d", "Persona3d Plugin"), NAME_None, FPersona3dStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};