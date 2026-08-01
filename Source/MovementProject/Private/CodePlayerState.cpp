// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePlayerState.h"
#include "CodePlayerRoleDiscription.h"
#include "CodeWerewolfPartnerReveal.h"
#include "CodeGameState.h"
#include "SRoleInfo.h"
#include "Net/UnrealNetwork.h"

void ACodePlayerState::Client_ReceiveRole_Implementation(ERoles roleToReveal)
{
	APlayerController* LocalPlayerController = GetPlayerController();

	UCodePlayerRoleDiscription* RoleDiscriptionWidget = CreateWidget<UCodePlayerRoleDiscription>(LocalPlayerController, RoleDescriptionWidgetClass);
	if (RoleDiscriptionWidget)
	{
		RoleDiscriptionWidget->AddToViewport();

		if (RoleDataTable)
		{
			FString RoleNameString = UEnum::GetValueAsString(roleToReveal);
			RoleNameString.RemoveFromStart(TEXT("ERoles::"));

			FSRoleInfo* RoleInfo = RoleDataTable->FindRow<FSRoleInfo>(*RoleNameString, TEXT("Client_ReceiveRole_Implementation"));

			if (RoleInfo)
			{
				RoleDiscriptionWidget->RoleText->SetText(RoleInfo->displayName);
				RoleDiscriptionWidget->RoleDetails->SetText(RoleInfo->description);

				RoleDiscriptionWidget->RemoveWidgetAfterDelay(duration);
				
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Client_ReceiveRole_Implementation: RoleInfo is null for role %s"), *UEnum::GetValueAsString(roleToReveal));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Client_ReceiveRole_Implementation: RoleDataTable is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client_ReceiveRole_Implementation: RoleDiscriptionWidget is null"));
	}

}

void ACodePlayerState::Client_ReceiveWerewolfPartner_Implementation(const FText& partnerName)
{

	GetWorld()->GetTimerManager().SetTimer
	(
		ShowWidgetTimerHandle,
		[this, partnerName]()
		{
			OnShowWidgetTimer(partnerName);
		},
		duration,
		false
	);
}

void ACodePlayerState::Server_SubmitNightAction_Implementation(ACodePlayerState* abilityTarget)
{
	ACodeGameState* GameState = Cast<ACodeGameState>(GetWorld()->GetGameState());
	if (GameState)
	{

		if (GameState->currentPhase == EPhases::Night && bIsAlive && !bHasSubmittedNightAction && abilityTarget->bIsAlive)
		{

			bool bCanVoteFor = true;

			if (this == abilityTarget)
			{
				if (currentRole == ERoles::Medic)
				{
					if (selfProtectedCount >= 1)
					{
						voteErrorCount++;
						OnRep_VoteErrorCount();
						bCanVoteFor = false;
					}
					else if (selfProtectedCount < 1)
					{
						selfProtectedCount++;
					}
				}
			}
			else if (werewolfPartner == abilityTarget)
			{
				voteErrorCount++;
				OnRep_VoteErrorCount();
				bCanVoteFor = false;
			}

			if (currentRole == ERoles::Seer)
			{
				if (seerAbilityCount >= 1)
				{
					voteErrorCount++;
					OnRep_VoteErrorCount();
					bCanVoteFor = false;
				}
				else if (seerAbilityCount < 1)
				{
					seerAbilityCount++;
				}
			}

			

			if (bCanVoteFor)
			{
				if (RoleDataTable)
				{
					FString RoleNameString = UEnum::GetValueAsString(currentRole);
					RoleNameString.RemoveFromStart(TEXT("ERoles::"));

					FSRoleInfo* RoleInfo = RoleDataTable->FindRow<FSRoleInfo>(*RoleNameString, TEXT("Server_SubmitNightAction_Implementation"));

					if (RoleInfo)
					{
						if (RoleInfo->bCanActAtNight)
						{
							nightTarget = abilityTarget;
							bHasSubmittedNightAction = true;
							OnRep_NightTarget();
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Server_SubmitNightAction_Implementation: Player %s with role %s cannot act at night"), *GetPlayerName(), *UEnum::GetValueAsString(currentRole));
						}
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Server_SubmitNightAction_Implementation: GameState is null"));
		}
	}
}


void ACodePlayerState::Server_SubmitVote_Implementation(ACodePlayerState* abilityTarget)
{
	ACodeGameState* GameState = Cast<ACodeGameState>(GetWorld()->GetGameState());
	if (GameState)
	{

		if (GameState->currentPhase == EPhases::Voting && bIsAlive && abilityTarget->bIsAlive && !bHasSubmittedVote)
		{
			voteTarget = abilityTarget;
			bHasSubmittedVote = true;
			OnRep_VoteTarget();
			abilityTarget->votesOnPlayer++;
			abilityTarget->OnRep_VotesOnPlayer();

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Server_SubmitVote_Implementation: GameState is null"));
		}
	}
}

void ACodePlayerState::OnRep_CurrentRole()
{
	OnRoleAssigned.Broadcast();
}

void ACodePlayerState::OnRep_VoteErrorCount()
{
	OnErrorCountChanged.Broadcast();
}

void ACodePlayerState::OnRep_NightTarget()
{
	OnNightTargetChanged.Broadcast();
}

void ACodePlayerState::OnRep_TargetDeadCount()
{
	OnTargetDeadCountChanged.Broadcast();
}

void ACodePlayerState::OnRep_VoteTarget()
{
	OnVoteTargetChanged.Broadcast();
}

void ACodePlayerState::OnRep_VotesOnPlayer()
{
	OnVotesOnPlayerChanged.Broadcast();
}

void ACodePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACodePlayerState, currentRole);
	DOREPLIFETIME(ACodePlayerState, voteErrorCount);
	DOREPLIFETIME(ACodePlayerState, nightTarget);
	DOREPLIFETIME(ACodePlayerState, werewolfPartner);
	DOREPLIFETIME(ACodePlayerState, bIsAlive);
	DOREPLIFETIME(ACodePlayerState, bHasSubmittedNightAction);
	DOREPLIFETIME(ACodePlayerState, bHasSubmittedVote);
	DOREPLIFETIME(ACodePlayerState, targetDeadCount);
	DOREPLIFETIME(ACodePlayerState, votesOnPlayer);
}

void ACodePlayerState::OnShowWidgetTimer(const FText& partnerName)
{
	APlayerController* PlayerController = GetPlayerController();
	if (PlayerController)
	{
		UCodeWerewolfPartnerReveal* WerewolfPartnerRevealWidget = CreateWidget<UCodeWerewolfPartnerReveal>(PlayerController, WerewolfPartnerRevealWidgetClass);
		if (WerewolfPartnerRevealWidget)
		{
			WerewolfPartnerRevealWidget->AddToViewport();
			WerewolfPartnerRevealWidget->WerewolfNames->SetText(partnerName);
			WerewolfPartnerRevealWidget->RemoveWidgetAfterDelay(duration);
		}
	}
}
