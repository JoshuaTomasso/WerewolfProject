// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CodeMainMenuPanelOrder.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECodeMainMenuPanelOrder : uint8
{
	MainButtons,
	PlayGame,
	SettingsPanel,
	SoundSettings,
	ControlSettings,
	GraphicsSettings,
	DisplaySettings
};
