// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_CopyLocationValue.generated.h"

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API UBTTaskNode_CopyLocationValue : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetValue;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SourceValue;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
