// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ClearValue.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_ClearValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->ClearValue(InValue.SelectedKeyName);
	OnTaskFinished(OwnerComp, NodeMemory, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
