// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "RoboCombat/HealthAndDamage/Public/HealthComponent.h"
#include "RoboCombat/VehicleElements/Public/WeaponElementComponent.h"
#include "VehicleActor.generated.h"

enum EVehicleStateSelector : UINT8;
class ARCGameState;
class USphereComponent;
class AVehicleAIController;
class UVehicleHealthComponent;
class UVehicleElement;
class UChassisElementComponent;

UCLASS()
class ROBOCOMBAT_API AVehicleActor : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	ARCGameState* GameState;

	UFUNCTION()
	void OnVehicleDied(UHealthComponent* InHealthComponent);

public:

	AVehicleActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVehicleHealthComponent* HealthComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int PlayerIndex;

#pragma region OrderVisuals

private:

	UPROPERTY(VisibleAnywhere, Category = OrderVisualisation)
	UStaticMeshComponent* OrderMeshBase;

	UPROPERTY(VisibleAnywhere, Category = OrderVisualisation)
	UStaticMeshComponent* OrderMeshMain;

	UPROPERTY(EditDefaultsOnly, Category = OrderVisualisation)
	float OrderVisualiserZOffset;

	UPROPERTY(EditDefaultsOnly, Category = OrderVisualisation)
	UCurveFloat* OrderVisualisationFloatingCurve;
	float OrderVisualisationFloatingTime;
	
	UPROPERTY(EditDefaultsOnly, Category = OrderVisualisation)
	UStaticMesh* MoveToOrderMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = OrderVisualisation)
	UStaticMesh* DefenseOrderMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = OrderVisualisation)
	UStaticMesh* AttackOrderMesh;
	
public:

	UFUNCTION(BlueprintCallable)
	void UpdateOrderVisualisationMesh(EVehicleStateSelector InOrderType);

	UFUNCTION(BlueprintCallable)
	void UpdateOrderVisualisationLocation(FVector InNewLocation);

	UFUNCTION(BlueprintCallable)
	void AddOrderVisualisationFloatingTime(float InDeltaTime);
	
#pragma endregion 

#pragma region ElementsComponentsCreation

private:
	
	UPROPERTY(BlueprintGetter = GetElementsData, BlueprintSetter = SetElementsData, EditAnywhere)
	FVehicleData ElementsData;

protected:

	UFUNCTION(BlueprintPure)
	TArray<UVehicleElement*> GetAllElements();

public:

	UFUNCTION(BlueprintGetter)
	FVehicleData GetElementsData() const {return  ElementsData;}

	UFUNCTION(BlueprintSetter)
	void SetElementsData(const FVehicleData InElementsData);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UChassisElementComponent* ChassisElementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UWeaponElementComponent* BodyWeaponComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UVehicleElement* BodyElementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UWeaponElementComponent* RightExtendWeapon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UWeaponElementComponent* LeftExtendWeapon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USphereComponent* WeaponRangeCollision;

	bool SetupElementChecked(UVehicleElement* InElement, const FDataTableRowHandle InElementData);
	void SetupElements();
	

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif
	
#pragma endregion 

#pragma region FireControl

private:

	UPROPERTY(EditInstanceOnly, BlueprintGetter = GetDesiredShotTarget, BlueprintSetter = SetDesiredShotTarget)
	AActor* DesiredShotTarget;

	UPROPERTY(EditAnywhere)
	float FireRotationThreshold = 0.25f;

	UFUNCTION()
	void SetupWeaponRange();

	bool bTargetValid;
	
public:

	UFUNCTION(BlueprintGetter)
	AActor* GetDesiredShotTarget() const {return DesiredShotTarget;}

	UFUNCTION(BlueprintSetter)
	void SetDesiredShotTarget(AActor* InActor);

	UFUNCTION(BlueprintPure)
	float GetWeaponMinRange() const;

	UFUNCTION(BlueprintPure)
	float GetWeaponMaxRange() const;

	void SetTargetValid(bool InValue);
	
	/*
	 * Return true if weapon is ready to shoot
	 */
	bool RotateWeapons(float DeltaTime);

	void TryActivateWeapons(bool InNewActive);
	
	FRotator GetDesiredWeaponFacingRotation() const;

	UVehicleElement* GetWeaponRotationComponent() const {return !ElementsData.BodyDataReference.IsNull() ? BodyElementComponent : BodyWeaponComponent;}

#pragma endregion

#pragma region AI

private:

	UPROPERTY(BlueprintSetter = SetVehicleAI)
	AVehicleAIController* VehicleAI;

	UPROPERTY(BlueprintGetter = GetSeenEnemies)
	TArray<AVehicleActor*> SeenEnemies;

	UPROPERTY(BlueprintGetter = GetSeenAllies)
	TArray<AVehicleActor*> SeenAllies;

	void OnStartRegisterOverlaps();

public:

	UFUNCTION(BlueprintSetter)
	void SetVehicleAI(AVehicleAIController* InController) {VehicleAI = InController;}

	UFUNCTION(BlueprintGetter)
	TArray<AVehicleActor*> GetSeenEnemies() const {return SeenEnemies;}

	UFUNCTION(BlueprintGetter)
	TArray<AVehicleActor*> GetSeenAllies() const {return SeenAllies;}

	void RegisterEnemy(AVehicleActor* InEnemy);
	void RegisterAlly(AVehicleActor* InAlly);

	UFUNCTION()
	void OnAllyDied(UHealthComponent* InHealthComponent);

	UFUNCTION()
	void OnEnemyDied(UHealthComponent* InHealthComponent);

	UFUNCTION()
	void OnWeaponRangeCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void OnDetectedVehicle(AVehicleActor* OverlappedVehicle);

	UFUNCTION()
	void OnWeaponRangeCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent)
	void OnVehicleMissed(AVehicleActor* OverlappedVehicle);

	UFUNCTION(BlueprintPure)
	bool SeeEnemy(AVehicleActor* InEnemy) const {return SeenEnemies.Contains(InEnemy);}

#pragma endregion

#pragma region Interaction

private:

	bool bIsSelected;
	bool bIsHovered;
	
	UPROPERTY(VisibleAnywhere)
	UInteractableComponent* InteractableComponent;

	UPROPERTY(VisibleAnywhere)
	UDecalComponent* InteractionDecal;

	UPROPERTY()
	UMaterialInstanceDynamic* InteractionDecalMaterial;

	void UpdateInteractionDecal();

	UFUNCTION()
	void OnInteractionStart(UReceiveInteractionComponent* ByComponent);

	UFUNCTION()
	void OnInteractionEnd(UReceiveInteractionComponent* ByComponent);

	UFUNCTION()
	void OnInteractionDataReceived(UReceiveInteractionComponent* Sender, UObject* InDataObject);

	UFUNCTION()
	void OnHovered(UReceiveInteractionComponent* ByComponent);

	UFUNCTION()
	void OnUnhovered(UReceiveInteractionComponent* ByComponent);

protected:
	
	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName = OnInteractionStart))
	void OnInteractionStart_Blueprint(UReceiveInteractionComponent* ByComponent);

	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName = OnInteractionEnd))
	void OnInteractionEnd_Blueprint(UReceiveInteractionComponent* ByComponent);

	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName = OnInteractionDataReceived))
	void OnInteractionDataReceived_Blueprint(UReceiveInteractionComponent* Sender, UObject* InDataObject);
	
public:

	virtual bool CanInteract_Implementation(UReceiveInteractionComponent* ByComponent) const override;
	
#pragma endregion 

};
