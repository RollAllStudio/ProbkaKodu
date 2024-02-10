// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ClearValue.generated.h"

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API UBTTaskNode_ClearValue : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector InValue;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
