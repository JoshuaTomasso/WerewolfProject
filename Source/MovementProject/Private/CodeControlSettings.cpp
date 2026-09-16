#include "CodeControlSettings.h"

#include "CodeMainMenu.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"


void UCodeControlSettings::NativeConstruct()
{
	Super::NativeConstruct();

	if (ResetButton)
	{
		ResetButton->OnPressed.AddDynamic(this, &UCodeControlSettings::ResetButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeConstruct: ResetButton is null!"));
	}

	if (ApplyButton)
	{
		ApplyButton->OnPressed.AddDynamic(this, &UCodeControlSettings::ApplyButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeConstruct: ApplyButton is null!"));
	}

	if (BackButton)
	{
		BackButton->OnPressed.AddDynamic(this, &UCodeControlSettings::BackButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeConstruct: BackButton is null!"));
	}

	if (!ControlsScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("NativeConstruct: ControlsScrollBox is null!"));
		return;
	}

	ControlsScrollBox->ClearChildren();

	bHasUnsavedChanges = false;
	bHasConflicts = false;

	OptionKeys.Empty();
	KeyMap.Empty();
	SavedKeyMap.Empty();

	SetupControlsSettings();

	SavedKeyMap = KeyMap;

	UE_LOG(LogTemp, Log, TEXT("NativeConstruct: Control settings initialized with %d mappings."), KeyMap.Num());
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
		if (!CodeQuestionScreenClass)
		{
			UE_LOG(LogTemp, Error, TEXT("BackButtonClicked: CodeQuestionScreenClass is not set!"));
			return;
		}

		APlayerController* OwningPlayer = GetOwningPlayer();

		if (!OwningPlayer)
		{
			UE_LOG(LogTemp, Error, TEXT("BackButtonClicked: GetOwningPlayer() returned null!"));
			return;
		}

		if (CodeQuestionScreen)
		{
			CodeQuestionScreen->RemoveFromParent();
			CodeQuestionScreen = nullptr;
		}

		CodeQuestionScreen = CreateWidget<UCodeQuestionScreen>(OwningPlayer, CodeQuestionScreenClass);

		if (!CodeQuestionScreen)
		{
			UE_LOG(LogTemp, Error, TEXT("BackButtonClicked: Failed to create CodeQuestionScreen!"));
			return;
		}

		CodeQuestionScreen->SetButtonCount(true);

		if (CodeQuestionScreen->ButtonTwo)
		{
			CodeQuestionScreen->ButtonTwo->OnClicked().AddUObject(this, &UCodeControlSettings::BackOutWithoutSaving);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BackButtonClicked: CodeQuestionScreen->ButtonTwo is null!"));
		}

		if (CodeQuestionScreen->ButtonOne)
		{
			CodeQuestionScreen->ButtonOne->OnClicked().AddUObject(this, &UCodeControlSettings::ApplyChangedAndClose);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BackButtonClicked: CodeQuestionScreen->ButtonOne is null!"));
		}

		CodeQuestionScreen->SetQuestionText(FText::FromString(TEXT("You Have Unsaved Changes.")));
		CodeQuestionScreen->SetButtonOneText(FText::FromString(TEXT("Apply")));
		CodeQuestionScreen->SetButtonTwoText(FText::FromString(TEXT("Discard")));

		CodeQuestionScreen->AddToViewport();

		return;
	}

	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::SettingsPanel);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BackButtonClicked: No UCodeMainMenu outer found!"));
	}
}

void UCodeControlSettings::SetupControlsSettings()
{
	if (!ControlsScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupControlsSettings: ControlsScrollBox is null!"));
		return;
	}

	if (!CodeOptionKeyClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupControlsSettings: CodeOptionKeyClass is not set!"));
		return;
	}

	APlayerController* OwningPlayer = GetOwningPlayer();

	if (!OwningPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupControlsSettings: GetOwningPlayer() returned null!"));
		return;
	}

	OptionKeys.Empty();
	KeyMap = GetKeyMapping();

	UE_LOG(LogTemp, Log, TEXT("SetupControlsSettings: Retrieved %d input mappings."), KeyMap.Num());

	for (const TPair<FName, FKey>& KeyPair : KeyMap)
	{
		UCodeOptionKey* NewOptionKey = CreateWidget<UCodeOptionKey>(OwningPlayer, CodeOptionKeyClass);

		if (!NewOptionKey)
		{
			UE_LOG(LogTemp, Error, TEXT("SetupControlsSettings: Failed to create CodeOptionKey for %s"), *KeyPair.Key.ToString());
			continue;
		}

		OptionKeys.Add(KeyPair.Key, NewOptionKey);

		NewOptionKey->DefaultInputKey = KeyPair.Value;
		NewOptionKey->DefaultInputName = FText::FromName(KeyPair.Key);
		NewOptionKey->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));

		ControlsScrollBox->AddChild(NewOptionKey);

		NewOptionKey->OnKeyChangedDispatch.AddDynamic(this, &UCodeControlSettings::UpdateKeyMapping);
	}
}

TMap<FName, FKey> UCodeControlSettings::GetKeyMapping()
{
	TMap<FName, FKey> TempKeyMap;

	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetKeyMapping: GetOwningPlayer() returned null!"));
		return TempKeyMap;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetKeyMapping: GetLocalPlayer() returned null!"));
		return TempKeyMap;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetKeyMapping: EnhancedInputLocalPlayerSubsystem not found!"));
		return TempKeyMap;
	}

	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();

	if (!UserSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("GetKeyMapping: GetUserSettings() returned null!"));
		return TempKeyMap;
	}

	if (DefaultMappingContext)
	{
		UserSettings->RegisterInputMappingContexts({ DefaultMappingContext });
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetKeyMapping: DefaultMappingContext is null!"));
	}

	UEnhancedPlayerMappableKeyProfile* Profile = UserSettings->GetActiveKeyProfile();

	if (!Profile)
	{
		UE_LOG(LogTemp, Error, TEXT("GetKeyMapping: No active key profile found!"));
		return TempKeyMap;
	}

	const TMap<FName, FKeyMappingRow>& Rows = Profile->GetPlayerMappingRows();

	for (const TPair<FName, FKeyMappingRow>& KeyPair : Rows)
	{
		for (const FPlayerKeyMapping& Mapping : KeyPair.Value.Mappings)
		{
			const FKey CurrentKey = Mapping.GetCurrentKey();

			if (!CurrentKey.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("GetKeyMapping: Invalid key for mapping %s."), *KeyPair.Key.ToString());
				continue;
			}

			TempKeyMap.Add(KeyPair.Key, CurrentKey);
			break;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("GetKeyMapping: Returning %d mappings."), TempKeyMap.Num());

	return TempKeyMap;
}

void UCodeControlSettings::UpdateKeyMapping(FName InputMappingKey, FKey InputKey)
{
	if (InputMappingKey.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateKeyMapping: InputMappingKey is None!"));
		return;
	}

	if (!InputKey.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateKeyMapping: InputKey is invalid for %s"), *InputMappingKey.ToString());
		return;
	}

	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateKeyMapping: GetOwningPlayer() returned null!"));
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateKeyMapping: GetLocalPlayer() returned null!"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateKeyMapping: EnhancedInputLocalPlayerSubsystem not found!"));
		return;
	}

	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();

	if (!UserSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateKeyMapping: GetUserSettings() returned null!"));
		return;
	}

	UEnhancedPlayerMappableKeyProfile* Profile = UserSettings->GetActiveKeyProfile();

	if (!Profile)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateKeyMapping: No active key profile found!"));
		return;
	}

	const TMap<FName, FKeyMappingRow>& Rows = Profile->GetPlayerMappingRows();
	const FKeyMappingRow* Row = Rows.Find(InputMappingKey);

	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateKeyMapping: Mapping row not found: %s"), *InputMappingKey.ToString());
		return;
	}

	for (const FPlayerKeyMapping& ExistingMapping : Row->Mappings)
	{
		const FKey CurrentKey = ExistingMapping.GetCurrentKey();

		if (InputKey == CurrentKey)
		{
			UE_LOG(LogTemp, Verbose, TEXT("UpdateKeyMapping: %s already uses %s."), *InputMappingKey.ToString(), *InputKey.ToString());
			return;
		}

		CheckKeyConflicts(InputMappingKey, InputKey);

		FMapPlayerKeyArgs Args;
		Args.MappingName = InputMappingKey;
		Args.Slot = ExistingMapping.GetSlot();
		Args.NewKey = InputKey;
		Args.HardwareDeviceId = ExistingMapping.GetHardwareDeviceId().HardwareDeviceIdentifier;

		FGameplayTagContainer FailureReason;

		UserSettings->MapPlayerKey(Args, FailureReason);

		if (FailureReason.IsEmpty())
		{
			bHasUnsavedChanges = true;
			KeyMap.Add(InputMappingKey, InputKey);

			UE_LOG(LogTemp, Log, TEXT("UpdateKeyMapping: Successfully changed %s to %s."), *InputMappingKey.ToString(), *InputKey.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UpdateKeyMapping: MapPlayerKey failed for %s."), *InputMappingKey.ToString());
		}

		break;
	}
}

void UCodeControlSettings::CheckKeyConflicts(FName InputMappingKey, FKey InputKey)
{
	bHasConflicts = false;

	if (InputMappingKey.IsNone())
	{
		return;
	}

	if (!InputKey.IsValid())
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckKeyConflicts: GetOwningPlayer() returned null!"));
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckKeyConflicts: GetLocalPlayer() returned null!"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckKeyConflicts: EnhancedInputLocalPlayerSubsystem not found!"));
		return;
	}

	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();

	if (!UserSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("CheckKeyConflicts: GetUserSettings() returned null!"));
		return;
	}

	UEnhancedPlayerMappableKeyProfile* Profile = UserSettings->GetActiveKeyProfile();

	if (!Profile)
	{
		UE_LOG(LogTemp, Error, TEXT("CheckKeyConflicts: No active key profile found!"));
		return;
	}

	const TMap<FName, FKeyMappingRow>& Rows = Profile->GetPlayerMappingRows();

	for (const TPair<FName, FKeyMappingRow>& KeyPair : Rows)
	{
		if (KeyPair.Key == InputMappingKey)
		{
			continue;
		}

		for (const FPlayerKeyMapping& Mapping : KeyPair.Value.Mappings)
		{
			if (Mapping.GetCurrentKey() == InputKey)
			{
				UE_LOG(LogTemp, Log, TEXT("CheckKeyConflicts: %s conflicts with %s."), *InputMappingKey.ToString(), *KeyPair.Key.ToString());

				if (TObjectPtr<UCodeOptionKey>* FoundOptionKey = OptionKeys.Find(KeyPair.Key))
				{
					UCodeOptionKey* OptionWidget = *FoundOptionKey;

					if (OptionWidget)
					{
						OptionWidget->SetUnAssigned();
						bHasConflicts = true;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("CheckKeyConflicts: Option widget is null for %s."), *KeyPair.Key.ToString());
					}
				}

				break;
			}
		}
	}
}

void UCodeControlSettings::ApplyChangedSettings()
{
	if (bHasConflicts)
	{
		if (!CodeQuestionScreenClass)
		{
			UE_LOG(LogTemp, Error, TEXT("ApplyChangedSettings: CodeQuestionScreenClass is not set!"));
			return;
		}

		APlayerController* OwningPlayer = GetOwningPlayer();

		if (!OwningPlayer)
		{
			UE_LOG(LogTemp, Error, TEXT("ApplyChangedSettings: GetOwningPlayer() returned null!"));
			return;
		}

		if (CodeQuestionScreen)
		{
			CodeQuestionScreen->RemoveFromParent();
			CodeQuestionScreen = nullptr;
		}

		CodeQuestionScreen = CreateWidget<UCodeQuestionScreen>(OwningPlayer, CodeQuestionScreenClass);

		if (!CodeQuestionScreen)
		{
			UE_LOG(LogTemp, Error, TEXT("ApplyChangedSettings: Failed to create CodeQuestionScreen!"));
			return;
		}

		CodeQuestionScreen->SetButtonCount(false);

		if (CodeQuestionScreen->ButtonOne)
		{
			CodeQuestionScreen->ButtonOne->OnClicked().AddUObject(this, &UCodeControlSettings::CloseQuestionScreen);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ApplyChangedSettings: CodeQuestionScreen->ButtonOne is null!"));
		}

		CodeQuestionScreen->SetQuestionText(FText::FromString(TEXT("You Have Keymapping Conflict. Resolve Them First.")));
		CodeQuestionScreen->SetButtonOneText(FText::FromString(TEXT("Close")));

		CodeQuestionScreen->AddToViewport();

		return;
	}

	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyChangedSettings: GetOwningPlayer() returned null!"));
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyChangedSettings: GetLocalPlayer() returned null!"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyChangedSettings: EnhancedInputLocalPlayerSubsystem not found!"));
		return;
	}

	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();

	if (!UserSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyChangedSettings: GetUserSettings() returned null!"));
		return;
	}

	UserSettings->ApplySettings();
	UserSettings->SaveSettings();

	KeyMap = GetKeyMapping();
	SavedKeyMap = KeyMap;
	bHasUnsavedChanges = false;
	bHasConflicts = false;

	UE_LOG(LogTemp, Log, TEXT("ApplyChangedSettings: Settings successfully applied and saved. %d mappings."), KeyMap.Num());
}

void UCodeControlSettings::ApplyChangedAndClose()
{
	if (bHasConflicts)
	{
		ApplyChangedSettings();
		return;
	}

	ApplyChangedSettings();

	if (bHasUnsavedChanges)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyChangedAndClose: Settings still have unsaved changes. Not closing."));
		return;
	}

	CloseQuestionScreen();

	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::SettingsPanel);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyChangedAndClose: No UCodeMainMenu outer found!"));
	}
}

void UCodeControlSettings::BackOutWithoutSaving()
{
	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("BackOutWithoutSaving: GetOwningPlayer() returned null!"));
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("BackOutWithoutSaving: GetLocalPlayer() returned null!"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("BackOutWithoutSaving: EnhancedInputLocalPlayerSubsystem not found!"));
		return;
	}

	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();

	if (!UserSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("BackOutWithoutSaving: GetUserSettings() returned null!"));
		return;
	}

	UEnhancedPlayerMappableKeyProfile* Profile = UserSettings->GetActiveKeyProfile();

	if (!Profile)
	{
		UE_LOG(LogTemp, Error, TEXT("BackOutWithoutSaving: No active key profile found!"));
		return;
	}

	const TMap<FName, FKeyMappingRow>& Rows = Profile->GetPlayerMappingRows();

	for (const TPair<FName, FKey>& Pair : SavedKeyMap)
	{
		const FKeyMappingRow* Row = Rows.Find(Pair.Key);

		if (!Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("BackOutWithoutSaving: Mapping row not found for %s."), *Pair.Key.ToString());
			continue;
		}

		for (const FPlayerKeyMapping& ExistingMapping : Row->Mappings)
		{
			FMapPlayerKeyArgs Args;
			Args.MappingName = Pair.Key;
			Args.Slot = ExistingMapping.GetSlot();
			Args.NewKey = Pair.Value;
			Args.HardwareDeviceId = ExistingMapping.GetHardwareDeviceId().HardwareDeviceIdentifier;

			FGameplayTagContainer FailureReason;

			UserSettings->MapPlayerKey(Args, FailureReason);

			if (!FailureReason.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("BackOutWithoutSaving: Failed to restore %s."), *Pair.Key.ToString());
			}

			break;
		}
	}

	UserSettings->ApplySettings();
	KeyMap = GetKeyMapping();
	bHasUnsavedChanges = false;
	bHasConflicts = false;

	if (ControlsScrollBox)
	{
		ControlsScrollBox->ClearChildren();
		SetupControlsSettings();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BackOutWithoutSaving: ControlsScrollBox is null!"));
	}

	CloseQuestionScreen();

	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::SettingsPanel);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BackOutWithoutSaving: No UCodeMainMenu outer found!"));
	}
}

void UCodeControlSettings::ResetKeyMapping()
{
	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResetKeyMapping: GetOwningPlayer() returned null!"));
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResetKeyMapping: GetLocalPlayer() returned null!"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResetKeyMapping: EnhancedInputLocalPlayerSubsystem not found!"));
		return;
	}

	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();

	if (!UserSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("ResetKeyMapping: GetUserSettings() returned null!"));
		return;
	}

	UEnhancedPlayerMappableKeyProfile* Profile = UserSettings->GetActiveKeyProfile();

	if (!Profile)
	{
		UE_LOG(LogTemp, Error, TEXT("ResetKeyMapping: No active key profile found!"));
		return;
	}

	const TMap<FName, FKeyMappingRow>& Rows = Profile->GetPlayerMappingRows();

	for (const TPair<FName, FKeyMappingRow>& KeyPair : Rows)
	{
		for (const FPlayerKeyMapping& ExistingMapping : KeyPair.Value.Mappings)
		{
			const FKey DefaultKey = ExistingMapping.GetDefaultKey();

			if (!DefaultKey.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("ResetKeyMapping: Invalid default key for %s."), *KeyPair.Key.ToString());
				continue;
			}

			FMapPlayerKeyArgs Args;
			Args.MappingName = KeyPair.Key;
			Args.Slot = ExistingMapping.GetSlot();
			Args.NewKey = DefaultKey;
			Args.HardwareDeviceId = ExistingMapping.GetHardwareDeviceId().HardwareDeviceIdentifier;

			FGameplayTagContainer FailureReason;

			UserSettings->MapPlayerKey(Args, FailureReason);

			if (!FailureReason.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("ResetKeyMapping: Failed to reset %s."), *KeyPair.Key.ToString());
			}

			break;
		}
	}

	UserSettings->ApplySettings();
	KeyMap = GetKeyMapping();
	bHasConflicts = false;
	bHasUnsavedChanges = true;

	if (!ControlsScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("ResetKeyMapping: ControlsScrollBox is null!"));
		return;
	}

	ControlsScrollBox->ClearChildren();
	SetupControlsSettings();

	UE_LOG(LogTemp, Log, TEXT("ResetKeyMapping: All input mappings reset to defaults."));
}

void UCodeControlSettings::CloseQuestionScreen()
{
	if (!CodeQuestionScreen)
	{
		return;
	}

	CodeQuestionScreen->RemoveFromParent();
	CodeQuestionScreen = nullptr;
}