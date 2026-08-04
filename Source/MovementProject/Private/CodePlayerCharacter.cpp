// Fill out your copyright notice in the Description page of Project Settings.

#include "CodePlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "CodePlayerController.h"

ACodePlayerCharacter::ACodePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACodePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    if (MayorRevealText)
    {
        MayorRevealText->SetVisibility(false);
	}
}

void ACodePlayerCharacter::Multicast_SetMayorRevealVisibility_Implementation(bool bIsVisible)
{
    if (MayorRevealText)
    {
        MayorRevealText->SetVisibility(bIsVisible);
    }
}

void ACodePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACodePlayerCharacter::Move);
        }

        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACodePlayerCharacter::Look);
        }

        if (TabOutAction)
        {
            EnhancedInputComponent->BindAction(TabOutAction, ETriggerEvent::Started, this, &ACodePlayerCharacter::TabOut);
		}

        if (SprintAction)
        {
            EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ACodePlayerCharacter::Sprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACodePlayerCharacter::StopSprinting);
        }

        if (CrouchAction)
        {
            EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ACodePlayerCharacter::PlayerCrouch);
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACodePlayerCharacter::PlayerUnCrouch);
        }

        if (JumpAction)
        {
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACodePlayerCharacter::PlayerJump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACodePlayerCharacter::PlayerStopJumping);
		}
    }
}

void ACodePlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ACodePlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ACodePlayerCharacter::TabOut(const FInputActionValue& Value)
{
	ACodePlayerController* CodePlayerController = Cast<ACodePlayerController>(GetController());

    if (CodePlayerController)
    {
        CodePlayerController->bShowMouseCursor = ~CodePlayerController->bShowMouseCursor;
        if (CodePlayerController->bShowMouseCursor)
        {
            CodePlayerController->SetInputMode(FInputModeGameAndUI());
        }
        else
        {
            CodePlayerController->SetInputMode(FInputModeGameOnly());
        }
	}
}

void ACodePlayerCharacter::Sprint(const FInputActionValue& Value)
{
    MovementComponent->MaxWalkSpeed = 1200.0f;

    if (!HasAuthority())
    {
        Server_SetSprinting(true);
    }
}

void ACodePlayerCharacter::StopSprinting(const FInputActionValue& Value)
{
    MovementComponent->MaxWalkSpeed = 600.0f;

    if (!HasAuthority())
    {
        Server_SetSprinting(false);
    }
}

void ACodePlayerCharacter::Server_SetSprinting_Implementation(bool bIsSprinting)
{
    if (bIsSprinting)
    {
        MovementComponent->MaxWalkSpeed = 1200.0f;
    }
    else
    {
        MovementComponent->MaxWalkSpeed = 600.0f;
    }
}

void ACodePlayerCharacter::PlayerCrouch(const FInputActionValue& Value)
{
	ACharacter::Crouch();
}

void ACodePlayerCharacter::PlayerUnCrouch(const FInputActionValue& Value)
{
    ACharacter::UnCrouch();
}

void ACodePlayerCharacter::PlayerJump(const FInputActionValue& Value)
{
    if (!bIsJumping)
    {
        bIsJumping = true;
        ACharacter::Jump();
	}
}

void ACodePlayerCharacter::PlayerStopJumping(const FInputActionValue& Value)
{
	ACharacter::StopJumping();
    bIsJumping = false;
}

