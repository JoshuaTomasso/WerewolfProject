// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAudioSettings.generated.h"

USTRUCT(BlueprintType)
struct FSAudioSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> VolumeData;

};
