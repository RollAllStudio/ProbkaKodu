// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQuerryContext_SeenAllies.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"

void UEnvQuerryContext_SeenAllies::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                  FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AVehicleActor* OwningVehicle = Cast<AVehicleActor>(QueryInstance.Owner);

	TArray<AActor*> OutContextArray;
	
	if (OwningVehicle)
		for(int i = 0; i < OwningVehicle->GetSeenAllies().Num(); i++)
			OutContextArray.AddUnique(OwningVehicle->GetSeenAllies()[i]);
		
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, OutContextArray);	
	
}
