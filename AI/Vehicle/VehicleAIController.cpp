// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleAIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RoboCombat/HealthAndDamage/Public/VehicleHealthComponent.h"
#include "RoboCombat/Player/ClickData/PlayerClickData.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"
#include "RoboCombat/VehicleFactory/VehicleFactory.h"

void AVehicleAIController::OnPossess(APawn* InPawn)
{
	PossessedVehicle = Cast<AVehicleActor>(InPawn);

	if (PossessedVehicle)
	{
		RunBehaviorTree(BehaviorTree);
		PossessedVehicle->SetVehicleAI(this);
		PossessedVehicle->HealthComponent->OnHealthDepleted.AddUniqueDynamic(this, &AVehicleAIController::OnPossessedVehicleDied);

		AVehicleFactory* VehicleOwningFactory = Cast<AVehicleFactory>(PossessedVehicle->GetOwner());

		if (VehicleOwningFactory)
		{
			SetTargetMovementLocation(VehicleOwningFactory->VehicleDefaultTarget->GetComponentLocation());
			PossessedVehicle->SetOwner(this);
		}
	}

	Super::OnPossess(InPawn);
	
}

void AVehicleAIController::SetBaseState(EVehicleStateSelector InNewBaseState)
{
	GetBlackboardComponent()->SetValueAsEnum(TEXT("BaseState"), InNewBaseState);
	PossessedVehicle->UpdateOrderVisualisationMesh(InNewBaseState);
}

EVehicleStateSelector AVehicleAIController::GetBaseState()
{
	return static_cast<EVehicleStateSelector>(GetBlackboardComponent()->GetValueAsEnum(TEXT("BaseState")));
}

void AVehicleAIController::SetDesiredShotTarget(AActor* InNewTarget)
{
	PossessedVehicle->SetDesiredShotTarget(InNewTarget);
}

void AVehicleAIController::SetPlayerDefinedTargetEnemy(AVehicleActor* InVehicle)
{
	if (InVehicle == PlayerDefinedTargetEnemy)
	return;

	UnbindPlayerDefinedTargetEnemy();
	PlayerDefinedTargetEnemy = InVehicle;
	
	if (PlayerDefinedTargetEnemy)
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("bIsPlayerDesiredTargetEnemy"), true);
		GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerDesiredTargetEnemy"), InVehicle);
		PlayerDefinedTargetEnemy->HealthComponent->OnHealthDepleted.AddUniqueDynamic(this, &AVehicleAIController::OnDefinedPlayerTargetEnemyDied);
		return;
	}

	GetBlackboardComponent()->ClearValue("bIsPlayerDesiredTargetEnemy");
	GetBlackboardComponent()->ClearValue(TEXT("PlayerDesiredTargetEnemy"));
	
}

void AVehicleAIController::UnbindPlayerDefinedTargetEnemy()
{
	if (PlayerDefinedTargetEnemy)
	PlayerDefinedTargetEnemy->HealthComponent->OnHealthDepleted.RemoveDynamic(this, &AVehicleAIController::OnDefinedPlayerTargetEnemyDied);
}

void AVehicleAIController::OnDefinedPlayerTargetEnemyDied(UHealthComponent* HealthComponent)
{
	SetPlayerDefinedTargetEnemy(nullptr);
	PossessedVehicle->UpdateOrderVisualisationMesh(GetBaseState());
	PossessedVehicle->UpdateOrderVisualisationLocation(PossessedVehicle->GetActorLocation());
}

void AVehicleAIController::OnPossessedVehicleDied(UHealthComponent* HealthComponent)
{
	BrainComponent->StopLogic(TEXT("VehicleDied"));
}

void AVehicleAIController::ReceivePlayerClickData(UPlayerClickData* InClickData)
{
	AVehicleActor* ClickedVehicle = Cast<AVehicleActor>(InClickData->GetHitActor());

	if (ClickedVehicle)
	{
		if (!ClickedVehicle->PlayerIndex == PossessedVehicle->PlayerIndex)
		{
			SetPlayerDefinedTargetEnemy(ClickedVehicle);
			ClearTargetMovementLocation();
			PossessedVehicle->UpdateOrderVisualisationMesh(EVehicleStateSelector::Vss_Aggressive);
			return;
		}
	}

	SetPlayerDefinedTargetEnemy(nullptr);
	SetTargetMovementLocation(InClickData->GetHitLocation());
	PossessedVehicle->UpdateOrderVisualisationMesh(GetBaseState());
}

void AVehicleAIController::ClearTargetMovementLocation()
{
	GetBlackboardComponent()->ClearValue(TEXT("TargetLocation"));
}

void AVehicleAIController::SetTargetMovementLocation(FVector InLocation)
{	
	GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), InLocation);
	PossessedVehicle->UpdateOrderVisualisationLocation(InLocation);
}

void AVehicleAIController::SetWeaponsRangeInBlackBoard()
{

	float MinRange = PossessedVehicle->GetWeaponMinRange();
	float MaxRange = PossessedVehicle->GetWeaponMaxRange();
	
	GetBlackboardComponent()->SetValueAsFloat(TEXT("MinWeaponRange"), MinRange);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("MaxWeaponRange"), MaxRange);

	GetBlackboardComponent()->SetValueAsFloat(TEXT("AggressiveMinDistance"), MinRange * 0.25);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AggressiveMaxDistance"), MaxRange * 0.75);
	
}
