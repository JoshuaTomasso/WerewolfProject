// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGameState.h"
#include "CodePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "CodePlayerState.h"
#include "CodeNightActionTargeting.h"

void ACodeGameState::MulticastSendFinalPlayerList_Implementation()
{
	ACodePlayerController* LocalPlayerController = Cast<ACodePlayerController>(GetWorld()->GetFirstPlayerController());
	if (LocalPlayerController)
	{
		LocalPlayerController->nightActionWidget->PopulateTargetList();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MulticastSendFinalPlayerList: LocalPlayerController is null"));
	}
}

void ACodeGameState::MulticastNotifyWinner_Implementation(int winningTeam)
{
	UE_LOG(LogTemp, Log, TEXT("MulticastNotifyWinner: Winning Team is %d"), winningTeam);
	ACodePlayerController* LocalPlayerController = Cast<ACodePlayerController>(GetWorld()->GetFirstPlayerController());
	if (LocalPlayerController)
	{
		// Show a message to the player about the winning team
		if (winningTeam == 1)
		{
			LocalPlayerController->ShowGameOverWidget(TEXT("Winning Team: Villagers"));
		}
		else if (winningTeam == 2)
		{
			LocalPlayerController->ShowGameOverWidget(TEXT("Winning Team: Werewolves"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Winning Team: None Yet"));
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

	DOREPLIFETIME(ACodeGameState, currentPhase);
	DOREPLIFETIME(ACodeGameState, phaseTimeRemaining);
	DOREPLIFETIME(ACodeGameState, phaseDuration);
	DOREPLIFETIME(ACodeGameState, phaseEndTime);
	DOREPLIFETIME(ACodeGameState, ExpectedPlayerCount);
}