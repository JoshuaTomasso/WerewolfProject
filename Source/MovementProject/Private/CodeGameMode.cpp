// Fill out your copyright notice in the Description page of Project Settings.

#include "CodeGameMode.h"
#include "Containers/List.h"
#include "SRoleInfo.h"
#include "CodeGameState.h"
#include "Kismet/GameplayStatics.h"

void ACodeGameMode::BeginPlay()
{
	if (ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>()))
	{
		CurrentGameState->ExpectedPlayerCount = ExpectedPlayerCount;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay: CurrentGameState is null"));
	}
}

void ACodeGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	if (UGameplayStatics::HasOption(Options, TEXT("MaxPlayers")))
	{
		const int32 OptionPlayerCount = UGameplayStatics::GetIntOption(Options, TEXT("MaxPlayers"), ExpectedPlayerCount);
		if (OptionPlayerCount > 0)
		{
			ExpectedPlayerCount = OptionPlayerCount;
		}
	}
}

ACodeGameMode::ACodeGameMode()
{
	WerewolfRatio = 0.25f;
	MinWerewolves = 1;
	MinPlayersToStart = 4;
	
	SpecialRoleMinPlayers.Add(ERoles::Medic, 4);
	SpecialRoleMinPlayers.Add(ERoles::Seer, 5);
	SpecialRoleMinPlayers.Add(ERoles::Mayor, 6);
}

void ACodeGameMode::OnPhaseTimerComplete()
{
	if (bGameOver)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnPhaseTimerComplete: Game is over, cannot proceed to next phase"));
		return;
	}
	
	if (const ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>()))
	{
		switch (CurrentGameState->CurrentPhase)
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
}

void ACodeGameMode::AssignRoles()
{
	PlayerStates.Empty();

	for (APlayerState* PlayerState : GetGameState<AGameStateBase>()->PlayerArray)
	{
		if (ACodePlayerState* CodePlayerState = Cast<ACodePlayerState>(PlayerState))
		{
			PlayerStates.Add(CodePlayerState);
		}
	}
	
	const int32 PlayerCount = PlayerStates.Num();
	
	if (PlayerCount < FMath::Max(MinPlayersToStart, MinWerewolves + 1))
	{
		UE_LOG(LogTemp, Error, TEXT("AssignRoles: Not enough players (%d) to assign roles."), PlayerCount);
		return;
	}

	RolePool.Empty();
	
	int32 NumWerewolves = FMath::Max(MinWerewolves, FMath::RoundToInt(PlayerCount * WerewolfRatio));
	
	int32 MaxWerewolves = FMath::Max(MinWerewolves, (PlayerCount - 1) / 2);
	NumWerewolves = FMath::Clamp(NumWerewolves, MinWerewolves, MaxWerewolves);

	for (int32 i = 0; i < NumWerewolves; i++)
	{
		RolePool.Add(ERoles::Werewolf);
	}
	
	int32 RemainingSlots = PlayerCount - RolePool.Num();
	for (const TPair<ERoles, int32>& SpecialRole : SpecialRoleMinPlayers)
	{
		if (RemainingSlots <= 0)
		{
			break;
		}
		
		if (PlayerCount >= SpecialRole.Value)
		{
			RolePool.Add(SpecialRole.Key);
			--RemainingSlots;
		}
	}
	
	while (RolePool.Num() < PlayerCount)
	{
		RolePool.Add(ERoles::Villager);
	}
	
	if (RolePool.Num() > PlayerCount)
	{
		RolePool.SetNum(PlayerCount);
	}
	
	for (int32 i = RolePool.Num() - 1; i > 0; --i)
	{
		const int32 RandomIndex = FMath::RandRange(0, i);
		RolePool.Swap(i, RandomIndex);
	}
	
	Werewolves.Empty();
	
	for (int i = 0; i <= PlayerStates.Num() - 1; ++i)
	{
		PlayerStates[i]->CurrentRole = RolePool[i];

		PlayerStates[i]->Client_ReceiveRole(PlayerStates[i]->CurrentRole);

		if (PlayerStates[i]->CurrentRole == ERoles::Werewolf)
		{
			Werewolves.Add(PlayerStates[i]);
		}
	}

	for (ACodePlayerState* PlayerState : Werewolves)
	{
		PartnerNameText = FText::FromString(TEXT(""));

		for (const ACodePlayerState* Werewolf : Werewolves)
		{
			if (PlayerState != Werewolf)
			{
				if (!PartnerNameText.IsEmpty())
				{
					PartnerNameText = FText::Format(FText::FromString(TEXT("{0}, {1}")), PartnerNameText, FText::FromString(Werewolf->GetPlayerName()));
				}
				else
				{
					PartnerNameText = FText::FromString(Werewolf->GetPlayerName());
				}
			}
		}
		if (!PartnerNameText.IsEmpty())
		{
			PlayerState->Client_ReceiveWerewolfPartner(PartnerNameText);
		}
	}

	WerewolfPacks.Empty();
	for (ACodePlayerState* PlayerState : PlayerStates)
	{
		if (PlayerState)
		{
			PlayerState->WerewolfPartner = nullptr;
		}
	}

	for (int i = 0; i < Werewolves.Num(); i += 2)
	{
		FSWerewolfPack NewPack;
		NewPack.WolfOne = Werewolves[i];
		NewPack.WolfTwo = (i + 1 <Werewolves.Num()) ? Werewolves[i + 1] : nullptr;

		ACodePlayerState* WolfOnePlayerState = Cast<ACodePlayerState>(NewPack.WolfOne);
		ACodePlayerState* WolfTwoPlayerState = Cast<ACodePlayerState>(NewPack.WolfTwo);

		if (WolfOnePlayerState && WolfTwoPlayerState)
		{
			WolfOnePlayerState->WerewolfPartner = WolfTwoPlayerState;
			WolfTwoPlayerState->WerewolfPartner = WolfOnePlayerState;
			WolfOnePlayerState->OnRep_WerewolfPartner();
			WolfTwoPlayerState->OnRep_WerewolfPartner();
		}

		WerewolfPacks.Add(NewPack);
	}

	for (ACodePlayerState* PlayerState : PlayerStates)
	{
		if (PlayerState)
		{
			PlayerState->OnRep_CurrentRole();
		}
	}
}


void ACodeGameMode::StartPhase(const EPhases NewPhase)
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

	CurrentGameState->CurrentPhase = NewPhase;
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	switch (CurrentGameState->CurrentPhase)
	{
	case EPhases::RoleReveal:
		CurrentGameState->PhaseTimeRemaining = RoleRevealDuration;
		CurrentGameState->PhaseDuration = CurrentGameState->PhaseTimeRemaining;
		CurrentGameState->PhaseEndTime = CurrentTime + CurrentGameState->PhaseDuration;
		break;
	case EPhases::Lobby:
		CurrentGameState->PhaseTimeRemaining = 0.0f;
		CurrentGameState->PhaseDuration = CurrentGameState->PhaseTimeRemaining;
		CurrentGameState->PhaseEndTime = CurrentTime + CurrentGameState->PhaseDuration;
		break;
	case EPhases::Night:
		CurrentGameState->PhaseTimeRemaining = NightDuration;
		CurrentGameState->PhaseDuration = CurrentGameState->PhaseTimeRemaining;
		CurrentGameState->PhaseEndTime = CurrentTime + CurrentGameState->PhaseDuration;
		for (ACodePlayerState* PlayerState : PlayerStates)
		{
			if (PlayerState)
			{
				PlayerState->bHasSubmittedNightAction = false;
				PlayerState->NightTarget = nullptr;
				PlayerState->bIsProtected = false;
				PlayerState->VotesOnPlayer = 0;
				PlayerState->OnRep_VotesOnPlayer();
				PlayerState->OnRep_NightTarget();
			}
		}
		break;
	case EPhases::Day:
		CurrentGameState->PhaseTimeRemaining = DayDuration;
		CurrentGameState->PhaseDuration = CurrentGameState->PhaseTimeRemaining;
		CurrentGameState->PhaseEndTime = CurrentTime + CurrentGameState->PhaseDuration;
		for (ACodePlayerState* PlayerState : PlayerStates)
		{
			if (PlayerState)
			{
				PlayerState->bHasSubmittedVote = false;
				PlayerState->VoteTarget = nullptr;
				PlayerState->VotesOnPlayer = 0;
				PlayerState->OnRep_VoteTarget();
				PlayerState->OnRep_VotesOnPlayer();
				CurrentGameState->SkipVoteCount = 0;
				CurrentGameState->OnRep_SkipVoteCount();
			}
		}
		break;
	case EPhases::Voting:
		CurrentGameState->PhaseTimeRemaining = VotingDuration;
		CurrentGameState->PhaseDuration = CurrentGameState->PhaseTimeRemaining;
		CurrentGameState->PhaseEndTime = CurrentTime + CurrentGameState->PhaseDuration;

		break;
	default:
		break;
	}

	GetWorld()->GetTimerManager().ClearTimer(PhaseTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		PhaseTimerHandle,
		this,
		&ACodeGameMode::OnPhaseTimerComplete,
		CurrentGameState->PhaseTimeRemaining,
		false
	);
}

void ACodeGameMode::NotifyPlayerReady(ACodePlayerController* Controller)
{
	if (!ReadyPlayerControllers.Contains(Controller))
	{
		ReadyPlayerControllers.Add(Controller);
		if (ReadyPlayerControllers.Num() >= ExpectedPlayerCount && !bGameStarted)
		{
			if (ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>()))
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
	KillTargetArray.Empty();
	KillTarget = nullptr;

	for (const ACodePlayerState* Werewolf : Werewolves)
	{
		if (Werewolf && Werewolf->NightTarget && Werewolf->NightTarget->bIsAlive)
		{
			KillTargetArray.Add(Werewolf->NightTarget);
		}
	}

	if (KillTargetArray.Num() > 0)
	{
		bool bAllAgree = true;
		for (const ACodePlayerState* Target : KillTargetArray)
		{
			if (Target != KillTargetArray[0])
			{
				bAllAgree = false;
				break;
			}
		}

		if (bAllAgree)
		{
			KillTarget = KillTargetArray[0];
		}
		else
		{
			const int32 RandomIndex = FMath::RandRange(0, KillTargetArray.Num() - 1);
			KillTarget = KillTargetArray[RandomIndex];
		}
	}

	for (ACodePlayerState* PlayerState : PlayerStates)
	{
		if (!PlayerState || !PlayerState->NightTarget)
		{
			continue;
		}

		FString TargetRoleNameString = UEnum::GetValueAsString(PlayerState->NightTarget->CurrentRole);
		TargetRoleNameString.RemoveFromStart(TEXT("ERoles::"));

		const FSRoleInfo* RoleInfo = GameModeRoleDataTable->FindRow<FSRoleInfo>(*TargetRoleNameString, TEXT("PlayerControllerTick"));

		if (PlayerState->CurrentRole == ERoles::Medic)
		{
			ProtectionTarget = PlayerState->NightTarget;
			if (PlayerState == ProtectionTarget && PlayerState->SelfProtectedCount < 3)
			{
				PlayerState->SelfProtectedCount++;
				ProtectionTarget->bIsProtected = true;
				const FString PlayerName = PlayerState->GetPlayerName();
				UE_LOG(LogTemp, Warning, TEXT("ResolveNightActions: Medic %s protected themselves."), *PlayerName);
			}
			else if (ProtectionTarget != PlayerState)
			{
				ProtectionTarget->bIsProtected = true;
				UE_LOG(LogTemp, Warning, TEXT("ResolveNightActions: %s was protected by the Medic."), *ProtectionTarget->GetPlayerName());
			}
		}
		else if (PlayerState->CurrentRole == ERoles::Seer)
		{
			SeererTarget = PlayerState->NightTarget;
			if (SeererTarget)
			{
				if (RoleInfo)
				{
					FString RoleTeam = UEnum::GetValueAsString(RoleInfo->Team);
					RoleTeam.RemoveFromStart(TEXT("ETeams::"));
					UE_LOG(LogTemp, Warning, TEXT("ResolveNightActions: %s's team is %s."), *SeererTarget->GetPlayerName(), *RoleTeam);

					if (ACodePlayerController* SeerController = Cast<ACodePlayerController>(PlayerState->GetOwner()))
					{
						SeerController->Client_SetNightResultText(RoleTeam);
					}
				}
			}
		}
	}

	if (KillTarget && KillTarget->bIsProtected)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResolveNightActions: %s was protected and survived the night."), *KillTarget->GetPlayerName());
		KillTarget->bIsProtected = false;
		KillTarget = nullptr;
		KillTargetArray.Empty();
	}
	else if (KillTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResolveNightActions: %s was killed during the night."), *KillTarget->GetPlayerName());
		KillTarget->bIsAlive = false;
		KillTarget = nullptr;
		KillTargetArray.Empty();
	}
}

void ACodeGameMode::ResolveVotes()
{
	for (const ACodePlayerState* PlayerState : PlayerStates)
	{
		if (!PlayerState || !PlayerState->bIsAlive)
		{
			continue;
		}

		if (PlayerState && PlayerState->bIsAlive && PlayerState->CurrentRole == ERoles::Mayor)
		{
			if (PlayerState->bHasRevealedRole && PlayerState->VoteTarget)
			{
				UE_LOG(LogTemp, Warning, TEXT("ResolveVotes: Mayor %s voted for %s."), *PlayerState->GetPlayerName(), *PlayerState->VoteTarget->GetPlayerName());
				PlayerState->VoteTarget->VotesOnPlayer++;
				PlayerState->VoteTarget->OnRep_VotesOnPlayer();
			}
		}

	}

	ACodePlayerState* PlayerWithMostVotes = nullptr;
	int32 HighestVoteCount = -1;
	bool bTiedForFirst = false;


	for (ACodePlayerState* PlayerState : PlayerStates)
	{
		if (!PlayerState || !PlayerState->bIsAlive)
		{
			continue;
		}

		if (const ACodeGameState* CurrentGameState = Cast<ACodeGameState>(GetGameState<ACodeGameState>()))
		{
			if (PlayerState->VotesOnPlayer > HighestVoteCount && PlayerState->VotesOnPlayer > CurrentGameState->SkipVoteCount)
			{
				HighestVoteCount = PlayerState->VotesOnPlayer;
				PlayerWithMostVotes = PlayerState;
				bTiedForFirst = false;
			}
			else if (PlayerState->VotesOnPlayer == HighestVoteCount)
			{
				bTiedForFirst = true;
			}
		}
	}

	if (PlayerWithMostVotes && HighestVoteCount > 0 && !bTiedForFirst)
	{
		PlayerWithMostVotes->bIsAlive = false;
		UE_LOG(LogTemp, Warning, TEXT("ResolveVotes: %s was voted out."), *PlayerWithMostVotes->GetPlayerName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ResolveVotes: No player was voted out due to a tie or no votes."));
	}
}

void ACodeGameMode::CheckWinConditions()
{
	VillagerCount = 0;
	WerewolfCount = 0;

	for (const ACodePlayerState* PlayerState : PlayerStates)
	{
		if (!PlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("CheckWinConditions: PlayerState is null"));
			continue;
		}

		FString TargetRoleNameString = UEnum::GetValueAsString(PlayerState->CurrentRole);
		TargetRoleNameString.RemoveFromStart(TEXT("ERoles::"));

		if (const FSRoleInfo* RoleInfo = GameModeRoleDataTable->FindRow<FSRoleInfo>(*TargetRoleNameString, TEXT("CheckWinConditions")))
		{
			if (RoleInfo->Team == ETeams::Villagers && PlayerState->bIsAlive)
			{
				VillagerCount++;
			}
			else if (RoleInfo->Team == ETeams::Werewolves && PlayerState->bIsAlive)
			{
				WerewolfCount++;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CheckWinConditions: RoleInfo is null for player %s with role %s"), *PlayerState->GetPlayerName(), *TargetRoleNameString);
		}
	}

	if (WerewolfCount == 0)
	{
		WinningTeam = 1; // Villagers win
		bGameOver = true;
	}
	else if (VillagerCount < WerewolfCount)
	{
		WinningTeam = 2; // Werewolves win
		bGameOver = true;
	}
	else
	{
		WinningTeam = -1; // No winner yet
	}
	
	if (ACodeGameState* MyGameState = GetGameState<ACodeGameState>())
	{
		MyGameState->MulticastNotifyWinner(WinningTeam);

		if (bGameOver)
		{
			GetWorld()->GetTimerManager().ClearTimer(PhaseTimerHandle);
			MyGameState->PhaseTimeRemaining = 0.0f;
			MyGameState->PhaseDuration = 0.0f;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckWinConditions: MyGameState is null : Final Check"));
	}
}


