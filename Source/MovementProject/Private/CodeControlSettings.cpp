// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeControlSettings.h"
#include "CodeMainMenu.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UCodeControlSettings::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ResetButton)
		ResetButton->OnPressed.AddDynamic(this, &UCodeControlSettings::ResetButtonClicked);

	if (ApplyButton)
		ApplyButton->OnPressed.AddDynamic(this, &UCodeControlSettings::ApplyButtonClicked);

	if (BackButton)
		BackButton->OnPressed.AddDynamic(this, &UCodeControlSettings::BackButtonClicked);
	
	ControlsScrollBox->ClearChildren();
	SetupControlsSettings();
	SavedKeyMap = KeyMap;
}

void UCodeControlSettings::ResetButtonClicked()
{
	ResetKeyMapping();
}

void UCodeControlSettings::ApplyButtonClicked()
{
	ApplyChangedSettings();
}

void UCodeControlSettings::BackButtonClicked()
{
	if (bHasUnsavedChanges)
	{
		CodeQuestionScreen = CreateWidget<UCodeQuestionScreen>(GetOwningPlayer(), CodeQuestionScreenClass);
		if (!CodeQuestionScreen)
		{
			UE_LOG(LogTemp, Error, TEXT("ApplyChangedSettings: CodeQuestionScreenClass is not set!"));
			return;
		}

		CodeQuestionScreen->SetButtonCount(true);
		CodeQuestionScreen->ButtonTwo->OnClicked().AddUObject(this, &UCodeControlSettings::BackOutWithoutSaving);
		CodeQuestionScreen->ButtonOne->OnClicked().AddUObject(this, &UCodeControlSettings::ApplyChangedAndClose);
		CodeQuestionScreen->AddToViewport();
		CodeQuestionScreen->SetQuestionText(FText::FromString("You Have Unsaved Changes."));
		CodeQuestionScreen->SetButtonOneText(FText::FromString("Apply"));
		CodeQuestionScreen->SetButtonTwoText(FText::FromString("Discard"));
	}
	else
	{
		if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
		{
			ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::SettingsPanel);
		}
	}
}

void UCodeControlSettings::SetupControlsSettings()
{
	OptionKeys.Empty();
	KeyMap = GetKeyMapping();
	
	for (TPair<FName, FKey> KeyPair : KeyMap)
	{
		CodeOptionKey = CreateWidget<UCodeOptionKey>(GetOwningPlayer(), CodeOptionKeyClass);
		OptionKeys.Add(KeyPair.Key, CodeOptionKey);
		CodeOptionKey->DefaultInputKey = KeyPair.Value;
		CodeOptionKey->DefaultInputName = FText::FromName(KeyPair.Key);
		CodeOptionKey->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
		ControlsScrollBox->AddChild(CodeOptionKey);
		
		CodeOptionKey->OnKeyChangedDispatch.AddDynamic(this, &UCodeControlSettings::UpdateKeyMapping);
	}
}

TMap<FName, FKey> UCodeControlSettings::GetKeyMapping()
{
	TMap<FName, FKey> TempKeyMap;
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->GetUserSettings()->RegisterInputMappingContexts({ DefaultMappingContext });
			}

			const TMap<FName, FKeyMappingRow>& Rows = Subsystem->GetUserSettings()->GetActiveKeyProfile()->GetPlayerMappingRows();
			for (const TPair<FName, FKeyMappingRow>& KeyPair : Rows)
			{
				if (KeyPair.Value.Mappings.Num() > 0)
				{
					const FPlayerKeyMapping& Mapping = KeyPair.Value.Mappings.Array()[0];
					TempKeyMap.Add(KeyPair.Key, Mapping.GetCurrentKey());
				}
			}
		}
	}
	return TempKeyMap;
}

void UCodeControlSettings::UpdateKeyMapping(FName InputMappingKey, FKey InputKey)
{
	CheckKeyConflicts(InputMappingKey, InputKey);
	
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
			const TMap<FName, FKeyMappingRow>& Rows = UserSettings->GetActiveKeyProfile()->GetPlayerMappingRows();
			if (const FKeyMappingRow* Row = Rows.Find(InputMappingKey))
			{
				if (Row->Mappings.Num() > 0)
				{
					const FPlayerKeyMapping& ExistingMapping = Row->Mappings.Array()[0];

					FMapPlayerKeyArgs Args;
					Args.MappingName = InputMappingKey;
					Args.Slot = ExistingMapping.GetSlot();
					Args.NewKey = InputKey;
					Args.HardwareDeviceId = ExistingMapping.GetHardwareDeviceId().HardwareDeviceIdentifier;
					FGameplayTagContainer FailureReason;
					
					if (InputKey != ExistingMapping.GetCurrentKey())
					{
						UserSettings->MapPlayerKey(Args, FailureReason);
						bHasUnsavedChanges = true;
					}
					
				}
			}
		}
	}
}

void UCodeControlSettings::CheckKeyConflicts(FName InputMappingKey, FKey InputKey)
{
	bHasConflicts = false;
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
			const TMap<FName, FKeyMappingRow>& Rows = UserSettings->GetActiveKeyProfile()->GetPlayerMappingRows();
			for (const TPair<FName, FKeyMappingRow>& KeyPair : Rows)
			{
				if (KeyPair.Key != InputMappingKey)
				{
					if (KeyPair.Value.Mappings.Num() > 0)
					{
						const FPlayerKeyMapping& Mapping = KeyPair.Value.Mappings.Array()[0];
						if (Mapping.GetCurrentKey() == InputKey)
						{
							if (UCodeOptionKey* TempOptionKey = *OptionKeys.Find(KeyPair.Key))
							{
								TempOptionKey->SetUnAssigned();
								bHasConflicts = true;
							}
						}
					}
				}
			}
		}
	}
}

void UCodeControlSettings::ApplyChangedSettings()
{
	if (bHasConflicts)
	{
		CodeQuestionScreen = CreateWidget<UCodeQuestionScreen>(GetOwningPlayer(), CodeQuestionScreenClass);
		if (!CodeQuestionScreen)
		{
			UE_LOG(LogTemp, Error, TEXT("ApplyChangedSettings: CodeQuestionScreenClass is not set!"));
			return;
		}

		CodeQuestionScreen->SetButtonCount(false);
		CodeQuestionScreen->ButtonOne->OnClicked().AddUObject(this, &UCodeControlSettings::CloseQuestionScreen);
		CodeQuestionScreen->AddToViewport();
		CodeQuestionScreen->SetQuestionText(FText::FromString("You Have Keymapping Conflict. Resolve Them First."));
		CodeQuestionScreen->SetButtonOneText(FText::FromString("Close"));
	}
	else
	{
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
				UserSettings->ApplySettings();
				UserSettings->SaveSettings();
				
				KeyMap = GetKeyMapping();
				SavedKeyMap = KeyMap;
				bHasUnsavedChanges = false;
			}
		}
	}
}

void UCodeControlSettings::ApplyChangedAndClose()
{
	CloseQuestionScreen();
	ApplyChangedSettings();
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::SettingsPanel);
	}
}

void UCodeControlSettings::BackOutWithoutSaving()
{
	for (const TPair<FName, FKey>& Pair : SavedKeyMap)
	{
		UpdateKeyMapping(Pair.Key, Pair.Value);
	}
	
	ControlsScrollBox->ClearChildren();
	bHasUnsavedChanges = false;
	SetupControlsSettings();
	CloseQuestionScreen();
	
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::SettingsPanel);
	}
}

void UCodeControlSettings::ResetKeyMapping()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			const TMap<FName, FKeyMappingRow>& Rows = Subsystem->GetUserSettings()->GetActiveKeyProfile()->GetPlayerMappingRows();
			
			for (const TPair<FName, FKeyMappingRow>& KeyPair : Rows)
			{
				if (KeyPair.Value.Mappings.Num() > 0)
				{
					const FPlayerKeyMapping& ExistingMapping = KeyPair.Value.Mappings.Array()[0];
					UpdateKeyMapping(FName(KeyPair.Key.ToString()), ExistingMapping.GetDefaultKey());
				}
			}
			ControlsScrollBox->ClearChildren();
			bHasUnsavedChanges = false;
			SetupControlsSettings();
		}
	}
}

void UCodeControlSettings::CloseQuestionScreen()
{
	CodeQuestionScreen->RemoveFromParent();
}

