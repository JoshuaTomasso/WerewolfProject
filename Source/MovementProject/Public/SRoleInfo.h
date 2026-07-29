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
	ERoles role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETeams team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText displayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int nightActionOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanActAtNight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAnnounceRole;

};