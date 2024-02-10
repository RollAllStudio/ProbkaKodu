// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RCPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API ARCPlayerState : public APlayerState
{
	GENERATED_BODY()

#pragma region Points

private:
	
	float PointsAbsolute;

	UPROPERTY(BlueprintGetter = GetPoints)
	int DisplayPoints;

	UPROPERTY(BlueprintGetter = GetBuildingPoints)
	int BuildingPoints;

public:

	UFUNCTION(BlueprintCallable)
	void AddPoints(const float InPoints);
	void SetDisplayPoints(const int InNewValue);

	UFUNCTION(BlueprintGetter)
	int GetPoints() const {return DisplayPoints;}

	UFUNCTION(BlueprintGetter)
	int GetBuildingPoints() const {return BuildingPoints;}

	UFUNCTION(BlueprintCallable)
	void SpendPlayerPoints(const int InPoints) {BuildingPoints -= InPoints;}

	UFUNCTION(BlueprintCallable)
	void RefundPlayerPoints(const int InPoints) {BuildingPoints += InPoints;}

	UFUNCTION(BlueprintCallable)
	void AddBuildingPoints(const int InPoints) {BuildingPoints += InPoints;}

#pragma endregion 
	
};
