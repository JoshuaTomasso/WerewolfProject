#pragma once

#include "CoreMinimal.h"
#include "ETeams.generated.h"

UENUM(BlueprintType)
enum class ETeams : uint8
{
	Werewolves UMETA(DisplayName = "Werewolves"),
	Villagers  UMETA(DisplayName = "Villagers"),
};