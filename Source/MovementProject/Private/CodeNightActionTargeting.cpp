// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeNightActionTargeting.h"
#include "CodePlayerState.h"
#include "CodeButtonAndText.h"
#include "CodeGameState.h"

void UCodeNightActionTargeting::NativeConstruct()
{
	if (VoteNotificationText)
	{
		VoteNotificationText->SetVisibility(ESlateVisibility::Collapsed);
		TargetDeadNotificationText->SetVisibility(ESlateVisibility::Collapsed);

		APlayerController* OwningController = GetOwningPlayer();
		if (OwningController && OwningController->PlayerState)
		{
			InitializePlayerState();
		}
		else
		{
			InitializationRetryCount = 0;
			TryInitializePlayerState();
		}
	}
}

void UCodeNightActionTargeting::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(InitializationTimerHandle);
	}

	if (CastedPlayer)
	{
		CastedPlayer->OnRoleAssigned.RemoveDynamic(this, &UCodeNightActionTargeting::OnRoleAssignedHandler);
		CastedPlayer->OnErrorCountChanged.RemoveDynamic(this, &UCodeNightActionTargeting::ShowVoteNotification);
		CastedPlayer->OnTargetDeadCountChanged.RemoveDynamic(this, &UCodeNightActionTargeting::ShowTargetDeadNotification);
	}

	if (PartnersTarget)
	{
		PartnersTarget->OnNightTargetChanged.RemoveDynamic(this, &UCodeNightActionTargeting::UpdatePartnerVoteDisplay);
	}

	Super::NativeDestruct();
}

void UCodeNightActionTargeting::PopulateTargetList()
{
	if (!CastedPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("PopulateTargetList: CastedPlayer is null"));
		return;
	}

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
					if (CodePlayerState->currentRole != ERoles::Medic && CodePlayerState == CastedPlayer)
					{
						continue; // Skip self
					}
					else if (CastedPlayer->currentRole == ERoles::Werewolf && CodePlayerState == CastedPlayer->werewolfPartner)
					{
						continue; // Skip the werewolf's partner
					}
					else if (CodePlayerState->bIsAlive)
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

void UCodeNightActionTargeting::UpdatePartnerVoteDisplay()
{
	if (PartnersTarget && PartnersTarget->nightTarget)
	{
		ChosenPlayerName->SetText(FText::FromString(PartnersTarget->nightTarget->GetPlayerName()));
	}
	else
	{
		ChosenPlayerName->SetText(FText::FromString(TEXT("Partner Has Not Chosen Yet.")));
	}
}

void UCodeNightActionTargeting::OnRoleAssignedHandler()
{
	if (CastedPlayer->currentRole == ERoles::Werewolf)
	{
		PartnerChoiceText->SetVisibility(ESlateVisibility::Visible);
		ChosenPlayerName->SetVisibility(ESlateVisibility::Visible);

		if (CastedPlayer->werewolfPartner)
		{
			PartnersTarget = CastedPlayer->werewolfPartner;
			CastedPlayer->werewolfPartner->OnNightTargetChanged.RemoveDynamic(this, &UCodeNightActionTargeting::UpdatePartnerVoteDisplay);
			CastedPlayer->werewolfPartner->OnNightTargetChanged.AddDynamic(this, &UCodeNightActionTargeting::UpdatePartnerVoteDisplay);
			UpdatePartnerVoteDisplay();
		}
	}
	else
	{
		PartnerChoiceText->SetVisibility(ESlateVisibility::Collapsed);
		ChosenPlayerName->SetVisibility(ESlateVisibility::Collapsed);
	}
	PopulateTargetList();
}

void UCodeNightActionTargeting::ShowVoteNotification()
{
	VoteNotificationText->SetVisibility(ESlateVisibility::Visible);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer
	(
		TimerHandle,
		this,
		&UCodeNightActionTargeting::HideVoteNotification,
		1.0f,
		false
	);
}

void UCodeNightActionTargeting::HideVoteNotification()
{
	VoteNotificationText->SetVisibility(ESlateVisibility::Collapsed);
}

void UCodeNightActionTargeting::ShowTargetDeadNotification()
{
	TargetDeadNotificationText->SetVisibility(ESlateVisibility::Visible);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer
	(
		TimerHandle,
		this,
		&UCodeNightActionTargeting::HideTargetDeadNotification,
		1.0f,
		false
	);
}

void UCodeNightActionTargeting::HideTargetDeadNotification()
{
	TargetDeadNotificationText->SetVisibility(ESlateVisibility::Collapsed);
}

void UCodeNightActionTargeting::TryInitializePlayerState()
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
			CastedPlayer = PlayerState;

			if (PlayerNameText)
			{
				PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
			}

			PlayerState->OnRoleAssigned.RemoveDynamic(this, &UCodeNightActionTargeting::OnRoleAssignedHandler);
			PlayerState->OnRoleAssigned.AddDynamic(this, &UCodeNightActionTargeting::OnRoleAssignedHandler);

			OnRoleAssignedHandler();

			PlayerState->OnErrorCountChanged.RemoveDynamic(this, &UCodeNightActionTargeting::ShowVoteNotification);
			PlayerState->OnErrorCountChanged.AddDynamic(this, &UCodeNightActionTargeting::ShowVoteNotification);

			PlayerState->OnTargetDeadCountChanged.RemoveDynamic(this, &UCodeNightActionTargeting::ShowTargetDeadNotification);
			PlayerState->OnTargetDeadCountChanged.AddDynamic(this, &UCodeNightActionTargeting::ShowTargetDeadNotification);
		}
	}
	else if (InitializationRetryCount < 50)
	{
		InitializationRetryCount++;
		GetWorld()->GetTimerManager().SetTimer(
			InitializationTimerHandle,
			this,
			&UCodeNightActionTargeting::TryInitializePlayerState,
			0.1f,
			false
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TryInitializePlayerState: Failed to initialize after 50 attempts, giving up"));
	}
}

void UCodeNightActionTargeting::InitializePlayerState()
{
	APlayerController* OwningController = GetOwningPlayer();
	if (!OwningController || !OwningController->PlayerState)
	{
		return;
	}

	if (ACodePlayerState* PlayerState = Cast<ACodePlayerState>(OwningController->PlayerState))
	{
		CastedPlayer = PlayerState;

		if (PlayerNameText)
		{
			PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
		}

		PlayerState->OnRoleAssigned.RemoveDynamic(this, &UCodeNightActionTargeting::OnRoleAssignedHandler);
		PlayerState->OnRoleAssigned.AddDynamic(this, &UCodeNightActionTargeting::OnRoleAssignedHandler);

		OnRoleAssignedHandler();

		PlayerState->OnErrorCountChanged.RemoveDynamic(this, &UCodeNightActionTargeting::ShowVoteNotification);
		PlayerState->OnErrorCountChanged.AddDynamic(this, &UCodeNightActionTargeting::ShowVoteNotification);
	}
}