
#include "MissileBullet.h"

#include "Curves/CurveVector.h"
#include "Kismet/KismetMathLibrary.h"

void AMissileBullet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsChasing)
		ChaseTarget(DeltaSeconds);
}

void AMissileBullet::ChaseTarget(float DeltaTime)
{
	const FVector TargetLocation = GetTargetActor()->GetActorLocation();
	const FVector ChaseDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetLocation);
	const FVector NewLocation = GetActorLocation() + (ChaseDirection * (DeltaTime * GetBulletData().Speed));

	MoveMissile(NewLocation);
}

void AMissileBullet::MoveMissile(const FVector InNewLocation)
{
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InNewLocation));
	SetActorLocation(InNewLocation);
}

void AMissileBullet::Shot()
{
	Super::Shot();
	GetWorldTimerManager().SetTimer(ActivationTimer, this, &AMissileBullet::StartChasing, GetBulletData().ActivationTime);
}

void AMissileBullet::SetBulletData(const FBulletsData InBulletData)
{
	Super::SetBulletData(InBulletData);
}

void AMissileBullet::StartChasing()
{
	if(GetTargetActor())
	{
		StaticMeshComponent->SetSimulatePhysics(false);
		PrimaryActorTick.SetTickFunctionEnable(true);
		bIsChasing = true;
		return;
	}

	Destroy();
}