// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGamePhaseTimer.h"
#include "CodeGameState.h"

void UCodeGamePhaseTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (ACodeGameState* GameState = GetWorld()->GetGameState<ACodeGameState>())
	{
		
		if (const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPhases")))
		{
			GamePhaseText->SetText(EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(GameState->CurrentPhase)));
		}

		const float TimePercent = FMath::Clamp((GameState->PhaseEndTime - GameState->GetServerWorldTimeSeconds()) / GameState->PhaseDuration, 0.0f, 1.0f);
		GamePhaseTimerBar->SetPercent(TimePercent);

		GamePhaseTimerBar->SetFillColorAndOpacity(
			GameState->CurrentPhase == EPhases::RoleReveal ? RoleRevealColor :
			GameState->CurrentPhase == EPhases::Lobby ? LobbyColor :
			GameState->CurrentPhase == EPhases::Night ? NightColor :
			GameState->CurrentPhase == EPhases::Day ? DayColor :
			GameState->CurrentPhase == EPhases::Voting ? VotingColor : FLinearColor::White
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameState or UI elements are null in UCodeGamePhaseTimer::NativeTick"));
	}
}

