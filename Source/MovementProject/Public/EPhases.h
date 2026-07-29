#pragma once

#include "CoreMinimal.h"
#include "EPhases.generated.h"

UENUM(BlueprintType)
enum class EPhases : uint8
{
	RoleReveal UMETA(DisplayName = "Role Reveal"),
	Day       UMETA(DisplayName = "Day"),
	Night     UMETA(DisplayName = "Night"),
	Voting    UMETA(DisplayName = "Voting"),
	Lobby     UMETA(DisplayName = "Lobby"),
};