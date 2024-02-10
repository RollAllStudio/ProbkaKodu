#include "RoboCombat/HealthAndDamage/Public/VehicleHealthComponent.h"

#include "RoboCombat/VehicleActor/VehicleActor.h"

void UVehicleHealthComponent::BeginPlay()
{
	float MaxHealthIterator = 0;

	AVehicleActor* OwnerVehicle = Cast<AVehicleActor>(GetOwner());
	if (OwnerVehicle)
	{
		TArray<UActorComponent*> OwnerComponents = OwnerVehicle->GetComponents().Array();
		for	(int i = 0; i < OwnerComponents.Num(); i++)
		{
			UVehicleElement* CurrentElement = Cast<UVehicleElement>(OwnerComponents[i]);
			if (CurrentElement)
			{
				FVehicleElementData CurrentElementData;
				FDataTableRowHandle CurrentDataHandle = CurrentElement->GetVehicleElementDataReference();
				if (UVehicleElementsDataBase::ReadElementData(CurrentDataHandle, CurrentElementData, this))
					MaxHealthIterator += CurrentElementData.HealthPoints;
			}
		}
	}
	SetMaxHealthPoints(MaxHealthIterator);
	Super::BeginPlay();
}
