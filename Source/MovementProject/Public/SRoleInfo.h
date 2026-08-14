#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ERoles.h"
#include "ETeams.h"
#include "SRoleInfo.generated.h"

USTRUCT(BlueprintType)
struct FSRoleInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoles Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETeams Team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NightActionOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanActAtNight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAnnounceRole;

};