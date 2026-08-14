// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ERoles.h"
#include "CodePlayerState.generated.h"

class UCodePlayerRoleDiscription;
class UCodeWerewolfPartnerReveal;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoleAssigned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnErrorCountChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNightTargetChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetDeadChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVoteTargetChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVotesOnPlayerChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHasRevealedRoleChanged);

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API ACodePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floats")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ints", Replicated, ReplicatedUsing = OnRep_VoteErrorCount)
	int VoteErrorCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ints", Replicated, ReplicatedUsing = OnRep_TargetDeadCount)
	int TargetDeadCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ints", Replicated, ReplicatedUsing = OnRep_VotesOnPlayer)
	int VotesOnPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ints")
	int SelfProtectedCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ints")
	int SeerAbilityCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player States", Replicated, ReplicatedUsing = OnRep_NightTarget)
	ACodePlayerState* NightTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player States", Replicated, ReplicatedUsing = OnRep_VoteTarget)
	ACodePlayerState* VoteTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player States")
	ACodePlayerState* PlayerVote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player States", Replicated)
	ACodePlayerState* WerewolfPartner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools", Replicated)
	bool bIsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools", Replicated)
	bool bHasSubmittedNightAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools", Replicated)
	bool bHasSubmittedVote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bIsProtected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools", Replicated, ReplicatedUsing = OnRep_HasRevealedRole)
	bool bHasRevealedRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role", Replicated, ReplicatedUsing = OnRep_CurrentRole)
	ERoles CurrentRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	class UDataTable* RoleDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodePlayerRoleDiscription> RoleDescriptionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeWerewolfPartnerReveal> WerewolfPartnerRevealWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	FTimerHandle ShowWidgetTimerHandle;

	UPROPERTY(BlueprintAssignable, Category = "Role")
	FOnRoleAssigned OnRoleAssigned;

	UPROPERTY(BlueprintAssignable, Category = "Error")
	FOnErrorCountChanged OnErrorCountChanged;

	UPROPERTY(BlueprintAssignable, Category = "Night Target")
	FOnNightTargetChanged OnNightTargetChanged;

	UPROPERTY(BlueprintAssignable, Category = "Night Target Dead")
	FOnTargetDeadChanged OnTargetDeadCountChanged;

	UPROPERTY(BlueprintAssignable, Category = "Vote Target")
	FOnVoteTargetChanged OnVoteTargetChanged;

	UPROPERTY(BlueprintAssignable, Category = "Votes On Player")
	FOnVotesOnPlayerChanged OnVotesOnPlayerChanged;

	UPROPERTY(BlueprintAssignable, Category = "Role Revealed")
	FOnHasRevealedRoleChanged OnHasRevealedRoleChanged;

	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "GameState")
	void Client_ReceiveRole(ERoles RoleToReveal);

	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "GameState")
	void Client_ReceiveWerewolfPartner(const FText& PartnerName);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GameState")
	void Server_SubmitNightAction(ACodePlayerState* AbilityTarget);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GameState")
	void Server_SubmitVote(ACodePlayerState* AbilityTarget);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GameState")
	void Server_RevealRole();

	UFUNCTION()
	void OnRep_CurrentRole();

	UFUNCTION()
	void OnRep_VoteErrorCount();

	UFUNCTION()
	void OnRep_NightTarget();

	UFUNCTION()
	void OnRep_TargetDeadCount();

	UFUNCTION()
	void OnRep_VoteTarget();

	UFUNCTION()
	void OnRep_VotesOnPlayer();

	UFUNCTION()
	void OnRep_HasRevealedRole();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UFUNCTION()
	void OnShowWidgetTimer(const FText& partnerName);
};
