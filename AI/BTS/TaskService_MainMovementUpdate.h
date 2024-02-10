// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TaskService_MainMovementUpdate.generated.h"

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API UPickMovementGoal : public UBTService
{
	GENERATED_BODY()

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Refs)
	FBlackboardKeySelector SelfActor;

	UPROPERTY(EditAnywhere, Category = Refs)
	FBlackboardKeySelector TargetEnemy;
	
	UPROPERTY(EditAnywhere, Category = Output)
	FBlackboardKeySelector MovementGoal;

	UPROPERTY(EditAnywhere, Category = Output)
	FBlackboardKeySelector MovementGoalDistance;

	UPROPERTY(EditAnywhere, Category = Input)
	FBlackboardKeySelector FightMovementLocation;

	UPROPERTY(EditAnywhere, Category = Input)
	FBlackboardKeySelector OrderTargetLocation;
	
};