// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBullet.h"
#include "MissileBullet.generated.h"

UCLASS()
class ROBOCOMBAT_API AMissileBullet : public AProjectileBullet
{
	GENERATED_BODY()

	virtual void Tick(float DeltaSeconds) override;

	void ChaseTarget(float DeltaTime);
	void MoveMissile(const FVector InNewLocation);

	virtual void Shot() override;
	virtual void SetBulletData(const FBulletsData InBulletData) override;
	
	bool bIsChasing;
	void StartChasing();

	FTimerHandle ActivationTimer;
	
};
