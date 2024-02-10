// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "HitScanBullet.generated.h"

UCLASS()
class ROBOCOMBAT_API AHitScanBullet : public ABullet
{
	GENERATED_BODY()

	AHitScanBullet();
	FCollisionQueryParams HitScanTraceParams;
	virtual void Shot() override;

#if WITH_EDITORONLY_DATA

public:
	
	UPROPERTY(EditAnywhere)
	bool TraceDebug = true;

#endif
	
};
