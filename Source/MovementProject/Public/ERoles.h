#pragma once

#include "CoreMinimal.h"
#include "ERoles.generated.h"

UENUM(BlueprintType)
enum class ERoles : uint8
{
	Villager   UMETA(DisplayName = "Villager"),
	Werewolf   UMETA(DisplayName = "Werewolf"),
	Seer       UMETA(DisplayName = "Seer"),
	Mayor      UMETA(DisplayName = "Mayor"),
	Medic      UMETA(DisplayName = "Medic"),
};