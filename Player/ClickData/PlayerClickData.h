// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerClickData.generated.h"

/**
 *	Click info that is send to interacted object
 *	For example player send orders to vehicles this way
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOCOMBAT_API UPlayerClickData : public UObject
{
	GENERATED_BODY()

	FHitResult InHitResult;

public:

	void SetHitResult(FHitResult InNewHit) {InHitResult = InNewHit;}

	UFUNCTION(BlueprintPure)
	AActor* GetHitActor();

	UFUNCTION(BlueprintPure)
	FVector GetHitLocation() const {return InHitResult.Location;}
	
};