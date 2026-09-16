// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeDisplaySettings.h"
#include "CodeMainMenu.h"
#include "CodeOptionCycle.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

void UCodeDisplaySettings::NativeConstruct()
{
	Super::NativeConstruct();
		
	if (ResetButton)
		ResetButton->OnPressed.AddDynamic(this, &UCodeDisplaySettings::ResetButtonClicked);

	if (ApplyButton)
		ApplyButton->OnPressed.AddDynamic(this, &UCodeDisplaySettings::ApplyButtonClicked);

	if (BackButton)
		BackButton->OnPressed.AddDynamic(this, &UCodeDisplaySettings::BackButtonClicked);

	if (!DisplayScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("NativeConstruct: DisplayScrollBox is null!"));
		return;
	}

	if (!OptionCycleWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("NativeConstruct: OptionCycleWidgetClass is null!"));
		return;
	}

	SetupDisplayScreenWidget();
	SetupWindowedModeWidget();
	SetupScreenResolutionWidget();
	SetupFPSLimitWidget();
}

void UCodeDisplaySettings::ResetButtonClicked()
{
	
}

void UCodeDisplaySettings::ApplyButtonClicked()
{
	ApplyDisplaySettings();
}

void UCodeDisplaySettings::BackButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::SettingsPanel);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BackButtonClicked: No UCodeMainMenu outer found!"));
	}
}

void UCodeDisplaySettings::SetupDisplayScreenWidget()
{
	if (!OptionCycleWidgetClass || !DisplayScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupDisplayScreenWidget: OptionCycleWidgetClass or DisplayScrollBox is null!"));
		return;
	}

	APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupDisplayScreenWidget: GetOwningPlayer() returned null!"));
		return;
	}

	DisplayScreenWidget = CreateWidget<UCodeOptionCycle>(OwningPlayer, OptionCycleWidgetClass);
	if (!DisplayScreenWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupDisplayScreenWidget: Failed to create DisplayScreenWidget!"));
		return;
	}

	TArray<FText> DisplayNames;
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		if (UExtendedUserSettingsSubsystem* SettingsSubsystem = GameInstance->GetSubsystem<UExtendedUserSettingsSubsystem>())
		{
			AllDisplays = SettingsSubsystem->GetAllDisplayInfo();
			for (FDisplayInfo DisplayInfo : AllDisplays)
			{
				DisplayNames.Add(FText::FromString(DisplayInfo.Name));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SetupDisplayScreenWidget: No ExtendedUserSettingsSubsystem found!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupDisplayScreenWidget: GameInstance is null!"));
	}

	DisplayScreenWidget->OptionArray = DisplayNames;
	DisplayScreenWidget->OptionNameText =  FText::FromString("Display Screen");
	DisplayScreenWidget->DefaultSelectedIndex = 1;
	DisplayScreenWidget->InitializeOptionCycle();

	DisplayScrollBox->AddChild(DisplayScreenWidget);
	DisplayScreenWidget->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
}

void UCodeDisplaySettings::SetupWindowedModeWidget()
{
	if (!OptionCycleWidgetClass || !DisplayScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupWindowedModeWidget: OptionCycleWidgetClass or DisplayScrollBox is null!"));
		return;
	}

	APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupWindowedModeWidget: GetOwningPlayer() returned null!"));
		return;
	}

	WindowedModeWidget = CreateWidget<UCodeOptionCycle>(OwningPlayer, OptionCycleWidgetClass);
	if (!WindowedModeWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupWindowedModeWidget: Failed to create WindowedModeWidget!"));
		return;
	}

	TArray<FText> WindowModeArray;
	AllWindowedModes.Empty();

	const UEnum* WindowModeEnum = StaticEnum<EWindowMode::Type>();
	if (!WindowModeEnum)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupWindowedModeWidget: WindowModeEnum is null!"));
		return;
	}

	const int32 NumEntries = WindowModeEnum->NumEnums() - 1;

	for (int32 i = 0; i < NumEntries; i++)
	{
		const int64 EnumValue = WindowModeEnum->GetValueByIndex(i);

		if (EnumValue == EWindowMode::NumWindowModes)
			continue;

		WindowModeArray.Add(WindowModeEnum->GetDisplayNameTextByIndex(i));
		AllWindowedModes.Add(static_cast<EWindowMode::Type>(EnumValue));
	}

	if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		const EWindowMode::Type CurrentMode = Settings->GetFullscreenMode();
		const FText CurrentModeText = WindowModeEnum->GetDisplayNameTextByValue(static_cast<int64>(CurrentMode));

		const int32 FoundIndex = WindowModeArray.IndexOfByPredicate(
			[&CurrentModeText](const FText& Candidate)
			{
				return Candidate.EqualTo(CurrentModeText);
			});

		WindowedModeWidget->DefaultSelectedIndex = FoundIndex != INDEX_NONE ? FoundIndex : 0;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupWindowedModeWidget: GEngine or GameUserSettings is null!"));
	}

	WindowedModeWidget->OptionArray = WindowModeArray;
	WindowedModeWidget->OptionNameText = FText::FromString("Windowed Mode");
	WindowedModeWidget->InitializeOptionCycle();

	DisplayScrollBox->AddChild(WindowedModeWidget);
	WindowedModeWidget->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
}

void UCodeDisplaySettings::SetupScreenResolutionWidget()
{
	if (!OptionCycleWidgetClass || !DisplayScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupScreenResolutionWidget: OptionCycleWidgetClass or DisplayScrollBox is null!"));
		return;
	}

	APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupScreenResolutionWidget: GetOwningPlayer() returned null!"));
		return;
	}

	ScreenResolutionWidget = CreateWidget<UCodeOptionCycle>(OwningPlayer, OptionCycleWidgetClass);
	if (!ScreenResolutionWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupScreenResolutionWidget: Failed to create ScreenResolutionWidget!"));
		return;
	}

	TArray<FIntPoint> ScreenResolutionArray;
	TArray<FText> ScreenResolutionTextArray;
	
	
	if (UKismetSystemLibrary::GetSupportedFullscreenResolutions(ScreenResolutionArray))
	{
		for (const FIntPoint& ScreenResolution : ScreenResolutionArray)
		{
			FText Temp = FText::FromString(FString::Printf(TEXT("%d x %d"), ScreenResolution.X, ScreenResolution.Y));
			ScreenResolutionTextArray.Add(Temp);
		}
	}
	
	ScreenResolutionWidget->OptionArray = ScreenResolutionTextArray;
	ScreenResolutionWidget->OptionNameText = FText::FromString("Screen Resolution");
	ScreenResolutionWidget->InitializeOptionCycle();

	DisplayScrollBox->AddChild(ScreenResolutionWidget);
	ScreenResolutionWidget->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
}

void UCodeDisplaySettings::SetupFPSLimitWidget()
{
	if (!OptionCycleWidgetClass || !DisplayScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupFPSLimitWidget: OptionCycleWidgetClass or DisplayScrollBox is null!"));
		return;
	}

	APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupFPSLimitWidget: GetOwningPlayer() returned null!"));
		return;
	}

	FPSLimitWidget = CreateWidget<UCodeOptionCycle>(OwningPlayer, OptionCycleWidgetClass);
	if (!FPSLimitWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupFPSLimitWidget: Failed to create FPSLimitWidget!"));
		return;
	}

	FPSLimitWidget->OptionArray = {FText::FromString("60"), FText::FromString("144"), FText::FromString("240"), FText::FromString("Unlimited")};
	FPSLimitWidget->OptionNameText = FText::FromString("FPS Limit");
	FPSLimitWidget->InitializeOptionCycle();

	DisplayScrollBox->AddChild(FPSLimitWidget);
	FPSLimitWidget->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
}

void UCodeDisplaySettings::SetActiveDisplay()
{
	if (!DisplayScreenWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SetActiveDisplay: DisplayScreenWidget is null!"));
		return;
	}

	const UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("SetActiveDisplay: GameInstance is null!"));
		return;
	}

	UExtendedUserSettingsSubsystem* SettingsSubsystem = GameInstance->GetSubsystem<UExtendedUserSettingsSubsystem>();
	if (!SettingsSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetActiveDisplay: No ExtendedUserSettingsSubsystem found!"));
		return;
	}

	const int32 SelectedIndex = DisplayScreenWidget->GetSelectedIndex();
	if (!AllDisplays.IsValidIndex(SelectedIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetActiveDisplay: SelectedIndex %d is not valid in AllDisplays!"), SelectedIndex);
		return;
	}

	SettingsSubsystem->SetActiveDisplayByID(AllDisplays[SelectedIndex].ID);
}

void UCodeDisplaySettings::SetResolution()
{
	TArray<FIntPoint> ScreenResolutionArray;
	if (UKismetSystemLibrary::GetSupportedFullscreenResolutions(ScreenResolutionArray))
	{
		if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings())
		{
			GameUserSettings->SetScreenResolution(ScreenResolutionArray[ScreenResolutionWidget->GetSelectedIndex()]);
		}
	}
	
}

void UCodeDisplaySettings::ApplyDisplaySettings()
{
	if (!GEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyDisplaySettings: GEngine is null!"));
		return;
	}

	if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings())
	{
		SetActiveDisplay();

		if (!WindowedModeWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("ApplyDisplaySettings: WindowedModeWidget is null!"));
			return;
		}

		const int32 SelectedIndex = WindowedModeWidget->GetSelectedIndex();
		if (!AllWindowedModes.IsValidIndex(SelectedIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("ApplyDisplaySettings: SelectedIndex %d is not valid in AllWindowedModes!"), SelectedIndex);
			return;
		}

		GameUserSettings->SetFullscreenMode(AllWindowedModes[SelectedIndex]);
		GameUserSettings->ApplyResolutionSettings(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyDisplaySettings: GetGameUserSettings() returned null!"));
	}
}
