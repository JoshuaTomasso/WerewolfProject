// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SAudioSettings.h"
#include "CodeUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeUserSettings : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sound Settings")
	FSAudioSettings AudioSettings;
	
};
