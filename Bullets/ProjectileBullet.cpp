
#include "ProjectileBullet.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RoboCombat/HealthAndDamage/Public/HealthComponent.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"

class UHealthComponent;

void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AProjectileBullet::OnBulletHit);
}

void AProjectileBullet::OnBulletHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == BulletOwner)
	return;
	
	UHealthComponent* HitHealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();
	if (HitHealthComponent)
	{
		HitHealthComponent->ApplyDamage(GetBulletData().Damage);
		Destroy();
	}
}

AProjectileBullet::AProjectileBullet()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	StaticMeshComponent->SetUseCCD(true);
	StaticMeshComponent->SetCollisionProfileName("OverlapAllDynamic");
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(false);
}

void AProjectileBullet::Shot()
{
	StaticMeshComponent->SetSimulatePhysics(true);
	FVector TossVelocity;
	UGameplayStatics::SuggestProjectileVelocity(this, TossVelocity, GetActorLocation(),
											(GetTargetActor()->GetActorLocation() + GetTargetActor()->GetVelocity()), GetBulletData().Speed, false, 15,
											GetWorld()->GetGravityZ(), ESuggestProjVelocityTraceOption::DoNotTrace);
	StaticMeshComponent->AddImpulse(TossVelocity, NAME_None, true);
}

void AProjectileBullet::SetBulletData(const FBulletsData InBulletData)
{
	Super::SetBulletData(InBulletData);
	StaticMeshComponent->SetStaticMesh(InBulletData.ProjectileMesh.LoadSynchronous());
}