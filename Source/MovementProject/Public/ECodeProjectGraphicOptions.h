// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECodeProjectGraphicOptions.generated.h"

UENUM(BlueprintType)
enum class ECodeProjectGraphicOptions : uint8
{
	Overall						UMETA(DisplayName = "Overall"),
	ViewDistance				UMETA(DisplayName = "View Distance"),
	AntiAliasingQuality 		UMETA(DisplayName = "Anti-aliasing Quality"),
	ShadowQuality				UMETA(DisplayName = "Shadow Quality"),
	GlobalIlluminationQuality   UMETA(DisplayName = "Global Illumination Quality"),
	ReflectionQuality			UMETA(DisplayName = "Reflection Quality"),
	PostProcessQuality			UMETA(DisplayName = "Post Process Quality"),
	TextureQuality				UMETA(DisplayName = "Texture Quality"),
	EffectsQuality				UMETA(DisplayName = "Effects Quality"),
	FoliageQuality				UMETA(DisplayName = "Foliage Quality"),
	ShadingQuality				UMETA(DisplayName = "Shading Quality"),
};