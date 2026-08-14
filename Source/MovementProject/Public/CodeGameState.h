// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EPhases.h"
#include "CodeGameState.generated.h"

class ACodePlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkipVoteCountChanged);

/**
 *
 */
UCLASS()
class MOVEMENTPROJECT_API ACodeGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Phases")
	EPhases CurrentPhase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Floats")
	float PhaseTimeRemaining;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Floats")
	float PhaseDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Floats")
	float PhaseEndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Ints")
	int ExpectedPlayerCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_SkipVoteCount, Category = "Voting")
	int32 SkipVoteCount;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSkipVoteCountChanged OnSkipVoteCountChanged;

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "GameState")
	void MulticastSendFinalPlayerList();

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "GameState")
	void MulticastNotifyWinner(int WinningTeam);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_SkipVoteCount() const;
};