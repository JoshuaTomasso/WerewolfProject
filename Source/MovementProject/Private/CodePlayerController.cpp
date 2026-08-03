// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePlayerController.h"
#include "CodeGameMode.h"
#include "CodeGamePhaseTimer.h"
#include "CodeGameState.h"
#include "SRoleInfo.h"
#include "CodeNightActionTargeting.h"
#include "CodePlayerState.h"
#include "CodeDayVoteTargeting.h"
#include "CodeGameOver.h"
#include "CodeNightResult.h"
#include "CodeRevealeRole.h"

void ACodePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (GamePhaseTimerWidgetClass)
		{
			gamePhaseTimerWidget = CreateWidget<UCodeGamePhaseTimer>(this, GamePhaseTimerWidgetClass);
			if (gamePhaseTimerWidget)
			{
				gamePhaseTimerWidget->AddToViewport();
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
		if (GameOverWidgetClass)
		{
			gameOverWidget = CreateWidget<UCodeGameOver>(this, GameOverWidgetClass);
			if (gameOverWidget)
			{
				gameOverWidget->AddToViewport();
				gameOverWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BeginPlay: gameOverWidget is null"));
			}
		}
		if (NightResultWidgetClass)
		{
			nightResultWidget = CreateWidget<UCodeNightResult>(this, NightResultWidgetClass);
			if (nightResultWidget)
			{
				nightResultWidget->AddToViewport();
				nightResultWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BeginPlay: nightResultWidget is null"));
			}
		}
		if (RevealeRoleWidgetClass)
		{
			revealeRoleWidget = CreateWidget<UCodeRevealeRole>(this, RevealeRoleWidgetClass);
			if (revealeRoleWidget)
			{
				revealeRoleWidget->AddToViewport();
				revealeRoleWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BeginPlay: revealeRoleWidget is null"));
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
					nightResultWidget->SetVisibility(ESlateVisibility::Collapsed);
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
							else
							{
								nightActionWidget->SetVisibility(ESlateVisibility::Collapsed);
							}
							bNightWidgetActive = true;
						}

					}

				}
			}
			else if (GameState->currentPhase == EPhases::Voting && bHasAckedReady)
			{

				ACodePlayerState* CodePlayerState = GetPlayerState<ACodePlayerState>();
				if (CodePlayerState)
				{
					bShowMouseCursor = true;
					SetInputMode(FInputModeGameAndUI());
					if (dayVoteWidget && CodePlayerState->bIsAlive)
					{
						nightActionWidget->SetVisibility(ESlateVisibility::Collapsed);
						nightResultWidget->SetVisibility(ESlateVisibility::Collapsed);
						revealeRoleWidget->SetVisibility(ESlateVisibility::Collapsed);
						dayVoteWidget->SetVisibility(ESlateVisibility::Visible);
					}
					else
					{
						dayVoteWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
			}
			else if (GameState->currentPhase == EPhases::Day && bHasAckedReady)
			{
				ACodePlayerState* CodePlayerState = GetPlayerState<ACodePlayerState>();
				if (CodePlayerState)
				{
					if (nightResultWidget && CodePlayerState->bIsAlive)
					{
						dayVoteWidget->SetVisibility(ESlateVisibility::Collapsed);
						nightActionWidget->SetVisibility(ESlateVisibility::Collapsed);
						nightResultWidget->SetVisibility(ESlateVisibility::Visible);
						if (CodePlayerState->currentRole == ERoles::Mayor && !CodePlayerState->bHasRevealedRole)
						{
							revealeRoleWidget->SetVisibility(ESlateVisibility::Visible);
						}
					}
					else
					{
						nightResultWidget->SetVisibility(ESlateVisibility::Collapsed);
						revealeRoleWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
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

void ACodePlayerController::Client_SetNightResultText_Implementation(const FString& ResultText)
{
	if (nightResultWidget)
	{
		nightResultWidget->UpdateNightResult(FText::FromString(ResultText));
		nightResultWidget->ShowNightResult();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client_SetNightResultText: nightResultWidget is null"));
	}
}

void ACodePlayerController::ShowGameOverWidget(FString WinningFaction)
{
	if (gameOverWidget)
	{
		gamePhaseTimerWidget->SetVisibility(ESlateVisibility::Collapsed);
		gameOverWidget->SetVisibility(ESlateVisibility::Visible);

		gameOverWidget->GameOverText->SetText(FText::FromString(WinningFaction));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ShowGameOverWidget: gameOverWidget is null"));
	}
}
