// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeButtonAndText.h"
#include "CodePlayerState.h"
#include "CodeGameState.h"

void UCodeButtonAndText::NativeConstruct()
{
	if (PlayerNameButton)
	{
		PlayerNameButton->OnPressed.AddDynamic(this, &UCodeButtonAndText::OnButtonPressed);
	}
}

void UCodeButtonAndText::NativeDestruct()
{
	if (targetPlayerState)
	{
		targetPlayerState->OnVotesOnPlayerChanged.RemoveDynamic(this, &UCodeButtonAndText::UpdateVoteCountDisplay);
	}
}

void UCodeButtonAndText::SetupEntry(ACodePlayerState* PlayerReference)
{
	targetPlayerState = PlayerReference;
	
	if (targetPlayerState)
	{
		targetPlayerState->OnVotesOnPlayerChanged.RemoveDynamic(this, &UCodeButtonAndText::UpdateVoteCountDisplay);
		targetPlayerState->OnVotesOnPlayerChanged.AddDynamic(this, &UCodeButtonAndText::UpdateVoteCountDisplay);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupEntry: targetPlayerState is null"));
	}
	
	UpdateVoteCountDisplay();
}

void UCodeButtonAndText::OnButtonPressed()
{
	ACodePlayerState* playerState = Cast<ACodePlayerState>(GetOwningPlayer()->PlayerState);

	if (GetWorld()->GetGameState<ACodeGameState>()->currentPhase == EPhases::Night)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnButtonPressed NIGHT: Player %s pressed button for target %s"), *playerState->GetPlayerName(), *targetPlayerState->GetPlayerName());
		if (playerState)
		{
			playerState->Server_SubmitNightAction(targetPlayerState);
		}
	}
	else if (GetWorld()->GetGameState<ACodeGameState>()->currentPhase == EPhases::Voting)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnButtonPressed VOTING: Player %s pressed button for target %s"), *playerState->GetPlayerName(), *targetPlayerState->GetPlayerName());
		if (playerState)
		{
			playerState->Server_SubmitVote(targetPlayerState);
		}
	}
}
void UCodeButtonAndText::UpdateVoteCountDisplay()
{
	if (!targetPlayerState || !PlayerNameText)
	{
		return;
	}

	if (targetPlayerState->votesOnPlayer > 0)
	{
		PlayerNameText->SetText(FText::FromString(FString::Printf(TEXT("%s (%d)"), *targetPlayerState->GetPlayerName(), targetPlayerState->votesOnPlayer)));
	}
	else
	{
		PlayerNameText->SetText(FText::FromString(targetPlayerState->GetPlayerName()));
	}
}
