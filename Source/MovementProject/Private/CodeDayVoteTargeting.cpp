// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeDayVoteTargeting.h"
#include "CodePlayerState.h"
#include "CodeButtonAndText.h"
#include "CodeGameState.h"
#include "CodePlayerController.h"

void UCodeDayVoteTargeting::NativeConstruct()
{
	InitializationRetryCount = 0;
	TryInitializePlayerState();
}

void UCodeDayVoteTargeting::PopulateTargetList()
{
	PlayerChoicesScrollBox->ClearChildren();

	AGameStateBase* GameStateBase = GetWorld()->GetGameState();
	if (GameStateBase)
	{
		ACodeGameState* CodeGameState = Cast<ACodeGameState>(GameStateBase);
		if (CodeGameState)
		{
			for (APlayerState* PlayerState : CodeGameState->PlayerArray)
			{
				ACodePlayerState* CodePlayerState = Cast<ACodePlayerState>(PlayerState);
				if (CodePlayerState)
				{

					if (CodePlayerState->bIsAlive)
					{
						UCodeButtonAndText* NewEntry = CreateWidget<UCodeButtonAndText>(this, ButtonAndTextWidget);
						NewEntry->SetupEntry(CodePlayerState);
						PlayerChoicesScrollBox->AddChild(NewEntry);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PopulateTargetList: GameState is not ACodeGameState"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PopulateTargetList: GameState is null"));
	}
}

void UCodeDayVoteTargeting::TryInitializePlayerState()
{
	APlayerController* OwningController = GetOwningPlayer();
	if (!OwningController)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryInitializePlayerState: No owning controller"));
		return;
	}

	if (OwningController->PlayerState)
	{
		if (ACodePlayerState* PlayerState = Cast<ACodePlayerState>(OwningController->PlayerState))
		{
			if (PlayerNameText)
			{
				PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
			}
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