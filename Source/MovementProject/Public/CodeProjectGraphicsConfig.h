// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ECodeProjectGraphicOptions.h"
#include "CodeProjectGraphicsConfig.generated.h"


USTRUCT(BlueprintType)
struct FCodeProjectGraphicsConfig : public FTableRowBase
{
	
	GENERATED_BODY()
	
	FCodeProjectGraphicsConfig()
		: GraphicsOptionType(ECodeProjectGraphicOptions::Overall)
		, DefaultOptionIndex(0)
	{
		Options.Add(0, FText::FromString(TEXT("Low")));
		Options.Add(1, FText::FromString(TEXT("Medium")));
		Options.Add(2, FText::FromString(TEXT("High")));
		Options.Add(3, FText::FromString(TEXT("Epic")));
		Options.Add(4, FText::FromString(TEXT("Cinematic")));
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECodeProjectGraphicOptions GraphicsOptionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FText> Options;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DefaultOptionIndex;
};
