// Fill out your copyright notice in the Description page of Project Settings.

#include "CodeGameMode.h"
#include "Containers/List.h"

void ACodeGameMode::BeginPlay()
{
	ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>());
	if (CurrentGameState)
	{
		CurrentGameState->ExpectedPlayerCount = expectedPlayerCount;
	}
}

void ACodeGameMode::OnPhaseTimerComplete()
{
	ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>());
	if (!CurrentGameState)
	{
		return;
	}

	switch (CurrentGameState->currentPhase)
	{
	case EPhases::Lobby:
		StartPhase(EPhases::Night);
		break;
	case EPhases::Night:
		StartPhase(EPhases::Day);
		break;
	case EPhases::Day:
		ResolveNightActions();
		StartPhase(EPhases::Voting);
		break;
	case EPhases::Voting:
		StartPhase(EPhases::Night);
		break;
	default:
		break;
	}
}

void ACodeGameMode::AssignRoles()
{
	playerStates.Empty();

	for (APlayerState* PlayerState : GetGameState<AGameStateBase>()->PlayerArray)
	{
		ACodePlayerState* CodePlayerState = Cast<ACodePlayerState>(PlayerState);
		if (CodePlayerState)
		{
			playerStates.Add(CodePlayerState);
		}
	}

	rolePool.Empty();

	rolePool.Add(ERoles::Werewolf);
	rolePool.Add(ERoles::Werewolf);
	rolePool.Add(ERoles::Medic);
	rolePool.Add(ERoles::Seer);

	for (int32 i = rolePool.Num() - 1; i > 0; --i)
	{
		int32 RandomIndex = FMath::RandRange(0, i);
		rolePool.Swap(i, RandomIndex);
	}

	for (int i = 0; i < playerStates.Num() - 1; ++i)
	{
		playerStates[i]->currentRole = rolePool[i];

		if (playerStates[i]->currentRole == ERoles::Werewolf)
		{
			werewolves.Add(playerStates[i]);
			if (werewolves.Num() == 2)
			{
				werewolves[0]->werewolfPartner = werewolves[1];
				werewolves[1]->werewolfPartner = werewolves[0];
			}
		}
	}

	for (ACodePlayerState* PlayerState : werewolves)
	{
		partnerNameText = FText::FromString(TEXT(""));

		for (ACodePlayerState* Werewolf : werewolves)
		{
			if (PlayerState != Werewolf)
			{
				if (!partnerNameText.IsEmpty())
				{
					partnerNameText = FText::Format(FText::FromString(TEXT("{0}, {1}")), partnerNameText, FText::FromString(Werewolf->GetPlayerName()));
				}
				else
				{
					partnerNameText = FText::FromString(Werewolf->GetPlayerName());
				}
			}
		}
		// Client Reveal Partner Name
	}

	werewolfPacks.Empty();

	for (int i = 0; i < werewolves.Num() - 1; i++)
	{
		if (i % 2 == 0)
		{
			if (i + 1 < werewolves.Num())
			{
				FSWerewolfPack NewPack;
				NewPack.wolfOne = werewolves[i];
				NewPack.wolfTwo = werewolves[i + 1];
				werewolfPacks.Add(NewPack);
			}
			else
			{
				FSWerewolfPack NewPack;
				NewPack.wolfOne = werewolves[i];
				NewPack.wolfTwo = nullptr;
				werewolfPacks.Add(NewPack);
			}
		}
	}
}


void ACodeGameMode::StartPhase(EPhases NewPhase)
{
	ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>());
	if (!CurrentGameState)
	{
		return;
	}

	CurrentGameState->currentPhase = NewPhase;
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	switch (CurrentGameState->currentPhase)
	{
	case EPhases::Lobby:
		CurrentGameState->phaseTimeRemaining = 0.0f;
		CurrentGameState->phaseDuration = CurrentGameState->phaseTimeRemaining;
		CurrentGameState->phaseEndTime = CurrentTime + CurrentGameState->phaseDuration;
		break;
	case EPhases::Night:
		CurrentGameState->phaseTimeRemaining = nightDuration;
		CurrentGameState->phaseDuration = CurrentGameState->phaseTimeRemaining;
		CurrentGameState->phaseEndTime = CurrentTime + CurrentGameState->phaseDuration;
		for (ACodePlayerState* PlayerState : playerStates)
		{
			if (PlayerState)
			{
				PlayerState->bHasSubmittedNightAction = false;
				PlayerState->nightTarget = nullptr;
			}
		}
		break;
	case EPhases::Day:
		CurrentGameState->phaseTimeRemaining = dayDuration;
		CurrentGameState->phaseDuration = CurrentGameState->phaseTimeRemaining;
		CurrentGameState->phaseEndTime = CurrentTime + CurrentGameState->phaseDuration;
		break;
	case EPhases::Voting:
		CurrentGameState->phaseTimeRemaining = votingDuration;
		CurrentGameState->phaseDuration = CurrentGameState->phaseTimeRemaining;
		CurrentGameState->phaseEndTime = CurrentTime + CurrentGameState->phaseDuration;
		break;
	default:
		break;
	}

	GetWorld()->GetTimerManager().ClearTimer(phaseTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		phaseTimerHandle,
		this,
		&ACodeGameMode::OnPhaseTimerComplete,
		CurrentGameState->phaseTimeRemaining,
		false
	);
}

void ACodeGameMode::NotifyPlayerReady(ACodePlayerController* Controller)
{
	if (!readyPlayerControllers.Contains(Controller))
	{
		readyPlayerControllers.Add(Controller);
		if (readyPlayerControllers.Num() >= expectedPlayerCount && !bGameStarted)
		{
			ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>());
			CurrentGameState->MulticastSendFinalPlayerList();

			AssignRoles();
			StartPhase(EPhases::Night);
			bGameStarted = true;
		}
	}
}

void ACodeGameMode::ResolveNightActions()
{
}