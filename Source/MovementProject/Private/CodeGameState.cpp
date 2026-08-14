// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGameState.h"
#include "CodePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "CodePlayerState.h"
#include "CodeNightActionTargeting.h"

void ACodeGameState::MulticastSendFinalPlayerList_Implementation()
{
	if (const ACodePlayerController* LocalPlayerController = Cast<ACodePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		LocalPlayerController->NightActionWidget->PopulateTargetList();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MulticastSendFinalPlayerList: LocalPlayerController is null"));
	}
}

void ACodeGameState::MulticastNotifyWinner_Implementation(int WinningTeam)
{
	if (ACodePlayerController* LocalPlayerController = Cast<ACodePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (WinningTeam == 1)
		{
			LocalPlayerController->ShowGameOverWidget(TEXT("Winning Team: Villagers"));
		}
		else if (WinningTeam == 2)
		{
			LocalPlayerController->ShowGameOverWidget(TEXT("Winning Team: Werewolves"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MulticastSendFinalPlayerList: LocalPlayerController is null"));
	}
}

void ACodeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACodeGameState, CurrentPhase);
	DOREPLIFETIME(ACodeGameState, PhaseTimeRemaining);
	DOREPLIFETIME(ACodeGameState, PhaseDuration);
	DOREPLIFETIME(ACodeGameState, PhaseEndTime);
	DOREPLIFETIME(ACodeGameState, ExpectedPlayerCount);
	DOREPLIFETIME(ACodeGameState, SkipVoteCount);
}

void ACodeGameState::OnRep_SkipVoteCount() const
{
	OnSkipVoteCountChanged.Broadcast();
}
