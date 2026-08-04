// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CodePlayerCharacter.generated.h"

UCLASS()
class MOVEMENTPROJECT_API ACodePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACodePlayerCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* TabOutAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* CrouchAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    bool bIsJumping;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UCharacterMovementComponent* MovementComponent;
    
    void Move(const FInputActionValue& Value);

    void Look(const FInputActionValue& Value);

	void TabOut(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);

	void StopSprinting(const FInputActionValue& Value);

    UFUNCTION(Server, Reliable)
    void Server_SetSprinting(bool bIsSprinting);

	void PlayerCrouch(const FInputActionValue& Value);

	void PlayerUnCrouch(const FInputActionValue& Value);

	void PlayerJump(const FInputActionValue& Value);

	void PlayerStopJumping(const FInputActionValue& Value);
};
