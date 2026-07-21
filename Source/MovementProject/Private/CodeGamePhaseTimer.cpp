// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGamePhaseTimer.h"
#include "CodeGameState.h"

void UCodeGamePhaseTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ACodeGameState* GameState = GetWorld()->GetGameState<ACodeGameState>();

	if (GameState && GamePhaseText && GamePhaseTimerBar)
	{
		UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPhases"));
		if (EnumPtr)
		{
			GamePhaseText->SetText(EnumPtr->GetDisplayNameTextByValue((int64)GameState->currentPhase));
		}

		float TimePercent = FMath::Clamp((GameState->phaseEndTime - GetWorld()->GetTimeSeconds()) / GameState->phaseDuration, 0.0f, 1.0f);
		GamePhaseTimerBar->SetPercent(TimePercent);

		GamePhaseTimerBar->SetFillColorAndOpacity(
			GameState->currentPhase == EPhases::Lobby ? lobbyColor :
			GameState->currentPhase == EPhases::Night ? nightColor :
			GameState->currentPhase == EPhases::Day ? dayColor :
			GameState->currentPhase == EPhases::Voting ? votingColor : FLinearColor::White
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameState or UI elements are null in UCodeGamePhaseTimer::NativeTick"));
	}
}