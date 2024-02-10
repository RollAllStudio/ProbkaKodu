// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_CopyLocationValue.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

EBTNodeResult::Type UBTTaskNode_CopyLocationValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	AActor* SourceAsActor = Cast<AActor>(BB->GetValueAsObject(SourceValue.SelectedKeyName));

	if (SourceAsActor)
	{
		BB->SetValueAsVector(TargetValue.SelectedKeyName, SourceAsActor->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}
	
	BB->SetValueAsVector(TargetValue.SelectedKeyName, BB->GetValueAsVector(SourceValue.SelectedKeyName));
	return EBTNodeResult::Succeeded;
}
