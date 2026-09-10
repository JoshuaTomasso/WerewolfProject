// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "SSoundConfigs.generated.h"

USTRUCT(BlueprintType)
struct FSSoundConfigs : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundClass* SoundClass; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundMix* SoundMix;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultVolume;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SoundClassName;
};
