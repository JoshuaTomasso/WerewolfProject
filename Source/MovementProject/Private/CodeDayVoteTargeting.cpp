// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeDayVoteTargeting.h"
#include "CodePlayerState.h"
#include "CodeButtonAndText.h"
#include "CodeGameState.h"
#include "CodePlayerController.h"
#include "Components/Spacer.h"

void UCodeDayVoteTargeting::NativeConstruct()
{
	InitializationRetryCount = 0;
	TryInitializePlayerState();

	SkipVoteButton->SetupSkipVoteEntry();
}

void UCodeDayVoteTargeting::PopulateTargetList()
{
	PlayerChoicesScrollBox->ClearChildren();
	
	if (AGameStateBase* GameStateBase = GetWorld()->GetGameState())
	{
		if (ACodeGameState* CodeGameState = Cast<ACodeGameState>(GameStateBase))
		{
			for (APlayerState* PlayerState : CodeGameState->PlayerArray)
			{
				if (ACodePlayerState* CodePlayerState = Cast<ACodePlayerState>(PlayerState))
				{
					if (CodePlayerState->bIsAlive)
					{
						UCodeButtonAndText* NewEntry = CreateWidget<UCodeButtonAndText>(this, ButtonAndTextWidget);
						NewEntry->SetupEntry(CodePlayerState);
						PlayerChoicesScrollBox->AddChild(NewEntry);

						USpacer* Spacer = NewObject<USpacer>(this);
						Spacer->SetSize(FVector2D(EntrySpacing, EntrySpacing));
						PlayerChoicesScrollBox->AddChild(Spacer);
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PopulateTargetList: GameState is null"));
	}
}

void UCodeDayVoteTargeting::RefreshPlayerNameText()
{
	if (const APlayerController* OwningController = GetOwningPlayer())
	{
		if (const ACodePlayerState* PlayerState = Cast<ACodePlayerState>(OwningController->PlayerState))
		{
			if (PlayerNameText)
			{
				PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
			}
		}
	}
}

void UCodeDayVoteTargeting::TryInitializePlayerState()
{
	if (const APlayerController* OwningController = GetOwningPlayer())
	{
		if (OwningController->PlayerState)
		{
			if (ACodePlayerState* PlayerState = Cast<ACodePlayerState>(OwningController->PlayerState))
			{
				if (PlayerNameText)
				{
					PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
				}
				
				PlayerState->OnDisplayNameChanged.RemoveDynamic(this, &UCodeDayVoteTargeting::RefreshPlayerNameText);
				PlayerState->OnDisplayNameChanged.AddDynamic(this, &UCodeDayVoteTargeting::RefreshPlayerNameText);
			}
		}
		else if (InitializationRetryCount < 50)
		{
			InitializationRetryCount++;
			GetWorld()->GetTimerManager().SetTimer(
				InitializationTimerHandle,
				this,
				&UCodeDayVoteTargeting::TryInitializePlayerState,
				0.1f,
				false
			);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TryInitializePlayerState: Failed to initialize after 50 attempts, giving up"));
		}
	}
}
