// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CodeMainMenuPlayerController.generated.h"

class UCodeMainMenu;

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API ACodeMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UCodeMainMenu* MainMenuWidget;
	
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeMainMenu> MainMenuWidgetClass;
	
protected:
	virtual void BeginPlay() override;
	
};
