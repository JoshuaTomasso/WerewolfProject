#pragma once

#include "CoreMinimal.h"
#include "SWerewolfPack.generated.h"

USTRUCT(BlueprintType)
struct FSWerewolfPack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Werewolf Pack")
	APlayerState* WolfOne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Werewolf Pack")
	APlayerState* WolfTwo;

};