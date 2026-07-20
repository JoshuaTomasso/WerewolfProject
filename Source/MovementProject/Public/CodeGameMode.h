// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include <vector>
#include <string>
#include "CodeGameState.h"
#include "ERoles.h"
#include "SWerewolfPack.h"
#include "CodePlayerController.h"
#include "CodePlayerState.h"

#include "CodeGameMode.generated.h"

/**
 *
 */
UCLASS()
class MOVEMENTPROJECT_API ACodeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<ERoles> rolePool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<ACodePlayerState*> playerStates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<ACodePlayerState*> werewolves;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<ACodePlayerController*> readyPlayerControllers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<ACodePlayerState*> killTargetArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<FSWerewolfPack> werewolfPacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float nightDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float dayDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float votingDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ints")
	int expectedPlayerCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bGameStarted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bAValid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bBValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	ACodePlayerState* killTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	FTimerHandle phaseTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	FText partnerNameText;

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void OnPhaseTimerComplete();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void AssignRoles();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void StartPhase(EPhases NewPhase);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void NotifyPlayerReady(ACodePlayerController* Controller);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void ResolveNightActions();
};