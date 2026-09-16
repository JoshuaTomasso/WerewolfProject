// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DisplayInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct MOVEMENTPROJECT_API FDisplayInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NativeWidth = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NativeHeight = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint MaxResolution = FIntPoint(ForceInitToZero);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPrimary = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DPI = 0;
	
	// Default Constructor
	FDisplayInfo()
	{
		Name = FString("");
		ID = FString("");
		NativeWidth = 0;
		NativeHeight = 0;
		MaxResolution = FIntPoint(ForceInitToZero);
		bIsPrimary = false;
		DPI = 0;
	}
	
	// Custom Constructor
	FDisplayInfo(const FString& InName, const FString& InID, const int32 InNativeWidth, const int32 InNativeHeight,
		const FIntPoint InMaxResolution, bool InIsPrimary, int32 InDPI)
			: Name(InName)
	, ID(InID)
	, NativeWidth(InNativeWidth)
	, NativeHeight(InNativeHeight)
	, MaxResolution(InMaxResolution)
	, bIsPrimary(InIsPrimary)
	, DPI(InDPI)
	{
	}
	
};
