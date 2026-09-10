// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "CodeOptionKey.h"
#include "Components/ScrollBox.h"
#include "CodeQuestionScreen.h"
#include "CodeControlSettings.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeControlSettings : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* ResetButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ApplyButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ControlsScrollBox;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, FKey> KeyMap;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, FKey> SavedKeyMap;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, UCodeOptionKey*> OptionKeys;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCodeOptionKey> CodeOptionKeyClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCodeOptionKey* CodeOptionKey;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCodeQuestionScreen> CodeQuestionScreenClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCodeQuestionScreen* CodeQuestionScreen;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasConflicts;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasUnsavedChanges;
	
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
	void ApplyChangedSettings();
	
	UFUNCTION()
	void ApplyChangedAndClose();
	
	UFUNCTION()
	void BackOutWithoutSaving();
	
	UFUNCTION()
	void ResetKeyMapping();
	
	UFUNCTION()
	void CloseQuestionScreen();
};
