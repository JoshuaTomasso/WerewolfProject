// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGameInstance.h"
#include "CodeSessionManager.h"
#include "GameFramework/GameUserSettings.h"

void UCodeGameInstance::Init()
{
	Super::Init();
	
	if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings()) 
	{
		UserSettings->SetOverallScalabilityLevel(0);
		UserSettings->SetFullscreenMode(EWindowMode::Windowed);
		UserSettings->SetFrameRateLimit(30);
		UserSettings->ApplySettings(true);
	}

	SessionManager = NewObject<UCodeSessionManager>(this);
}
