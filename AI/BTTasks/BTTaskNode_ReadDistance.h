// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ReadDistance.generated.h"

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API UBTTaskNode_ReadDistance : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector From;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector To;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector OutDistance;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FVector ReturnVectorFromBlackboard(UBlackboardComponent* BB, FBlackboardKeySelector BBKey);
	
};
