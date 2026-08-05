// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPhases.h"
#include "CodePlayerController.generated.h"

class UCodeGamePhaseTimer;
class UCodeNightActionTargeting;
class UCodeDayVoteTargeting;
class UCodeGameOver;
class UCodeNightResult;
class UCodeRevealeRole;

/**
 *
 */
UCLASS()
class MOVEMENTPROJECT_API ACodePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeGamePhaseTimer> GamePhaseTimerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeNightActionTargeting> NightActionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeDayVoteTargeting> DayVoteWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeGameOver> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeNightResult> NightResultWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeRevealeRole> RevealeRoleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeGamePhaseTimer* gamePhaseTimerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeNightActionTargeting* nightActionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeDayVoteTargeting* dayVoteWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeNightResult* nightResultWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeGameOver* gameOverWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeRevealeRole* revealeRoleWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bNightWidgetActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bHasAckedReady;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bNightTargetListPopulated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bDayTargetListPopulated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phases")
	EPhases LastKnownPhase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	class UDataTable* ControllerRoleDataTable;

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PlayerController")
	void Server_NotifyReady();

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "PlayerController")
	void Client_SetNightResultText(const FString& ResultText);

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void ShowGameOverWidget(FString WinningFaction);

};