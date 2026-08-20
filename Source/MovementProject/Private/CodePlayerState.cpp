// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePlayerState.h"
#include "CodePlayerRoleDiscription.h"
#include "CodeWerewolfPartnerReveal.h"
#include "CodeGameState.h"
#include "SRoleInfo.h"
#include "Net/UnrealNetwork.h"
#include "CodePlayerCharacter.h"

void ACodePlayerState::Client_ReceiveRole_Implementation(ERoles RoleToReveal)
{
	APlayerController* LocalPlayerController = GetPlayerController();

	
	if (UCodePlayerRoleDiscription* RoleDescriptionWidget = CreateWidget<UCodePlayerRoleDiscription>(LocalPlayerController, RoleDescriptionWidgetClass))
	{
		RoleDescriptionWidget->AddToViewport();

		if (RoleDataTable)
		{
			FString RoleNameString = UEnum::GetValueAsString(RoleToReveal);
			RoleNameString.RemoveFromStart(TEXT("ERoles::"));

			

			if (const FSRoleInfo* RoleInfo = RoleDataTable->FindRow<FSRoleInfo>(*RoleNameString, TEXT("Client_ReceiveRole_Implementation")))
			{
				RoleDescriptionWidget->RoleText->SetText(RoleInfo->DisplayName);
				RoleDescriptionWidget->RoleDetails->SetText(RoleInfo->Description);

				RoleDescriptionWidget->RemoveWidgetAfterDelay(Duration);
				
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Client_ReceiveRole_Implementation: RoleInfo is null for role %s"), *UEnum::GetValueAsString(RoleToReveal));
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

void ACodePlayerState::Client_ReceiveWerewolfPartner_Implementation(const FText& PartnerName)
{

	GetWorld()->GetTimerManager().SetTimer
	(
		ShowWidgetTimerHandle,
		[this, PartnerName]()
		{
			OnShowWidgetTimer(PartnerName);
		},
		Duration,
		false
	);
}

void ACodePlayerState::Server_SubmitNightAction_Implementation(ACodePlayerState* AbilityTarget)
{
	
	if (const ACodeGameState* GameState = Cast<ACodeGameState>(GetWorld()->GetGameState()))
	{

		if (GameState->CurrentPhase == EPhases::Night && bIsAlive && !bHasSubmittedNightAction && AbilityTarget->bIsAlive)
		{

			bool bCanVoteFor = true;

			if (this == AbilityTarget)
			{
				if (CurrentRole == ERoles::Medic)
				{
					if (SelfProtectedCount >= 1)
					{
						VoteErrorCount++;
						OnRep_VoteErrorCount();
						bCanVoteFor = false;
					}
					else if (SelfProtectedCount < 1)
					{
						SelfProtectedCount++;
					}
				}
			}
			else if (WerewolfPartner == AbilityTarget)
			{
				VoteErrorCount++;
				OnRep_VoteErrorCount();
				bCanVoteFor = false;
			}

			if (CurrentRole == ERoles::Seer)
			{
				if (SeerAbilityCount >= 1)
				{
					VoteErrorCount++;
					OnRep_VoteErrorCount();
					bCanVoteFor = false;
				}
				else if (SeerAbilityCount < 1)
				{
					SeerAbilityCount++;
				}
			}

			

			if (bCanVoteFor)
			{
				if (RoleDataTable)
				{
					FString RoleNameString = UEnum::GetValueAsString(CurrentRole);
					RoleNameString.RemoveFromStart(TEXT("ERoles::"));

					

					if (const FSRoleInfo* RoleInfo = RoleDataTable->FindRow<FSRoleInfo>(*RoleNameString, TEXT("Server_SubmitNightAction_Implementation")))
					{
						if (RoleInfo->bCanActAtNight)
						{
							NightTarget = AbilityTarget;
							bHasSubmittedNightAction = true;
							OnRep_NightTarget();
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Server_SubmitNightAction_Implementation: Player %s with role %s cannot act at night"), *GetPlayerName(), *UEnum::GetValueAsString(CurrentRole));
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


void ACodePlayerState::Server_SubmitVote_Implementation(ACodePlayerState* AbilityTarget)
{
	if (ACodeGameState* GameState = Cast<ACodeGameState>(GetWorld()->GetGameState()))
	{
		if (AbilityTarget == nullptr)
		{
			VoteTarget = nullptr;
			bHasSubmittedVote = true;
			OnRep_VoteTarget();

			GameState->SkipVoteCount++;
			GameState->OnRep_SkipVoteCount();
		}
		else if (GameState->CurrentPhase == EPhases::Voting && bIsAlive && AbilityTarget->bIsAlive && !bHasSubmittedVote)
		{
			VoteTarget = AbilityTarget;
			bHasSubmittedVote = true;
			OnRep_VoteTarget();
			AbilityTarget->VotesOnPlayer++;
			AbilityTarget->OnRep_VotesOnPlayer();

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Server_SubmitVote_Implementation: GameState is null"));
		}
	}
}

void ACodePlayerState::Server_RevealRole_Implementation()
{
	bHasRevealedRole = true;
	OnRep_HasRevealedRole();
	
	if (ACodePlayerCharacter* PlayerCharacter = Cast<ACodePlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Multicast_SetMayorRevealVisibility(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server_RevealeRole_Implementation: PlayerCharacter is null"));
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

void ACodePlayerState::OnRep_HasRevealedRole()
{
	OnHasRevealedRoleChanged.Broadcast();
}

void ACodePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACodePlayerState, CurrentRole);
	DOREPLIFETIME(ACodePlayerState, VoteErrorCount);
	DOREPLIFETIME(ACodePlayerState, NightTarget);
	DOREPLIFETIME(ACodePlayerState, WerewolfPartner);
	DOREPLIFETIME(ACodePlayerState, bIsAlive);
	DOREPLIFETIME(ACodePlayerState, bHasSubmittedNightAction);
	DOREPLIFETIME(ACodePlayerState, bHasSubmittedVote);
	DOREPLIFETIME(ACodePlayerState, TargetDeadCount);
	DOREPLIFETIME(ACodePlayerState, VotesOnPlayer);
	DOREPLIFETIME(ACodePlayerState, bHasRevealedRole);
}

void ACodePlayerState::OnShowWidgetTimer(const FText& partnerName)
{
	if (APlayerController* PlayerController = GetPlayerController())
	{
		if (UCodeWerewolfPartnerReveal* WerewolfPartnerRevealWidget = CreateWidget<UCodeWerewolfPartnerReveal>(PlayerController, WerewolfPartnerRevealWidgetClass))
		{
			WerewolfPartnerRevealWidget->AddToViewport();
			WerewolfPartnerRevealWidget->WerewolfNames->SetText(partnerName);
			WerewolfPartnerRevealWidget->RemoveWidgetAfterDelay(Duration);
		}
	}
}
