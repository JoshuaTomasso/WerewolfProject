// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeWerewolfPartnerReveal.h"

void UCodeWerewolfPartnerReveal::RemoveWidgetAfterDelay(float DelayTime)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RemoveWidgetTimerHandle);

		World->GetTimerManager().SetTimer(
			RemoveWidgetTimerHandle,
			this,
			&UCodeWerewolfPartnerReveal::OnRemoveWidgetTimer,
			DelayTime,
			false
		);
	}
}

void UCodeWerewolfPartnerReveal::OnRemoveWidgetTimer()
{
	RemoveFromParent();
}