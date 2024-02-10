// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"
#include "VehicleFactory.generated.h"

class UBoxComponent;
class UReceiveInteractionComponent;
class UInteractableComponent;
class ARCGameState;
class ARCGameMode;
class AControlPoint;

//	Vehicle buildings delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartVehcileBuilding_Signature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopVehicleBuilding_Signature);

//	Queue operations delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewQueueEntry_Singnature, int32, EntryIndex, FVehicleData, EntryData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQueueEntryRemoved_Signature, int32, EntryIndex, FVehicleData, EntryData);

USTRUCT(BlueprintType)
struct FVehicleCreationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Cost;
	
};

UCLASS(Blueprintable, BlueprintType)
class ROBOCOMBAT_API AVehicleFactory : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

	AVehicleFactory();

	virtual void BeginPlay() override;

#pragma region BlockingVolume

private:

	UPROPERTY()
	TArray<AActor*> BlockingActors;

public:

	UFUNCTION()
	void OnBlockingVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnBlockingVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
#pragma endregion 

#pragma region Interaction

public:

	UFUNCTION()
	void OnInteractionStart(UReceiveInteractionComponent* ByComponent);

	UFUNCTION()
	void OnInteractionEnd(UReceiveInteractionComponent* ByComponent);

	UFUNCTION()
	virtual bool CanInteract_Implementation(UReceiveInteractionComponent* ByComponent) const override;

#pragma endregion 

#pragma region ControlPoint

private:
	
	UPROPERTY(EditInstanceOnly, DisplayName = ControlPoint)
	AControlPoint* ControlPointBuffer;

	UPROPERTY(BlueprintGetter = GetControlPoint)
	AControlPoint* ControlPoint;
	
public:
	
	UFUNCTION(BlueprintGetter)
	AControlPoint* GetControlPoint() const {return ControlPoint;}

	void PostEditControlPoint(AControlPoint* InNewPoint);

#if WITH_EDITOR
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif
	
#pragma endregion 

#pragma region Components

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* VehicleOutLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* VehicleDefaultTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BuildingBlockingVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInteractableComponent* InteractableComponent;

#pragma endregion 

#pragma region VehicleCreationData

private:

	UPROPERTY(BlueprintGetter = GetVehicleCreationData)
	FVehicleCreationData VehicleCreationData;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AVehicleActor> VehicleSpawnClass = AVehicleActor::StaticClass();
	
public:

	UFUNCTION(BlueprintPure)
	FVehicleCreationData ConstructVehicleCreationData(const FVehicleData InVehicleData);

	UFUNCTION(BlueprintGetter)
	FVehicleCreationData GetVehicleCreationData() const {return VehicleCreationData;}

#pragma endregion

#pragma region Costs

private:

	UPROPERTY()
	ARCGameMode* GameMode;

	UPROPERTY()
	ARCGameState* GameState;

public:

	UFUNCTION(BlueprintPure)
	bool CanPlayerBuildVehicle(float Cost);

	void SpendBuildCost(float Cost);
	void RefundBuildCost(int32 QueueIndex);

#pragma endregion 

#pragma region BuildingTimer_And_Vehicle_Construction

public:

	UPROPERTY(BlueprintAssignable)
	FStartVehcileBuilding_Signature OnVehicleBuildingStart;

	UPROPERTY(BlueprintAssignable)
	FStopVehicleBuilding_Signature OnVehicleBuildingStop;
	
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle BuildingTimer;

	/*
	 *	Starts BuildingTimer with data from index 0
	 *	of building queue array.
	 */
	UFUNCTION()
	void StartBuildingVehicle();

	/*
	 *	Creates vehicle actor with data from index 0
	 *	of building queue array.
	 */
	UFUNCTION()
	void EndBuildingVehicle();

	/*
	 *	Invalidate BuildingTimer - do not remove
	 *	element from array. Use after remove from queue
	 */
	UFUNCTION()
	void AbortBuildingVehicle();

#pragma endregion 

#pragma region Queue

private:

	UPROPERTY(BlueprintGetter = GetBuildingQueue, VisibleInstanceOnly, Category = Debug)
	TArray<FVehicleData> BuildingQueue;

public:

	UPROPERTY(BlueprintAssignable)
	FOnNewQueueEntry_Singnature OnNewQueueEntry;

	/*
	 * Called after remove entry, so we
	 * can't just access it, from queue
	 */
	UPROPERTY(BlueprintAssignable)
	FOnQueueEntryRemoved_Signature OnQueueEntryRemoved;

	UFUNCTION(BlueprintCallable)
	void AddToQueue(const FVehicleData InNewData);

	UFUNCTION(BlueprintCallable)
	void RemoveFromQueue(const int32 InElementIndex);

	UFUNCTION(BlueprintPure)
	FVehicleData GetCurrentBuildingVehicleData();

	UFUNCTION(BlueprintGetter)
	TArray<FVehicleData> GetBuildingQueue() const {return BuildingQueue;}

	UFUNCTION(BlueprintCallable)
	void SwapQueueEntries(int32 FirstEntryIndex, int32 SecondEntryIndex);

#pragma endregion

#pragma region DEBUG

#if	WITH_EDITORONLY_DATA

	UPROPERTY(EditInstanceOnly, Category = Debug)
	FVehicleData AddToQueueData;

	UPROPERTY(EditInstanceOnly, Category = Debug)
	int32 RemoveFromQueueData;

#endif

#if	WITH_EDITOR

	UFUNCTION(CallInEditor, Category = Debug)
	void AddDataToQueue();

	UFUNCTION(CallInEditor, Category = Debug)
	void RemoveDataFromQueue();

#endif

#pragma endregion 
	
};
