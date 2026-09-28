// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECodePauseMenuPanelOrder.Generated.h"

UENUM(BlueprintType)
enum class ECodePauseMenuPanelOrder : uint8
{
	PauseMenu,
	SettingsPanel,
	SoundPanel,
	ControlPanel,
	GraphicsPanel,
	DisplayPanel
};
