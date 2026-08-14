// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeNightResult.h"
#include "CodePlayerState.h"

void UCodeNightResult::NativeConstruct()
{
	InitializationRetryCount = 0;
	TryInitializePlayerState();
	NightResultText->SetVisibility(ESlateVisibility::Collapsed);
}

void UCodeNightResult::UpdateNightResult(const FText& ResultText) const
{
	if (NightResultText)
	{
		NightResultText->SetText(ResultText);
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

void UCodeNightResult::HideNightResult() const
{
	if (NightResultText)
	{
		NightResultText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCodeNightResult::TryInitializePlayerState()
{
	if (const APlayerController* OwningController = GetOwningPlayer())
	{
		if (OwningController->PlayerState)
		{
			if (const ACodePlayerState* PlayerState = Cast<ACodePlayerState>(OwningController->PlayerState))
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
}
