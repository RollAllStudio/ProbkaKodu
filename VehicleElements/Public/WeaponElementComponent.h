// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleElement.h"
#include "RoboCombat/Database/Public/VehicleElementsDataBase.h"
#include "WeaponElementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API UWeaponElementComponent : public UVehicleElement
{
	GENERATED_BODY()
	virtual void SetupElement(const FVehicleElementData InData) override;

public:

	FBulletsData BulletsData;

	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void StartFire(AActor* DesiredTarget);

	UPROPERTY()
	UAnimMontage* FireMontage;

	virtual void SetupAnimation(FVehicleElementData InData) override;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void StopFire();

	bool IsTargetInRange();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SpawnBullets();
	void SpawnBulletAtSocket(FName InSocketName);

	UPROPERTY(BlueprintReadOnly)
	bool bIsFiring = false;
	
};