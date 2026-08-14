// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeRevealeRole.h"
#include "CodePlayerState.h"

void UCodeRevealeRole::NativeConstruct()
{
	RevealeRoleButton->OnPressed.AddDynamic(this, &UCodeRevealeRole::OnRevealRoleButtonPressed);
}

void UCodeRevealeRole::OnRevealRoleButtonPressed()
{
	if (ACodePlayerState* PlayerState = Cast<ACodePlayerState>(GetOwningPlayer()->PlayerState))
	{
		PlayerState->Server_RevealRole();
	}
}
