// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePlayerRoleDiscription.h"

void UCodePlayerRoleDiscription::RemoveWidgetAfterDelay(float DelayTime)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RemoveWidgetTimerHandle);

		World->GetTimerManager().SetTimer(
			RemoveWidgetTimerHandle,
			this,
			&UCodePlayerRoleDiscription::OnRemoveWidgetTimer,
			DelayTime,
			false  
		);
	}
}

void UCodePlayerRoleDiscription::OnRemoveWidgetTimer()
{
	RemoveFromParent();
}