// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeRevealeRole.h"
#include "CodePlayerState.h"

void UCodeRevealeRole::NativeConstruct()
{
	RevealeRoleButton->OnPressed.AddDynamic(this, &UCodeRevealeRole::OnRevealeRoleButtonPressed);
}

void UCodeRevealeRole::OnRevealeRoleButtonPressed()
{
	ACodePlayerState* playerState = Cast<ACodePlayerState>(GetOwningPlayer()->PlayerState);
	if (playerState)
	{
		playerState->Server_RevealeRole();
	}
}
