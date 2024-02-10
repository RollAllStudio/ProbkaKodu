// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "VehicleAIController.generated.h"

class UPlayerClickData;
class UHealthComponent;

UENUM(BlueprintType)
enum EVehicleStateSelector : UINT8
{
	Vss_Passive		UMETA(DisplayName = Passive),
	Vss_Defensive	UMETA(DisplayName = Defensive),
	Vss_Aggressive	UMETA(DisplayName = Aggressive)
};

class AVehicleActor;
/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API AVehicleAIController : public AAIController
{
	GENERATED_BODY()

	virtual void OnPossess(APawn* InPawn) override;

private:

	UPROPERTY(BlueprintSetter = SetPlayerDefinedTargetEnemy, BlueprintGetter = GetPlayerDefinedTargetEnemy)
	AVehicleActor* PlayerDefinedTargetEnemy;

public:

	UFUNCTION(BlueprintCallable)
	void SetBaseState(EVehicleStateSelector InNewBaseState);

	UFUNCTION(BlueprintPure)
	EVehicleStateSelector GetBaseState();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly)
	AVehicleActor* PossessedVehicle;

	UFUNCTION(BlueprintCallable)
	void SetDesiredShotTarget(AActor* InNewTarget);

	UFUNCTION(BlueprintSetter)
	void SetPlayerDefinedTargetEnemy(AVehicleActor* InVehicle);

	void UnbindPlayerDefinedTargetEnemy();

	UFUNCTION()
	void OnDefinedPlayerTargetEnemyDied(UHealthComponent* HealthComponent);

	UFUNCTION(BlueprintGetter)
	AVehicleActor* GetPlayerDefinedTargetEnemy() const {return PlayerDefinedTargetEnemy;}

	UFUNCTION()
	void OnPossessedVehicleDied(UHealthComponent* HealthComponent);

	UFUNCTION(BlueprintCallable)
	void ReceivePlayerClickData(UPlayerClickData* InClickData);

	UFUNCTION(BlueprintCallable)
	void ClearTargetMovementLocation();

	UFUNCTION(BlueprintCallable)
	void SetTargetMovementLocation(FVector InLocation);

	void SetWeaponsRangeInBlackBoard();
	
};