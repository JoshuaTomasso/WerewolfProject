// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePlayerController.h"
#include "CodeGameMode.h"
#include "CodeGamePhaseTimer.h"

void ACodePlayerController::Server_NotifyReady_Implementation()
{
	ACodeGameMode* GameMode = Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode)
	{
		GameMode->NotifyPlayerReady(this);
	}
}

void ACodePlayerController::BeginPlay()
{
	if (IsLocalController())
	{
		UCodeGamePhaseTimer* GamePhaseTimerWidget = CreateWidget<UCodeGamePhaseTimer>(this, UCodeGamePhaseTimer::StaticClass());
		GamePhaseTimerWidget->AddToViewport();

	}
}

void ACodePlayerController::Tick(float DeltaTime)
{
	if (IsLocalController())
	{
		if (!bHasAckedReady)
		{
			ACodeGameState* GameState = Cast<ACodeGameState>(GetWorld()->GetGameState());
			if (GameState->ExpectedPlayerCount > 0 && GameState->PlayerArray.Num() >= GameState->ExpectedPlayerCount)
			{
				Server_NotifyReady();
				bHasAckedReady = true;
			}
		}
	}
}
