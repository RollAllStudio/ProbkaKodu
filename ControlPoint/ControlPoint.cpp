// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlPoint.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RoboCombat/Core/GameConstants/GameConstantsStatics.h"
#include "RoboCombat/Core/Public/RCGameMode.h"
#include "RoboCombat/HealthAndDamage/Public/VehicleHealthComponent.h"
#include "RoboCombat/Player/PlayerPawn.h"

AControlPoint::AControlPoint()
{

	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
	
	PoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pole"));
	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flag"));
	ConquerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ConquerCollision"));

	SetRootComponent(PoleMesh);
	FlagMesh->SetupAttachment(PoleMesh);
	ConquerCollision->SetupAttachment(PoleMesh);

	PoleMesh->SetCollisionProfileName(TEXT("ControllPoint"));
	PoleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ConquerCollision->SetCollisionProfileName(TEXT("ConquerCollision"));
	ConquerCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ConquerCollision->SetSphereRadius(250);
}

void AControlPoint::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (ConqueringPlayerIndex != -1)
	IncreaseConquerPoints(PointConfig.ConquerByVehicle * *PlayerConquerMultipliers.Find(ConqueringPlayerIndex) * (ConqueringPlayerIndex == OwningPlayerIndex ? 1 : -1));

	if (OwningPlayerIndex != -1 && GameMode)
	Score(DeltaSeconds);
	
}

void AControlPoint::BeginPlay()
{

	Super::BeginPlay();
	
	ConquerCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &AControlPoint::OnConquerCollisionBeginOverlap);
	ConquerCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &AControlPoint::OnConquerCollisionEndOverlap);

	FlagHeightMin = PoleMesh->GetSocketLocation(TEXT("FlagStart")).Z;
	FlagHeightMax = PoleMesh->GetSocketLocation(TEXT("FlagEnd")).Z;
	SetFlagHeight(1);

	FlagMaterial = FlagMesh->CreateDynamicMaterialInstance(0, FlagMesh->GetMaterial(0));
	SetFlagColorByPlayerIndex(OwningPlayerIndex);
	FlagMesh->SetMaterial(0, FlagMaterial);

	GameMode = Cast<ARCGameMode>(UGameplayStatics::GetGameMode(this));
	
}

void AControlPoint::AddFactory(AVehicleFactory* InNewFactory)
{
	Factories.AddUnique(InNewFactory);
}

void AControlPoint::RemoveFactory(AVehicleFactory* InRemovedFactory)
{
	Factories.Remove(InRemovedFactory);
}


void AControlPoint::OnConquerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVehicleActor* OverlappedVehicle = Cast<AVehicleActor>(OtherActor);
	if (OverlappedVehicle)
	{
		AddVehicle(OverlappedVehicle);
		return;
	}

	APlayerPawn* OverlappedPlayer = Cast<APlayerPawn>(OtherActor);
	if (OverlappedPlayer)
	{
		OverlappingPlayers.AddUnique(OverlappedPlayer);
		OnPlayerPawnEnter();
	}
}

void AControlPoint::OnConquerCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVehicleActor* LocalVehicle = Cast<AVehicleActor>(OtherActor);
	if (LocalVehicle)
	{
		RemoveVehicle(LocalVehicle);
		return;
	}

	APlayerPawn* OverlappedPlayer = Cast<APlayerPawn>(OtherActor);
	if (OverlappedPlayer)
	{
		OverlappingPlayers.Remove(OverlappedPlayer);
		OnPlayerPawnLeave();
	}
	
}

void AControlPoint::IncrementPlayerConquerMultiplier(int32 InPlayerIndex)
{
	
	if (PlayerConquerMultipliers.Contains(InPlayerIndex))
	{
		int NewValue = *PlayerConquerMultipliers.Find(InPlayerIndex) + 1;
		PlayerConquerMultipliers.Add(InPlayerIndex, NewValue);
		return;
	}
	PlayerConquerMultipliers.Add(InPlayerIndex, 1);

	SetConqueringPlayerIndex(PlayerConquerMultipliers.Num() == 1 ? InPlayerIndex : -1);
}

void AControlPoint::DecrementPlayerConquerMultiplier(int32 InPlayerIndex)
{
	if (PlayerConquerMultipliers.Contains(InPlayerIndex))
	{
		int NewValue = *PlayerConquerMultipliers.Find(InPlayerIndex) - 1;
		if (NewValue == 0)
		{
			PlayerConquerMultipliers.Remove(InPlayerIndex);
			if (PlayerConquerMultipliers.Num() == 1)
			{
				TArray<int32> LocalArray;
				PlayerConquerMultipliers.GetKeys(LocalArray);
				SetConqueringPlayerIndex(LocalArray[0]);
				return;
			}
			SetConqueringPlayerIndex(-1);
			return;
		}
		PlayerConquerMultipliers.Add(InPlayerIndex, NewValue);
	}
}

bool AControlPoint::BindVehicle(AVehicleActor* InVehicle)
{
	if (Vehicles.Contains(InVehicle))
	return false;

	Vehicles.Add(InVehicle);
	InVehicle->HealthComponent->OnHealthDepleted.AddUniqueDynamic(this, &AControlPoint::OnVehicleDied);
	return true;
}

void AControlPoint::OnVehicleDied(UHealthComponent* InHealthComponent)
{
	AVehicleActor* LocalVehicle = Cast<AVehicleActor>(InHealthComponent->GetOwner());
	if (LocalVehicle)
	RemoveVehicle(LocalVehicle);
}

bool AControlPoint::UnBindVehicle(AVehicleActor* InVehicle)
{
	if (!Vehicles.Contains(InVehicle))
	return false;

	Vehicles.Remove(InVehicle);
	InVehicle->HealthComponent->OnHealthDepleted.RemoveDynamic(this, &AControlPoint::OnVehicleDied);
	return true;
}

void AControlPoint::SetOwningPlayerIndex(const int32 InNewIndex)
{
	if (OwningPlayerIndex != InNewIndex)
	{
		OwningPlayerIndex = InNewIndex;
		SetFlagColorByPlayerIndex(OwningPlayerIndex);
		OnOwningPlayerChanged.Broadcast(OwningPlayerIndex);
	}
}

void AControlPoint::SetConquerPoints(const float InPoints)
{
	if (InPoints != ConquerPoints)
	{
		SetFlagHeight(InPoints/100);

		if (InPoints <= 0)
		{
			SetConquerPoints_Internal(InPoints);
			SetOwningPlayerIndex(ConqueringPlayerIndex);
			return;
		}

		SetConquerPoints_Internal(InPoints);
	}
}

void AControlPoint::SetConquerPoints_Internal(const float InPoints)
{
	ConquerPoints = InPoints;
	OnConquerPointsChanged.Broadcast(ConquerPoints);
}

void AControlPoint::AddVehicle(AVehicleActor* InVehicle)
{
	if (BindVehicle(InVehicle))
		IncrementPlayerConquerMultiplier(InVehicle->PlayerIndex);
		
}

void AControlPoint::RemoveVehicle(AVehicleActor* InRemovedVehicle)
{
	if (UnBindVehicle(InRemovedVehicle))
	DecrementPlayerConquerMultiplier(InRemovedVehicle->PlayerIndex);
}

void AControlPoint::SetConqueringPlayerIndex(const int32 InNewPlayerIndex)
{
	if (InNewPlayerIndex != ConqueringPlayerIndex)
	{
		ConqueringPlayerIndex = InNewPlayerIndex;
	}
}

void AControlPoint::SetFlagColorByPlayerIndex(const int32 InPlayerIndex)
{
	FlagMaterial->SetVectorParameterValue(TEXT("TintColor"), UGameConstantsStatics::GetDefaultPlayerData(InPlayerIndex, this).Color);
}

void AControlPoint::SetFlagHeight(const float InHeightAlpha)
{
	FVector CurrentFlagLocation = FlagMesh->GetComponentLocation();
	CurrentFlagLocation.Z = UKismetMathLibrary::Lerp(FlagHeightMin, FlagHeightMax, InHeightAlpha);
	FlagMesh->SetWorldLocation(CurrentFlagLocation);
}

void AControlPoint::Score(float DeltaTime)
{	
	GameMode->ScorePlayer((PointConfig.PointsIncome) * DeltaTime, OwningPlayerIndex);
}
