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
			GamePhaseTimerWidget = CreateWidget<UCodeGamePhaseTimer>(this, GamePhaseTimerWidgetClass);
			if (GamePhaseTimerWidget)
			{
				GamePhaseTimerWidget->AddToViewport();
			}
		}
		if (NightActionWidgetClass)
		{
			NightActionWidget = CreateWidget<UCodeNightActionTargeting>(this, NightActionWidgetClass);
			if (NightActionWidget)
			{
				NightActionWidget->AddToViewport();
				NightActionWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BeginPlay: nightActionWidget is null"));
			}
		}
		if (DayVoteWidgetClass)
		{
			DayVoteWidget = CreateWidget<UCodeDayVoteTargeting>(this, DayVoteWidgetClass);
			if (DayVoteWidget)
			{
				DayVoteWidget->AddToViewport();
				DayVoteWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BeginPlay: dayVoteWidget is null"));
			}
		}
		if (GameOverWidgetClass)
		{
			GameOverWidget = CreateWidget<UCodeGameOver>(this, GameOverWidgetClass);
			if (GameOverWidget)
			{
				GameOverWidget->AddToViewport();
				GameOverWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BeginPlay: gameOverWidget is null"));
			}
		}
		if (NightResultWidgetClass)
		{
			NightResultWidget = CreateWidget<UCodeNightResult>(this, NightResultWidgetClass);
			if (NightResultWidget)
			{
				NightResultWidget->AddToViewport();
				NightResultWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BeginPlay: nightResultWidget is null"));
			}
		}
		if (RevealRoleWidgetClass)
		{
			RevealRoleWidget = CreateWidget<UCodeRevealeRole>(this, RevealRoleWidgetClass);
			if (RevealRoleWidget)
			{
				RevealRoleWidget->AddToViewport();
				RevealRoleWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BeginPlay: revealeRoleWidget is null"));
			}
		}
	}
}

void ACodePlayerController::Tick(const float DeltaTime)
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
			if (GameState->CurrentPhase == EPhases::Night && bHasAckedReady)
			{
				if (DayVoteWidget)
				{
					DayVoteWidget->SetVisibility(ESlateVisibility::Collapsed);
					NightResultWidget->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Tick: dayVoteWidget is null"));
				}
				
				if (const ACodePlayerState* CodePlayerState = GetPlayerState<ACodePlayerState>())
				{

					FString RoleNameString = UEnum::GetValueAsString(CodePlayerState->CurrentRole);
					RoleNameString.RemoveFromStart(TEXT("ERoles::"));

					if (const FSRoleInfo* RoleInfo = ControllerRoleDataTable->FindRow<FSRoleInfo>(*RoleNameString, TEXT("PlayerControllerTick")))
					{

						if (RoleInfo->bCanActAtNight)
						{
							if (NightActionWidget && CodePlayerState->bIsAlive)
							{
								NightActionWidget->SetVisibility(ESlateVisibility::Visible);
							}
							else
							{
								NightActionWidget->SetVisibility(ESlateVisibility::Collapsed);
							}
							bNightWidgetActive = true;
						}

					}

				}
			}
			else if (GameState->CurrentPhase == EPhases::Voting && bHasAckedReady)
			{
				if (const ACodePlayerState* CodePlayerState = GetPlayerState<ACodePlayerState>())
				{

					if (DayVoteWidget && CodePlayerState->bIsAlive)
					{
						NightActionWidget->SetVisibility(ESlateVisibility::Collapsed);
						NightResultWidget->SetVisibility(ESlateVisibility::Collapsed);
						RevealRoleWidget->SetVisibility(ESlateVisibility::Collapsed);
						DayVoteWidget->SetVisibility(ESlateVisibility::Visible);
					}
					else
					{
						DayVoteWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
			}
			else if (GameState->CurrentPhase == EPhases::Day && bHasAckedReady)
			{
				
				if (const ACodePlayerState* CodePlayerState = GetPlayerState<ACodePlayerState>())
				{
					if (NightResultWidget && CodePlayerState->bIsAlive)
					{
						DayVoteWidget->SetVisibility(ESlateVisibility::Collapsed);
						NightActionWidget->SetVisibility(ESlateVisibility::Collapsed);
						NightResultWidget->SetVisibility(ESlateVisibility::Visible);
						if (CodePlayerState->CurrentRole == ERoles::Mayor && !CodePlayerState->bHasRevealedRole)
						{
							RevealRoleWidget->SetVisibility(ESlateVisibility::Visible);
						}
					}
					else
					{
						NightResultWidget->SetVisibility(ESlateVisibility::Collapsed);
						RevealRoleWidget->SetVisibility(ESlateVisibility::Collapsed);
					}

				}
			}

			if (GameState->CurrentPhase == EPhases::Night && !bNightTargetListPopulated)
			{
				NightActionWidget->PopulateTargetList();
				bNightTargetListPopulated = true;
			}
			else if (GameState->CurrentPhase == EPhases::Day && bNightTargetListPopulated)
			{
				bNightTargetListPopulated = false;
			}
			else if (GameState->CurrentPhase == EPhases::Voting && !bDayTargetListPopulated)
			{
				DayVoteWidget->PopulateTargetList();
				bDayTargetListPopulated = true;
			}
			else if (GameState->CurrentPhase == EPhases::Night && bDayTargetListPopulated)
			{
				bDayTargetListPopulated = false;
			}
		}
	}
}

void ACodePlayerController::Server_NotifyReady_Implementation()
{
	if (ACodeGameMode* GameMode = Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode()))
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
	if (NightResultWidget)
	{
		NightResultWidget->UpdateNightResult(FText::FromString(ResultText));
		NightResultWidget->ShowNightResult();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client_SetNightResultText: nightResultWidget is null"));
	}
}

void ACodePlayerController::ShowGameOverWidget(FString WinningFaction)
{
	if (GameOverWidget)
	{
		GamePhaseTimerWidget->SetVisibility(ESlateVisibility::Collapsed);
		GameOverWidget->SetVisibility(ESlateVisibility::Visible);

		GameOverWidget->GameOverText->SetText(FText::FromString(WinningFaction));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ShowGameOverWidget: gameOverWidget is null"));
	}
}
