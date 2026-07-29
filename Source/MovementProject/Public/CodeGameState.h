// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EPhases.h"
#include "CodeGameState.generated.h"

class ACodePlayerState;

/**
 *
 */
UCLASS()
class MOVEMENTPROJECT_API ACodeGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Phases")
	EPhases currentPhase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Floats")
	float phaseTimeRemaining;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Floats")
	float phaseDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Floats")
	float phaseEndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Ints")
	int ExpectedPlayerCount;

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "GameState")
	void MulticastSendFinalPlayerList();

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "GameState")
	void MulticastNotifyWinner(int winningTeam);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};