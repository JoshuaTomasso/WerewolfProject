#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "CodeOptionKey.h"
#include "CodeQuestionScreen.h"
#include "CodeControlSettings.generated.h"

UCLASS()
class MOVEMENTPROJECT_API UCodeControlSettings : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResetButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ApplyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ControlsScrollBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	TMap<FName, FKey> KeyMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	TMap<FName, FKey> SavedKeyMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	TMap<FName, TObjectPtr<UCodeOptionKey>> OptionKeys;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<UCodeOptionKey> CodeOptionKeyClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TObjectPtr<UCodeOptionKey> CodeOptionKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<UCodeQuestionScreen> CodeQuestionScreenClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TObjectPtr<UCodeQuestionScreen> CodeQuestionScreen;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	bool bHasConflicts = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	bool bHasUnsavedChanges = false;

protected:

	virtual void NativeConstruct() override;

public:

	UFUNCTION()
	void ResetButtonClicked();

	UFUNCTION()
	void ApplyButtonClicked();

	UFUNCTION()
	void BackButtonClicked();

	UFUNCTION()
	void SetupControlsSettings();

	UFUNCTION()
	TMap<FName, FKey> GetKeyMapping();

	UFUNCTION()
	void UpdateKeyMapping(FName InputMappingKey, FKey InputKey);

	UFUNCTION()
	void CheckKeyConflicts(FName InputMappingKey, FKey InputKey);

	UFUNCTION()
	void ResetKeyMapping();

	UFUNCTION()
	void ApplyChangedSettings();

	UFUNCTION()
	void ApplyChangedAndClose();

	UFUNCTION()
	void BackOutWithoutSaving();

	UFUNCTION()
	void CloseQuestionScreen();
};