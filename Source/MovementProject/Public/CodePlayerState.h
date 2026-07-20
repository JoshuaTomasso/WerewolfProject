// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "ERoles.h"

#include "CodePlayerState.generated.h"

class ACodePlayerController;


/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API ACodePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ints")
	int voteErrorCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player States")
	ACodePlayerState* nightTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player States")
	ACodePlayerState* playerVote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player States")
	ACodePlayerState* werewolfPartner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Controllers")
	ACodePlayerController* owningPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bIsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bHasSubmittedNightAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bIsProtected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	ERoles currentRole;
};
