
#include "HitScanBullet.h"

#include "RoboCombat/HealthAndDamage/Public/HealthComponent.h"

AHitScanBullet::AHitScanBullet()
{
	HitScanTraceParams = FCollisionQueryParams(FName(TEXT("HitScanParams")), true, GetOwner());
	HitScanTraceParams.bTraceComplex = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
}

void AHitScanBullet::Shot()
{
	FHitResult HitScanResult;
	HitScanTraceParams.AddIgnoredActor(BulletOwner);
	GetWorld()->LineTraceSingleByChannel(HitScanResult, GetActorLocation(), GetTargetActor()->GetActorLocation(), ECC_Vehicle, HitScanTraceParams);

	if (HitScanResult.GetActor())
	{
		UHealthComponent* HitHealthComponent = HitScanResult.GetActor()->FindComponentByClass<UHealthComponent>();
		if (HitHealthComponent)
		HitHealthComponent->ApplyDamage(GetBulletData().Damage);
	}
}
