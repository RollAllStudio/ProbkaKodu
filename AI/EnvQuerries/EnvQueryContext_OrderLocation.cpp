// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_OrderLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "RoboCombat/AI/Vehicle/VehicleAIController.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"

void UEnvQueryContext_OrderLocation::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                    FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AVehicleActor* OwningVehicle = Cast<AVehicleActor>(QueryInstance.Owner);

	if (!OwningVehicle)
		return;
	
	AVehicleAIController* OwningAI = Cast<AVehicleAIController>(OwningVehicle->GetController());

	if (!OwningAI)
		return;

	UEnvQueryItemType_Point::SetContextHelper(ContextData, OwningAI->GetBlackboardComponent()->GetValueAsVector(TEXT("TargetLocation")));
	
}
