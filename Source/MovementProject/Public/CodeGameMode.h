// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
	ACodeGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<ERoles> RolePool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<ACodePlayerState*> PlayerStates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<ACodePlayerState*> Werewolves;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<ACodePlayerController*> ReadyPlayerControllers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<ACodePlayerState*> KillTargetArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectors")
	TArray<FSWerewolfPack> WerewolfPacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float NightDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float DayDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float VotingDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float RoleRevealDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ints")
	int ExpectedPlayerCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bGameStarted;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoleSetup")
	float WerewolfRatio = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoleSetup")
	int32 MinWerewolves = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoleSetup")
	int32 MinPlayersToStart = 4;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoleSetup")
	TMap<ERoles, int32> SpecialRoleMinPlayers;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bAValid = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bBValid = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	ACodePlayerState* KillTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	ACodePlayerState* ProtectionTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	ACodePlayerState* SeererTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	FTimerHandle PhaseTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	FText PartnerNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	class UDataTable* GameModeRoleDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WinConditions")
	int WinningTeam = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WinConditions")
	int VillagerCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WinConditions")
	int WerewolfCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WinConditions")
	bool bGameOver = false;

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

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void ResolveVotes();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void CheckWinConditions();

protected:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

};