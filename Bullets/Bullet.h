// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoboCombat/Database/Public/VehicleElementsDataBase.h"
#include "Bullet.generated.h"

class AVehicleActor;
UCLASS(Abstract)
class ROBOCOMBAT_API ABullet : public AActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual void Shot();
	
	UFUNCTION(BlueprintSetter)
	virtual void SetBulletData(const FBulletsData InBulletData) {BulletData = InBulletData;}

	UFUNCTION(BlueprintGetter)
	FBulletsData GetBulletData() const {return BulletData;}
	
	UFUNCTION(BlueprintSetter)
	virtual void SetTargetActor(AActor* InTargetActor) {TargetActor = InTargetActor;}

	UFUNCTION(BlueprintGetter)
	AActor* GetTargetActor() const {return TargetActor;}

	UPROPERTY()
	AActor* BulletOwner;
	
private:

	UPROPERTY(BlueprintSetter = SetBulletData, BlueprintGetter = GetBulletData, meta=(ExposeOnSpawn))
	FBulletsData BulletData;

	UPROPERTY(BlueprintSetter = SetTargetActor, BlueprintGetter = GetTargetActor, meta=(ExposeOnSpawn))
	AActor* TargetActor;
	
};
