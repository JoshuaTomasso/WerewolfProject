// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGameInstance.h"
#include "CodeSessionManager.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UCodeGameInstance::Init()
{
	Super::Init();
	SessionManager = NewObject<UCodeSessionManager>(this);
	
	if (APlayerController* PC = GetPrimaryPlayerController(false))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->GetUserSettings()->RegisterInputMappingContexts({ DefaultMappingContext });
			}
		}
	}
}
