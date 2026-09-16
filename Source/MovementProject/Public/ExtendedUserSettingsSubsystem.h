// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DisplayInfo.h"
#include "ExtendedUserSettingsSubsystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MOVEMENTPROJECT_API UExtendedUserSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION()
	TArray<FDisplayInfo> GetAllDisplayInfo() const;
	
	UFUNCTION()
	void SetActiveDisplayByID(const FString& DisplayID);
	
};
