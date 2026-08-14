// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeNightActionTargeting.h"
#include "CodePlayerState.h"
#include "CodeButtonAndText.h"
#include "CodeGameState.h"
#include "Components/Spacer.h"

void UCodeNightActionTargeting::NativeConstruct()
{
	if (VoteNotificationText)
	{
		VoteNotificationText->SetVisibility(ESlateVisibility::Collapsed);

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
	
	if (AGameStateBase* GameStateBase = GetWorld()->GetGameState())
	{
		if (ACodeGameState* CodeGameState = Cast<ACodeGameState>(GameStateBase))
		{
			for (APlayerState* PlayerState : CodeGameState->PlayerArray)
			{
				if (ACodePlayerState* CodePlayerState = Cast<ACodePlayerState>(PlayerState))
				{
					if (CodePlayerState->CurrentRole != ERoles::Medic && CodePlayerState == CastedPlayer)
					{
						continue; // Skip self
					}
					else if (CastedPlayer->CurrentRole == ERoles::Werewolf && CodePlayerState == CastedPlayer->WerewolfPartner)
					{
						continue; // Skip the werewolf's partner
					}
					else if (CodePlayerState->bIsAlive)
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
	if (PartnersTarget && PartnersTarget->NightTarget)
	{
		ChosenPlayerName->SetText(FText::FromString(PartnersTarget->NightTarget->GetPlayerName()));
	}
	else
	{
		ChosenPlayerName->SetText(FText::FromString(TEXT("Partner Has Not Chosen Yet.")));
	}
}

void UCodeNightActionTargeting::OnRoleAssignedHandler()
{
	if (CastedPlayer->CurrentRole == ERoles::Werewolf)
	{
		PartnerChoiceText->SetVisibility(ESlateVisibility::Visible);
		ChosenPlayerName->SetVisibility(ESlateVisibility::Visible);

		if (CastedPlayer->WerewolfPartner)
		{
			PartnersTarget = CastedPlayer->WerewolfPartner;
			CastedPlayer->WerewolfPartner->OnNightTargetChanged.RemoveDynamic(this, &UCodeNightActionTargeting::UpdatePartnerVoteDisplay);
			CastedPlayer->WerewolfPartner->OnNightTargetChanged.AddDynamic(this, &UCodeNightActionTargeting::UpdatePartnerVoteDisplay);
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

	if (CastedPlayer->CurrentRole == ERoles::Medic)
	{
		VoteNotificationText->SetText(FText::FromString("You have ran out of self protection"));
	}
	else if (CastedPlayer->CurrentRole == ERoles::Seer)
	{
		VoteNotificationText->SetText(FText::FromString("You have ran out of seer ability"));
	}

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