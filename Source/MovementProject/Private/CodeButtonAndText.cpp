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
	if (TargetPlayerState)
	{
		TargetPlayerState->OnVotesOnPlayerChanged.RemoveDynamic(this, &UCodeButtonAndText::UpdateVoteCountDisplay);
		TargetPlayerState->OnDisplayNameChanged.RemoveDynamic(this, &UCodeButtonAndText::UpdateVoteCountDisplay);
	}
	
	if (ACodeGameState* GameState = GetWorld() ? GetWorld()->GetGameState<ACodeGameState>() : nullptr)
	{
		GameState->OnSkipVoteCountChanged.RemoveDynamic(this, &UCodeButtonAndText::UpdateSkipVoteCountDisplay);
	}
}

void UCodeButtonAndText::SetupEntry(ACodePlayerState* PlayerReference)
{
	PlayerNameButton->SetBackgroundColor(BaseButtonColor);
	TargetPlayerState = PlayerReference;
	
	if (TargetPlayerState)
	{
		TargetPlayerState->OnVotesOnPlayerChanged.RemoveDynamic(this, &UCodeButtonAndText::UpdateVoteCountDisplay);
		TargetPlayerState->OnVotesOnPlayerChanged.AddDynamic(this, &UCodeButtonAndText::UpdateVoteCountDisplay);
		TargetPlayerState->OnDisplayNameChanged.RemoveDynamic(this, &UCodeButtonAndText::UpdateVoteCountDisplay);
		TargetPlayerState->OnDisplayNameChanged.AddDynamic(this, &UCodeButtonAndText::UpdateVoteCountDisplay);
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
	TargetPlayerState = nullptr;
	
	if (ACodeGameState* GameState = GetWorld()->GetGameState<ACodeGameState>())
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

	if (GetWorld()->GetGameState<ACodeGameState>()->CurrentPhase == EPhases::Night)
	{
		if (playerState && TargetPlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnButtonPressed NIGHT: Player %s pressed button for target %s"), *playerState->GetPlayerName(), *TargetPlayerState->GetPlayerName());
			playerState->Server_SubmitNightAction(TargetPlayerState);
		}
	}
	else if (GetWorld()->GetGameState<ACodeGameState>()->CurrentPhase == EPhases::Voting)
	{
		if (playerState && TargetPlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnButtonPressed VOTING: Player %s pressed button for target %s"), *playerState->GetPlayerName(), *TargetPlayerState->GetPlayerName());
			playerState->Server_SubmitVote(TargetPlayerState);
		}
		else if (playerState && TargetPlayerState == nullptr && !playerState->bHasSubmittedVote)
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
	if (!TargetPlayerState || !PlayerNameText)
	{
		return;
	}

	if (TargetPlayerState->VotesOnPlayer > 0)
	{
		PlayerNameText->SetText(FText::FromString(FString::Printf(TEXT("%s (%d)"), *TargetPlayerState->GetPlayerName(), TargetPlayerState->VotesOnPlayer)));
	}
	else
	{
		PlayerNameText->SetText(FText::FromString(TargetPlayerState->GetPlayerName()));
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
