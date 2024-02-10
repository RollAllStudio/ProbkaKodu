// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleElement.h"
#include "ChassisElementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API UChassisElementComponent : public UVehicleElement
{
	GENERATED_BODY()
	virtual void SetupElement(const FVehicleElementData InData) override;
	
};