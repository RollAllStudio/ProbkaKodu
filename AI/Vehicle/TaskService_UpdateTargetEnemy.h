// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TaskService_UpdateTargetEnemy.generated.h"

class AVehicleActor;
class AVehicleAIController;
/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API UTaskService_UpdateTargetEnemy : public UBTService
{
	GENERATED_BODY()

	UTaskService_UpdateTargetEnemy();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY()
	UBlackboardComponent* BB;
	
	UPROPERTY()
	AVehicleAIController* AIController;
	
	UPROPERTY()
	AVehicleActor* VehicleActor;

	float GetDistanceToVehicle(AVehicleActor* InVehicle);

	bool TrySelectEnemy(AVehicleActor* InEnemy);
	
#pragma region Blackboard
	
	UPROPERTY(EditAnywhere) // Input
	FBlackboardKeySelector SelfActor;

	UPROPERTY(EditAnywhere) // Output
	FBlackboardKeySelector TargetEnemyActor;

	UPROPERTY(EditAnywhere) // Output
	FBlackboardKeySelector IsPlayerDesiredEnemy;

	UPROPERTY(EditAnywhere) // Output - > set only if we can't set player and value is not set
	FBlackboardKeySelector MovementTargetLocation;

#pragma endregion 
	
};
