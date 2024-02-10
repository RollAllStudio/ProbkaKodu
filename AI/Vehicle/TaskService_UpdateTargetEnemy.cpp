// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskService_UpdateTargetEnemy.h"
#include "VehicleAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"

UTaskService_UpdateTargetEnemy::UTaskService_UpdateTargetEnemy()
{
	NodeName = TEXT("Update target enemy");
}

void UTaskService_UpdateTargetEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	BB = OwnerComp.GetBlackboardComponent();
	AIController = Cast<AVehicleAIController>(OwnerComp.GetAIOwner());
	VehicleActor = Cast<AVehicleActor>(BB->GetValueAsObject(SelfActor.SelectedKeyName));

	if (AIController && VehicleActor)
	{
		if (AIController->GetPlayerDefinedTargetEnemy())
			AIController->PossessedVehicle->UpdateOrderVisualisationLocation(AIController->GetPlayerDefinedTargetEnemy()->GetActorLocation());
		
		if (VehicleActor->GetSeenEnemies().IsEmpty())
		{
			
			AIController->SetDesiredShotTarget(nullptr);
			
			BB->ClearValue(TargetEnemyActor.SelectedKeyName);
			
			if (!BB->IsVectorValueSet(MovementTargetLocation.SelectedKeyName) && !BB->GetValueAsBool(IsPlayerDesiredEnemy.SelectedKeyName))
			BB->SetValueAsVector(MovementTargetLocation.SelectedKeyName, VehicleActor->GetActorLocation());
			
			return;
		}
		
		if (TrySelectEnemy(AIController->GetPlayerDefinedTargetEnemy()))
		{
			BB->SetValueAsBool(IsPlayerDesiredEnemy.SelectedKeyName, true);
			return;
		}

		BB->ClearValue(IsPlayerDesiredEnemy.SelectedKeyName);
		
		if (TrySelectEnemy(Cast<AVehicleActor>(BB->GetValueAsObject(TargetEnemyActor.SelectedKeyName))))
		return;

		// Gather distances to seen enemies
		TArray<float> Distances;
		for (int i = 0; i < VehicleActor->GetSeenEnemies().Num(); i++)
		Distances.Add(GetDistanceToVehicle(VehicleActor->GetSeenEnemies()[i]));

		// Get nearest enemy index (in seen enemies array)
		int32 Index;
		float MinDistance;
		UKismetMathLibrary::MinOfFloatArray(Distances, Index, MinDistance);

		// Select nearest enemy (always return true, because actor in seen enemies array must pass this test)
		TrySelectEnemy(VehicleActor->GetSeenEnemies()[Index]);
	}
}

float UTaskService_UpdateTargetEnemy::GetDistanceToVehicle(AVehicleActor* InVehicle)
{
	return UKismetMathLibrary::Vector_Distance(VehicleActor->GetActorLocation(), InVehicle->GetActorLocation());
}

bool UTaskService_UpdateTargetEnemy::TrySelectEnemy(AVehicleActor* InEnemy)
{
	if (VehicleActor->SeeEnemy(InEnemy))
	{
		BB->SetValueAsObject(TargetEnemyActor.SelectedKeyName, InEnemy);
		AIController->SetDesiredShotTarget(InEnemy);
		return true;
	}

	return false;
	
}
