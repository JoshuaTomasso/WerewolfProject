// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CodePlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodePlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	FString PlayerName;
	
};
