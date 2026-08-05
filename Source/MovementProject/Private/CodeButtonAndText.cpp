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

	ACodeGameState* GameState = GetWorld() ? GetWorld()->GetGameState<ACodeGameState>() : nullptr;
	if (GameState)
	{
		GameState->OnSkipVoteCountChanged.RemoveDynamic(this, &UCodeButtonAndText::UpdateSkipVoteCountDisplay);
	}
}

void UCodeButtonAndText::SetupEntry(ACodePlayerState* PlayerReference)
{
	PlayerNameButton->SetBackgroundColor(BaseButtonColor);
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

void UCodeButtonAndText::SetupSkipVoteEntry()
{
	PlayerNameButton->SetBackgroundColor(BaseButtonColor);
	targetPlayerState = nullptr;

	ACodeGameState* GameState = GetWorld()->GetGameState<ACodeGameState>();
	if (GameState)
	{
		GameState->OnSkipVoteCountChanged.RemoveDynamic(this, &UCodeButtonAndText::UpdateSkipVoteCountDisplay);
		GameState->OnSkipVoteCountChanged.AddDynamic(this, &UCodeButtonAndText::UpdateSkipVoteCountDisplay);
	}

	PlayerNameText->SetText(FText::FromString(TEXT("Skip Vote")));
	UpdateSkipVoteCountDisplay();
}

void UCodeButtonAndText::OnButtonPressed()
{
	PlayerNameButton->SetBackgroundColor(ButtonClickedColor);
	ACodePlayerState* playerState = Cast<ACodePlayerState>(GetOwningPlayer()->PlayerState);

	if (GetWorld()->GetGameState<ACodeGameState>()->currentPhase == EPhases::Night)
	{
		if (playerState && targetPlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnButtonPressed NIGHT: Player %s pressed button for target %s"), *playerState->GetPlayerName(), *targetPlayerState->GetPlayerName());
			playerState->Server_SubmitNightAction(targetPlayerState);
		}
	}
	else if (GetWorld()->GetGameState<ACodeGameState>()->currentPhase == EPhases::Voting)
	{
		if (playerState && targetPlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnButtonPressed VOTING: Player %s pressed button for target %s"), *playerState->GetPlayerName(), *targetPlayerState->GetPlayerName());
			playerState->Server_SubmitVote(targetPlayerState);
		}
		else if (playerState && targetPlayerState == nullptr && !playerState->bHasSubmittedVote)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnButtonPressed VOTING: Player %s pressed button for SKIP VOTE"), *playerState->GetPlayerName());
			playerState->Server_SubmitVote(nullptr);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		ButtonResetTimerHandle,
		this,
		&UCodeButtonAndText::ResetButtonColor, 
		ButtonResetDelay, 
		false);
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

void UCodeButtonAndText::UpdateSkipVoteCountDisplay()
{
	ACodeGameState* GameState = GetWorld()->GetGameState<ACodeGameState>();
	if (!GameState || !PlayerNameText)
	{
		return;
	}

	if (GameState->SkipVoteCount > 0)
	{
		PlayerNameText->SetText(FText::FromString(FString::Printf(TEXT("Skip Vote (%d)"), GameState->SkipVoteCount)));
	}
	else
	{
		PlayerNameText->SetText(FText::FromString(TEXT("Skip Vote")));
	}
}

void UCodeButtonAndText::ResetButtonColor()
{
	PlayerNameButton->SetBackgroundColor(BaseButtonColor);
}
