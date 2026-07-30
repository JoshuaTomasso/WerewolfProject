// Fill out your copyright notice in the Description page of Project Settings.

#include "CodeGameMode.h"
#include "Containers/List.h"
#include "SRoleInfo.h"


void ACodeGameMode::BeginPlay()
{
	ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>());
	if (CurrentGameState)
	{
		CurrentGameState->ExpectedPlayerCount = expectedPlayerCount;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay: CurrentGameState is null"));
	}
}

void ACodeGameMode::OnPhaseTimerComplete()
{
	if (bGameOver)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnPhaseTimerComplete: Game is over, cannot proceed to next phase"));
		return;
	}

	ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>());
	if (!CurrentGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnPhaseTimerComplete: CurrentGameState is null"));
		return;
	}

	switch (CurrentGameState->currentPhase)
	{
	case EPhases::RoleReveal:
		StartPhase(EPhases::Night);
		break;
	case EPhases::Lobby:
		StartPhase(EPhases::Night);
		break;
	case EPhases::Night:
		ResolveNightActions();
		CheckWinConditions();
		StartPhase(EPhases::Day);
		break;
	case EPhases::Day:
		CheckWinConditions();
		StartPhase(EPhases::Voting);
		break;
	case EPhases::Voting:
		ResolveVotes();
		CheckWinConditions();
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

	for (int i = 0; i <= playerStates.Num() - 1; ++i)
	{
		playerStates[i]->currentRole = rolePool[i];

		playerStates[i]->Client_ReceiveRole(playerStates[i]->currentRole);

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
		PlayerState->Client_ReceiveWerewolfPartner(partnerNameText);
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

	for (ACodePlayerState* PlayerState : playerStates)
	{
		if (PlayerState)
		{
			PlayerState->OnRep_CurrentRole();
		}
	}
}


void ACodeGameMode::StartPhase(EPhases NewPhase)
{
	if (bGameOver)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartPhase: Game is over, cannot start new phase"));
		return;
	}

	ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>());
	if (!CurrentGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartPhase: CurrentGameState is null"));
		return;
	}

	CurrentGameState->currentPhase = NewPhase;
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	switch (CurrentGameState->currentPhase)
	{
	case EPhases::RoleReveal:
		CurrentGameState->phaseTimeRemaining = roleRevealDuration;
		CurrentGameState->phaseDuration = CurrentGameState->phaseTimeRemaining;
		CurrentGameState->phaseEndTime = CurrentTime + CurrentGameState->phaseDuration;
		break;
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
				PlayerState->bIsProtected = false;
				PlayerState->votesOnPlayer = 0;
				PlayerState->OnRep_VotesOnPlayer();
				PlayerState->OnRep_NightTarget();
			}
		}
		break;
	case EPhases::Day:
		CurrentGameState->phaseTimeRemaining = dayDuration;
		CurrentGameState->phaseDuration = CurrentGameState->phaseTimeRemaining;
		CurrentGameState->phaseEndTime = CurrentTime + CurrentGameState->phaseDuration;
		for (ACodePlayerState* PlayerState : playerStates)
		{
			if (PlayerState)
			{
				PlayerState->bHasSubmittedVote = false;
				PlayerState->voteTarget = nullptr;
				PlayerState->votesOnPlayer = 0;
				PlayerState->OnRep_VoteTarget();
				PlayerState->OnRep_VotesOnPlayer();
			}
		}
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
			if (CurrentGameState)
			{
				CurrentGameState->MulticastSendFinalPlayerList();
				AssignRoles();
				StartPhase(EPhases::RoleReveal);
				bGameStarted = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("NotifyPlayerReady: CurrentGameState is null"));
			}
		}
	}
}

void ACodeGameMode::ResolveNightActions()
{
	if (werewolves[0]->nightTarget && werewolves[0]->nightTarget->bIsAlive)
	{
		bAValid = true;
		killTargetArray.Add(werewolves[0]->nightTarget);
	}
	if (werewolves[1]->nightTarget && werewolves[1]->nightTarget->bIsAlive)
	{
		bBValid = true;
		killTargetArray.Add(werewolves[1]->nightTarget);
	}

	if (bAValid && bBValid)
	{
		if (werewolves[0]->nightTarget == werewolves[1]->nightTarget)
		{
			killTarget = werewolves[0]->nightTarget;
		}
		else
		{
			int32 RandomIndex = FMath::RandRange(0, killTargetArray.Num() - 1);
			killTarget = killTargetArray[RandomIndex];
		}
	}
	else if (bAValid)
	{
		killTarget = werewolves[0]->nightTarget;
	}
	else if (bBValid)
	{
		killTarget = werewolves[1]->nightTarget;
	}
	else
	{
		killTarget = nullptr;
	}
	for (ACodePlayerState* PlayerState : playerStates)
	{
		if (!PlayerState || !PlayerState->nightTarget)
		{
			continue;
		}

		FString TargetRoleNameString = UEnum::GetValueAsString(PlayerState->nightTarget->currentRole);
		TargetRoleNameString.RemoveFromStart(TEXT("ERoles::"));

		FSRoleInfo* RoleInfo = GameModeRoleDataTable->FindRow<FSRoleInfo>(*TargetRoleNameString, TEXT("PlayerControllerTick"));


		if (PlayerState->currentRole == ERoles::Medic)
		{
			protectionTarget = PlayerState->nightTarget;
			if (PlayerState == protectionTarget && PlayerState->selfProtectedCount < 3)
			{
				PlayerState->selfProtectedCount++;
				protectionTarget->bIsProtected = true;
				UE_LOG(LogTemp, Error, TEXT("ResolveNightActions: Medic %s protected themselves."), *PlayerState->GetPlayerName());
			}
			else if (protectionTarget != PlayerState)
			{
				protectionTarget->bIsProtected = true;
				UE_LOG(LogTemp, Error, TEXT("ResolveNightActions: %s was protected by the Medic."), *protectionTarget->GetPlayerName());
			}
		}
		else if (PlayerState->currentRole == ERoles::Seer)
		{
			seererTarget = PlayerState->nightTarget;
			if (seererTarget)
			{
				if (RoleInfo)
				{
					FString RoleTeam = UEnum::GetValueAsString(RoleInfo->team);
					RoleTeam.RemoveFromStart(TEXT("ETeams::"));
					UE_LOG(LogTemp, Error, TEXT("ResolveNightActions: Seer %s targeted %s."), *PlayerState->GetPlayerName(), *seererTarget->GetPlayerName());
					UE_LOG(LogTemp, Error, TEXT("ResolveNightActions: %s's team is %s."), *seererTarget->GetPlayerName(), *RoleTeam);
				}
			}
		}

	}
	if (killTarget && killTarget->bIsProtected)
	{
		UE_LOG(LogTemp, Error, TEXT("ResolveNightActions: %s was protected and survived the night."), *killTarget->GetPlayerName());
		killTarget->bIsProtected = false;
		killTarget = nullptr;
		killTargetArray.Empty();
	}
	else if (killTarget)
	{
		UE_LOG(LogTemp, Error, TEXT("ResolveNightActions: %s was killed during the night."), *killTarget->GetPlayerName());
		killTarget->bIsAlive = false;
		killTarget = nullptr;
		killTargetArray.Empty();
	}
}

void ACodeGameMode::ResolveVotes()
{
	ACodePlayerState* PlayerWithMostVotes = nullptr;
	for (ACodePlayerState* PlayerState : playerStates)
	{
		if (!PlayerState || !PlayerState->bIsAlive)
		{
			continue;
		}
		if (!PlayerWithMostVotes || PlayerState->votesOnPlayer > PlayerWithMostVotes->votesOnPlayer)
		{
			PlayerWithMostVotes = PlayerState;
		}
		else if (PlayerState->votesOnPlayer == PlayerWithMostVotes->votesOnPlayer)
		{
			PlayerWithMostVotes = nullptr;
		}
	}

	if (PlayerWithMostVotes)
	{
		PlayerWithMostVotes->bIsAlive = false;
		UE_LOG(LogTemp, Error, TEXT("ResolveVotes: %s was voted out."), *PlayerWithMostVotes->GetPlayerName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ResolveVotes: No player was voted out due to a tie or no votes."));
	}
}

void ACodeGameMode::CheckWinConditions()
{
	villagerCount = 0;
	werewolfCount = 0;

	for (ACodePlayerState* PlayerState : playerStates)
	{
		if (!PlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("CheckWinConditions: PlayerState is null"));
			continue;
		}

		FString TargetRoleNameString = UEnum::GetValueAsString(PlayerState->currentRole);
		TargetRoleNameString.RemoveFromStart(TEXT("ERoles::"));

		FSRoleInfo* RoleInfo = GameModeRoleDataTable->FindRow<FSRoleInfo>(*TargetRoleNameString, TEXT("CheckWinConditions"));

		if (RoleInfo)
		{
			if (RoleInfo->team == ETeams::Villagers && PlayerState->bIsAlive)
			{
				villagerCount++;
			}
			else if (RoleInfo->team == ETeams::Werewolves && PlayerState->bIsAlive)
			{
				werewolfCount++;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CheckWinConditions: RoleInfo is null for player %s with role %s"), *PlayerState->GetPlayerName(), *TargetRoleNameString);
		}
	}

	if (werewolfCount == 0)
	{
		winningTeam = 1; // Villagers win
		bGameOver = true;
	}
	else if (villagerCount < werewolfCount)
	{
		winningTeam = 2; // Werewolves win
		bGameOver = true;
	}
	else
	{
		winningTeam = -1; // No winner yet
	}

	ACodeGameState* MyGameState = GetGameState<ACodeGameState>();
	if (MyGameState)
	{
		MyGameState->MulticastNotifyWinner(winningTeam);

		if (bGameOver)
		{
			GetWorld()->GetTimerManager().ClearTimer(phaseTimerHandle);
			MyGameState->phaseTimeRemaining = 0.0f;
			MyGameState->phaseDuration = 0.0f;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckWinConditions: MyGameState is null : Final Check"));
	}

}


