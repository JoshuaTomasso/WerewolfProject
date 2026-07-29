// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePlayerController.h"
#include "CodeGameMode.h"
#include "CodeGamePhaseTimer.h"
#include "CodeGameState.h"
#include "SRoleInfo.h"
#include "CodeNightActionTargeting.h"
#include "CodePlayerState.h"
#include "CodeDayVoteTargeting.h"

void ACodePlayerController::Server_NotifyReady_Implementation()
{
	ACodeGameMode* GameMode = Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode)
	{
		GameMode->NotifyPlayerReady(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server_NotifyReady: GameMode is null"));
	}
}

void ACodePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (GamePhaseTimerWidgetClass)
		{
			UCodeGamePhaseTimer* GamePhaseTimerWidget = CreateWidget<UCodeGamePhaseTimer>(this, GamePhaseTimerWidgetClass);
			if (GamePhaseTimerWidget)
			{
				GamePhaseTimerWidget->AddToViewport();
			}
		}
		if (NightActionWidgetClass)
		{
			nightActionWidget = CreateWidget<UCodeNightActionTargeting>(this, NightActionWidgetClass);
			if (nightActionWidget)
			{
				nightActionWidget->AddToViewport();
				nightActionWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BeginPlay: nightActionWidget is null"));
			}
		}
		if (DayVoteWidgetClass)
		{
			dayVoteWidget = CreateWidget<UCodeDayVoteTargeting>(this, DayVoteWidgetClass);
			if (dayVoteWidget)
			{
				dayVoteWidget->AddToViewport();
				dayVoteWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BeginPlay: dayVoteWidget is null"));
			}
		}
	}
}

void ACodePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsLocalController())
	{
		ACodeGameState* GameState = Cast<ACodeGameState>(GetWorld()->GetGameState());
		if (!bHasAckedReady)
		{
			if (GameState)
			{
				if (GameState->ExpectedPlayerCount > 0 && GameState->PlayerArray.Num() >= GameState->ExpectedPlayerCount)
				{
					Server_NotifyReady();
					bHasAckedReady = true;
				}
			}
		}

		if (GameState)
		{
			if (GameState->currentPhase == EPhases::Night && bHasAckedReady)
			{
				if (dayVoteWidget)
				{
					dayVoteWidget->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Tick: dayVoteWidget is null"));
				}

				ACodePlayerState* CodePlayerState = GetPlayerState<ACodePlayerState>();
				if (CodePlayerState)
				{
					bShowMouseCursor = true;
					SetInputMode(FInputModeGameAndUI());
					FString RoleNameString = UEnum::GetValueAsString(CodePlayerState->currentRole);
					RoleNameString.RemoveFromStart(TEXT("ERoles::"));

					FSRoleInfo* RoleInfo = ControllerRoleDataTable->FindRow<FSRoleInfo>(*RoleNameString, TEXT("PlayerControllerTick"));

					if (RoleInfo)
					{

						if (RoleInfo->bCanActAtNight)
						{
							if (nightActionWidget && CodePlayerState->bIsAlive)
							{
								nightActionWidget->SetVisibility(ESlateVisibility::Visible);
							}
							bNightWidgetActive = true;
						}

					}

				}
			}
			else if (GameState->currentPhase == EPhases::Voting && bHasAckedReady)
			{
				bShowMouseCursor = true;
				SetInputMode(FInputModeGameAndUI());
				if (dayVoteWidget)
				{
					dayVoteWidget->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Tick: dayVoteWidget is null"));
				}
			}
			else
			{
				if (bNightWidgetActive)
				{
					if (nightActionWidget)
					{
						nightActionWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Tick: nightActionWidget is null"));
					}

					bNightWidgetActive = false;
					bShowMouseCursor = false;
					SetInputMode(FInputModeGameOnly());
				}
			}

			if (GameState->currentPhase == EPhases::Night && !bNightTargetListPopulated)
			{
				nightActionWidget->PopulateTargetList();
				bNightTargetListPopulated = true;
			}
			else if (GameState->currentPhase == EPhases::Day && bNightTargetListPopulated)
			{
				bNightTargetListPopulated = false;
			}
			else if (GameState->currentPhase == EPhases::Voting && !bDayTargetListPopulated)
			{
				dayVoteWidget->PopulateTargetList();
				bDayTargetListPopulated = true;
			}
			else if (GameState->currentPhase == EPhases::Night && bDayTargetListPopulated)
			{
				bDayTargetListPopulated = false;
			}
		}
	}
}