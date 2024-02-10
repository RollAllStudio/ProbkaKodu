// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReceiveInteractionComponent.h"
#include "Components/BoxComponent.h"
#include "MultipleSelectionComponent.generated.h"

class UPlayerClickData;
class UInteractableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableHovered_Signature, UInteractableComponent*, HoveredInteractable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableUnHovered_Signature, UInteractableComponent*, UnHoveredInteractable);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class ROBOCOMBAT_API UMultipleSelectionComponent : public UBoxComponent
{
	GENERATED_BODY()

	UPROPERTY()
	UReceiveInteractionComponent* OwnerInteractionComponent;

public:

	UMultipleSelectionComponent();
	
protected:

	virtual void BeginPlay() override;

#pragma region Decal

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Decal")
	UMaterialInterface* DecalMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicDecalMaterial;

	UPROPERTY()
	UDecalComponent* DecalComponent;

public:

	UFUNCTION(BlueprintCallable)
	void SetupDecal(UDecalComponent* InDecal);
	
#pragma endregion 

#pragma region CollisionDraw
	
private:
	
	UPROPERTY(BlueprintSetter = SetStartPoint, BlueprintGetter = GetStartPoint)
	FVector StartPoint;
	
	UPROPERTY(BlueprintSetter = SetEndPoint, BlueprintGetter = GetEndPoint)
	FVector EndPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float MinDrawThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float CollisionHeight;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FVector RemovedCollisionLocation;

	UPROPERTY(BlueprintGetter = IsCollisionOnScene)
	bool bIsCollisionOnScene;

	UFUNCTION()
	void SetLocationAndSize(FVector InLocation, FVector InSize);

public:

	UFUNCTION(BlueprintSetter)
	void SetStartPoint(const FVector InStartPoint);

	UFUNCTION(BlueprintGetter)
	FVector GetStartPoint() const {return StartPoint;}

	UFUNCTION(BlueprintSetter)
	void SetEndPoint(const FVector InEndPoint);

	UFUNCTION(BlueprintGetter)
	FVector GetEndPoint() const {return EndPoint;}

	UFUNCTION()
	void DrawCollision();

	UFUNCTION()
	void AddCollisionOnScene();

	UFUNCTION()
	void RemoveCollisionFromScene();

	UFUNCTION(BlueprintGetter)
	bool IsCollisionOnScene() const {return bIsCollisionOnScene;}

#pragma endregion

#pragma region Overlaps

private:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion 

#pragma region SelectionContext&Input

private:

	UPROPERTY(BlueprintGetter = GetHoveredInteractables)
	TArray<UInteractableComponent*> HoveredInteractables;

	UPROPERTY(BlueprintGetter = GetSelectedInteractables)
	TArray<UInteractableComponent*> SelectedInteractables;

	UFUNCTION()
	void AddHoveredInteractable(UInteractableComponent* InInteractable);

	UFUNCTION()
	void RemoveHoveredInteractable(UInteractableComponent* InInteractable);

public:

	UPROPERTY(BlueprintAssignable)
	FOnInteractableHovered_Signature OnInteractableHovered;
	
	UPROPERTY(BlueprintAssignable)
	FOnInteractableUnHovered_Signature OnInteractableUnHovered;

	UFUNCTION(BlueprintCallable)
	void ClearSelection();
	
	UFUNCTION(BlueprintCallable)
	void ClearHoveredInteractables(bool bSelect);

	UFUNCTION(BlueprintGetter)
	TArray<UInteractableComponent*> GetHoveredInteractables() const {return HoveredInteractables;}
	
	UFUNCTION(BlueprintPure)
	bool IsAnythingHovered() const {return SelectedInteractables.Num() > 0;}

	UFUNCTION(BlueprintGetter)
	TArray<UInteractableComponent*> GetSelectedInteractables() const {return SelectedInteractables;}

	UFUNCTION(BlueprintPure)
	bool IsAnythingSelected() const {return SelectedInteractables.Num() > 0;}

	UFUNCTION(BlueprintCallable)
	void SendClickData(UReceiveInteractionComponent* Sender, UObject* InContextObject);
	
#pragma endregion 
	
};
