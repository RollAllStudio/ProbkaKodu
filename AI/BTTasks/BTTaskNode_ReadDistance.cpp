// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ReadDistance.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_ReadDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	FVector From_AsVector = ReturnVectorFromBlackboard(BB, From);
	FVector To_AsVector = ReturnVectorFromBlackboard(BB, To);

	float Distance = FVector::Distance(From_AsVector, To_AsVector);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(OutDistance.SelectedKeyName, Distance);
	
	return EBTNodeResult::Succeeded;
}

FVector UBTTaskNode_ReadDistance::ReturnVectorFromBlackboard(UBlackboardComponent* BB, FBlackboardKeySelector BBKey)
{
	AActor* KeyActor = Cast<AActor>(BB->GetValueAsObject(BBKey.SelectedKeyName));
	if (KeyActor)
		return KeyActor->GetActorLocation();

	return BB->GetValueAsVector(BBKey.SelectedKeyName);
}
