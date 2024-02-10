// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboCombat/VehicleElements/Public/VehicleElement.h"
#include "RoboCombat/Database/Public/VehicleElementsDataBase.h"

void UVehicleElement::SetVehicleElementDataReference(const FDataTableRowHandle InDataReference)
{
	FVehicleElementData Data;
	if (!UVehicleElementsDataBase::ReadElementData(InDataReference, Data, this))
	{
		#if WITH_EDITOR
		if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red,
							TEXT("Try to set empty data into Vehicle element component"));
		#endif
		SetSkeletalMeshAsset(nullptr);
		VehicleElementDataReference = InDataReference;
		return;
	}

	VehicleElementDataReference = InDataReference;
	SetupElement(Data);
}

void UVehicleElement::SetupMesh(const FVehicleElementData InData)
{
	SetSkeletalMeshAsset(InData.SkeletalMeshAsset.LoadSynchronous());
	SetCollisionProfileName(FName(TEXT("Game_Vehicle")));
}

void UVehicleElement::SetupAnimation(const FVehicleElementData InData)
{
	SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SetAnimClass(InData.AnimationBlueprintClass);
}

void UVehicleElement::SetupElement(const FVehicleElementData InData)
{
	SetupMesh(InData);
	SetupAnimation(InData);
}
