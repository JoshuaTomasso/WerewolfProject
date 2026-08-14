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
	TSubclassOf<UCodeRevealeRole> RevealRoleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeGamePhaseTimer* GamePhaseTimerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeNightActionTargeting* NightActionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeDayVoteTargeting* DayVoteWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeNightResult* NightResultWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeGameOver* GameOverWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeRevealeRole* RevealRoleWidget;

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

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PlayerController")
	void Server_NotifyReady();

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "PlayerController")
	void Client_SetNightResultText(const FString& ResultText);

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void ShowGameOverWidget(FString WinningFaction);

};