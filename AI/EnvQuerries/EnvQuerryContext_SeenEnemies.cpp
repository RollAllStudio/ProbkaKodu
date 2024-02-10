// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQuerryContext_SeenEnemies.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"


void UEnvQuerryContext_SeenEnemies::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                   FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AVehicleActor* OwningVehicle = Cast<AVehicleActor>(QueryInstance.Owner);

	TArray<AActor*> OutContextArray;
	
	if (OwningVehicle)
		for(int i = 0; i < OwningVehicle->GetSeenEnemies().Num(); i++)
		{
			if (OwningVehicle->GetSeenEnemies()[i] != OwningVehicle->GetDesiredShotTarget())
				OutContextArray.AddUnique(OwningVehicle->GetSeenEnemies()[i]);
		}
		
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, OutContextArray);	
}
