// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboCombat/VehicleElements/Public/WeaponElementComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "RoboCombat/Bullets/Bullet.h"
#include "RoboCombat/Database/Public/VehicleElementsDataBase.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"

void UWeaponElementComponent::SetupElement(const FVehicleElementData InData)
{
	Super::SetupElement(InData);

	BulletsData = InData.BulletsData;
	FireMontage = BulletsData.FireMontage.LoadSynchronous();
}

void UWeaponElementComponent::SetupAnimation(FVehicleElementData InData)
{
	SetAnimationMode(EAnimationMode::AnimationSingleNode);
}

void UWeaponElementComponent::StartFire(AActor* DesiredTarget)
{
	TargetActor = DesiredTarget;
	if (bIsFiring != true)
	{
		if (IsTargetInRange())
		{
			bIsFiring = true;
			PlayAnimation(FireMontage, true);
			SetPlayRate(BulletsData.AnimPlayRate);
		}
	}
}

void UWeaponElementComponent::StopFire()
{
	if (bIsFiring != false)
	{
		bIsFiring = false;
		Stop();
	}
}

bool UWeaponElementComponent::IsTargetInRange()
{
	if (TargetActor)
	{
		return UKismetMathLibrary::Vector_Distance(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation()) <= BulletsData.Range;
	}
	return false;
}

void UWeaponElementComponent::SpawnBullets()
{
	for (int i = 0; i < BulletsData.SpawnSockets.Num(); i++)
	{
		FName SocketName = BulletsData.SpawnSockets[i];
		if (DoesSocketExist(SocketName))
			SpawnBulletAtSocket(SocketName);
	}
}

void UWeaponElementComponent::SpawnBulletAtSocket(FName InSocketName)
{
	const FTransform SpawnTransform = GetSocketTransform(InSocketName, RTS_World);
	ABullet* Bullet = GetWorld()->SpawnActorDeferred<ABullet>(BulletsData.BulletClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Bullet->SetBulletData(BulletsData);
	Bullet->SetTargetActor(TargetActor);
	Bullet->BulletOwner = GetOwner();
	Bullet->FinishSpawning(SpawnTransform);
	Bullet->Shot();
}
