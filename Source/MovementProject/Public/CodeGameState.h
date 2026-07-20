// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "EPhases.h"

#include "CodeGameState.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API ACodeGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phases")
	EPhases currentPhase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float phaseTimeRemaining;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float phaseDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float phaseEndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ints")
	int ExpectedPlayerCount;
	
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "GameState")
	void MulticastSendFinalPlayerList();

};
