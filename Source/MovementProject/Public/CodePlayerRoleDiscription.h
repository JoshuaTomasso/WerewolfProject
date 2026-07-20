// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CodePlayerRoleDiscription.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodePlayerRoleDiscription : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoleText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoleDetails;
};
