// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeNightResult.h"
#include "CodePlayerState.h"

void UCodeNightResult::NativeConstruct()
{
	InitializationRetryCount = 0;
	TryInitializePlayerState();
	NightResultText->SetVisibility(ESlateVisibility::Collapsed);
}

void UCodeNightResult::UpdateNightResult(const FText& resultText)
{
	if (NightResultText)
	{
		NightResultText->SetText(resultText);
	}
}

void UCodeNightResult::ShowNightResult()
{
	if (NightResultText)
	{
		NightResultText->SetVisibility(ESlateVisibility::Visible);

		GetWorld()->GetTimerManager().SetTimer(
			NightResultTimerHandle,
			this,
			&UCodeNightResult::HideNightResult,
			5.0f,
			false
		);
	}
}

void UCodeNightResult::HideNightResult()
{
	if (NightResultText)
	{
		NightResultText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCodeNightResult::TryInitializePlayerState()
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
			&UCodeNightResult::TryInitializePlayerState,
			0.1f,
			false
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TryInitializePlayerState: Failed to initialize after 50 attempts, giving up"));
	}
}