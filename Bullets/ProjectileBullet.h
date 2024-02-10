// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "ProjectileBullet.generated.h"

UCLASS()
class ROBOCOMBAT_API AProjectileBullet : public ABullet
{
	
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
public:

	UFUNCTION()
	void OnBulletHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	AProjectileBullet();

	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* StaticMeshComponent;
	
	virtual void Shot() override;
	virtual void SetBulletData(const FBulletsData InBulletData) override;

};
