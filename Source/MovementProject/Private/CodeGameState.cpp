// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGameState.h"
#include "CodePlayerController.h"

void ACodeGameState::MulticastSendFinalPlayerList_Implementation()
{
	ACodePlayerController* LocalPlayerController = Cast<ACodePlayerController>(GetWorld()->GetFirstPlayerController());
	if (LocalPlayerController)
	{
		// LocalPlayerController->nightActionWidget->PopulateTargetList();
	}
}