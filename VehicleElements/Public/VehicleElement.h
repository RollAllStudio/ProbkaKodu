// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VehicleElement.generated.h"

struct FVehicleElementData;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Placeable)
class ROBOCOMBAT_API UVehicleElement : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintGetter)
	FDataTableRowHandle GetVehicleElementDataReference() const {return VehicleElementDataReference;}

	UFUNCTION(BlueprintSetter)
	void SetVehicleElementDataReference(const FDataTableRowHandle InDataReference);
	void SetupMesh(FVehicleElementData InData);
	virtual void SetupAnimation(FVehicleElementData InData);

	virtual void SetupElement(const FVehicleElementData InData);
	
private:

	UPROPERTY(EditAnywhere, BlueprintGetter = GetVehicleElementDataReference, BlueprintSetter = SetVehicleElementDataReference, Category = Config, meta=(ExposeOnSpawn))
	FDataTableRowHandle VehicleElementDataReference;
	
};
