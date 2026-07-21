// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePlayerController.h"
#include "CodeGameMode.h"
#include "CodeGamePhaseTimer.h"
#include "CodeGameState.h"

void ACodePlayerController::Server_NotifyReady_Implementation()
{
	ACodeGameMode* GameMode = Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Notifying GameMode that player is ready"));
		GameMode->NotifyPlayerReady(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server_NotifyReady: GameMode is null"));
	}
}

void ACodePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		UCodeGamePhaseTimer* GamePhaseTimerWidget = CreateWidget<UCodeGamePhaseTimer>(this, GamePhaseTimerWidgetClass);
		if (GamePhaseTimerWidget)
		{
			GamePhaseTimerWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create GamePhaseTimerWidget"));
		}
	}
}

void ACodePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsLocalController())
	{
		if (!bHasAckedReady)
		{
			ACodeGameState* GameState = Cast<ACodeGameState>(GetWorld()->GetGameState());
			if (GameState)
			{
				if (GameState->ExpectedPlayerCount > 0 && GameState->PlayerArray.Num() >= GameState->ExpectedPlayerCount)
				{
					Server_NotifyReady();
					bHasAckedReady = true;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Tick: GameState is null"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tick: Not a local controller"));
	}
}