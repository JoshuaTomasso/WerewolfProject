// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "CodeRevealeRole.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeRevealeRole : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UButton* RevealeRoleButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRevealeRoleButtonPressed();

};
