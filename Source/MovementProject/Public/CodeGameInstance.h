// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CodeGameInstance.generated.h"

class UCodeSessionManager;

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	virtual void Init() override;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	UCodeSessionManager* SessionManager;
};
