// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"
#include "ControlPoint.generated.h"


class APlayerPawn;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwningPlayerChanged_Signature, int32, NewPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConquerPointsChanged_Signature, float, CurrentPoints);

class ARCGameMode;
class AVehicleFactory;
class USphereComponent;

USTRUCT(BlueprintType)
struct FControlPointConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PointsIncome = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConquerByVehicle = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FactorySpeedMultiplier = 1;
	
};

UCLASS(BlueprintType, Blueprintable)
class ROBOCOMBAT_API AControlPoint : public AActor
{
	GENERATED_BODY()

public:

	AControlPoint();
	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;

#pragma region Factories

private:

	UPROPERTY(VisibleInstanceOnly)
	TArray<AVehicleFactory*> Factories;

public:
	
	void AddFactory(AVehicleFactory* InNewFactory);
	void RemoveFactory(AVehicleFactory* InRemovedFactory);

#pragma endregion 

#pragma region BaseConfig_&_Components

private:
	
	UPROPERTY(EditAnywhere, BlueprintGetter = GetPointConfig)
	FControlPointConfig PointConfig;

public:

	UFUNCTION(BlueprintGetter)
	FControlPointConfig GetPointConfig() const {return PointConfig;}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PoleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* FlagMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* ConquerCollision;

#pragma endregion

#pragma region Overlaps

protected:

	UPROPERTY(BlueprintReadOnly)
	TArray<APlayerPawn*> OverlappingPlayers;
	
public:
	
	UFUNCTION()
	void OnConquerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerPawnEnter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerPawnLeave();
	
	UFUNCTION()
	void OnConquerCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion 

#pragma region ConquerLogic

protected:

	void SetConquerPoints_Internal(const float InPoints);

private:

	UPROPERTY(EditInstanceOnly, BlueprintGetter = GetOwningPlayerIndex, BlueprintSetter = SetOwningPlayerIndex)
	int32 OwningPlayerIndex = -1;

	UPROPERTY(BlueprintSetter = SetConqueringPlayerIndex)
	int32 ConqueringPlayerIndex = -1;

	UPROPERTY(EditInstanceOnly, BlueprintGetter = GetConquerPoints, BlueprintSetter = SetConquerPoints)
	float ConquerPoints = 1500;

	UPROPERTY(VisibleInstanceOnly)
	TMap<int32, int> PlayerConquerMultipliers;

	void IncrementPlayerConquerMultiplier(int32 InPlayerIndex);
	void DecrementPlayerConquerMultiplier(int32 InPlayerIndex);

	UPROPERTY()
	TArray<AVehicleActor*> Vehicles;
	
	bool BindVehicle(AVehicleActor* InVehicle);
	bool UnBindVehicle(AVehicleActor* InVehicle);

	UFUNCTION()
	void OnVehicleDied(UHealthComponent* InHealthComponent);

public:

	UPROPERTY(BlueprintAssignable)
	FOnConquerPointsChanged_Signature OnConquerPointsChanged;

	UPROPERTY(BlueprintAssignable)
	FOnOwningPlayerChanged_Signature OnOwningPlayerChanged;

	UFUNCTION(BlueprintSetter)
	void SetOwningPlayerIndex(const int32 InNewIndex);

	UFUNCTION(BlueprintGetter)
	int GetOwningPlayerIndex() const {return OwningPlayerIndex;}

	UFUNCTION(BlueprintSetter)
	void SetConquerPoints(const float InPoints);

	UFUNCTION(BlueprintGetter)
	float GetConquerPoints() const {return ConquerPoints;}

	UFUNCTION(BlueprintCallable)
	void IncreaseConquerPoints(const float InPointsDelta)
	{
		SetConquerPoints(UKismetMathLibrary::FClamp((GetConquerPoints() + InPointsDelta), -100, 100));
	}

	UFUNCTION(BlueprintCallable)
	void AddVehicle(AVehicleActor* InVehicle);

	UFUNCTION(BlueprintCallable)
	void RemoveVehicle(AVehicleActor* InRemovedVehicle);

	UFUNCTION(BlueprintSetter)
	void SetConqueringPlayerIndex(const int32 InNewPlayerIndex);

#pragma endregion

#pragma region ConquerVisuals

private:

	float FlagHeightMin;
	float FlagHeightMax;

	UPROPERTY()
	UMaterialInstanceDynamic* FlagMaterial;
	
	UFUNCTION(BlueprintCallable)
	void SetFlagColorByPlayerIndex(const int32 InPlayerIndex);

	UFUNCTION(BlueprintCallable)
	void SetFlagHeight(const float InHeightAlpha);

#pragma endregion

#pragma region scoring

private:

	UPROPERTY()
	ARCGameMode* GameMode;
	
public:

	UFUNCTION()
	void Score(float DeltaTime);

#pragma endregion 
	
};