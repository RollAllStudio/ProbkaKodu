// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskService_MainMovementUpdate.h"
#include "BehaviorTree/BlackboardComponent.h"

void UPickMovementGoal::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	FVector NewGoalLocation;
	const AActor* LocalSelfActor = Cast<AActor>(BB->GetValueAsObject(SelfActor.SelectedKeyName));  

	if (TargetEnemy.IsSet())
		NewGoalLocation = BB->GetValueAsVector(FightMovementLocation.SelectedKeyName);
	else
		NewGoalLocation = BB->GetValueAsVector(OrderTargetLocation.SelectedKeyName);

	BB->SetValueAsVector(MovementGoal.SelectedKeyName, NewGoalLocation);

	const float Distance = FVector::Dist2D(BB->GetValueAsVector(MovementGoal.SelectedKeyName), LocalSelfActor->GetActorLocation());
	BB->SetValueAsFloat(MovementGoalDistance.SelectedKeyName, Distance);
	
}
