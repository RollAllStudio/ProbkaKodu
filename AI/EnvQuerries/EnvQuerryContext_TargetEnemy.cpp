// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQuerryContext_TargetEnemy.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "RoboCombat/AI/Vehicle/VehicleAIController.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"

class UEnvQueryItemType_Actor;

void UEnvQuerryContext_TargetEnemy::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                   FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AVehicleActor* OwningVehicle = Cast<AVehicleActor>(QueryInstance.Owner);

	if (!OwningVehicle)
	return;
	
	AVehicleAIController* OwningAI = Cast<AVehicleAIController>(OwningVehicle->GetController());

	if (!OwningAI)
	return;

	if (OwningAI->GetPlayerDefinedTargetEnemy())
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, OwningAI->GetPlayerDefinedTargetEnemy());
		return;
	}
	
	if (OwningVehicle->GetDesiredShotTarget())
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, OwningVehicle->GetDesiredShotTarget());
	
}
