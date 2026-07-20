// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGamePhaseTimer.h"
#include "CodeGameState.h"

void UCodeGamePhaseTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ACodeGameState* GameState = GetWorld()->GetGameState<ACodeGameState>();

	if (GameState && GamePhaseText && GamePhaseTimerBar)
	{
		GamePhaseText->SetText(FText::FromString(UEnum::GetValueAsString(GameState->currentPhase)));
		GamePhaseTimerBar->SetPercent(FMath::Clamp((GameState->phaseEndTime - GetWorld()->GetTimeSeconds()) / GameState->phaseDuration, 0.0f, 1.0f));
		GamePhaseTimerBar->SetFillColorAndOpacity(
			GameState->currentPhase == EPhases::Lobby ? lobbyColor :
			GameState->currentPhase == EPhases::Night ? nightColor :
			GameState->currentPhase == EPhases::Day ? dayColor :
			GameState->currentPhase == EPhases::Voting ? votingColor : FLinearColor::White
		);
	}
}