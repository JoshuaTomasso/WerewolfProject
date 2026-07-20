// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "EPhases.h"


#include "CodePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API ACodePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bNightWidgetActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bHasAckedReady;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bTargetListPopulated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phases")
	EPhases currentPhase;

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PlayerController")
	void Server_NotifyReady();
};
