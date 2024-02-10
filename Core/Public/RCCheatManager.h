// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "RCCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API URCCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	
	UFUNCTION(Exec, Category = "Cheats")
	void AddAbsolutePoints(int32 InPlayerIndex, float PointsToAdd);

	UFUNCTION(Exec, Category = "Cheats")
	void AddBuildingPoints(int32 InPlayerIndex, float PointsToAdd);
	
};
