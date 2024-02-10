// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoboCombat/VehicleActor/VehicleActor.h"
#include "VehiclePreview.generated.h"

class USceneCaptureComponent2D;
class UVehicleElement;

UCLASS()
class ROBOCOMBAT_API AVehiclePreview : public AActor
{
	GENERATED_BODY()

	AVehiclePreview();

#pragma region Components

public:
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVehicleElement* Chassis;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVehicleElement* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVehicleElement* ChassisWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVehicleElement* BodyRightWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVehicleElement* BodyLeftWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneCaptureComponent2D* SceneCaptureComponent;

#pragma endregion 

#pragma region VehicleConfig

private:

	UPROPERTY(BlueprintGetter = GetVehicleData, BlueprintSetter = SetVehicleData)
	FVehicleData VehicleData;

public:

	UFUNCTION(BlueprintSetter)
	void SetVehicleData(const FVehicleData InData);

	UFUNCTION(BlueprintGetter)
	FVehicleData GetVehicleData() const {return VehicleData;}

	UFUNCTION(BlueprintCallable)
	void SetChassisData(const FDataTableRowHandle InChassisData);

	UFUNCTION(BlueprintGetter)
	FDataTableRowHandle GetChassisData() const {return Chassis->GetVehicleElementDataReference();}

	UFUNCTION(BlueprintCallable)
	void SetChassisWeaponData(const FDataTableRowHandle InChassisWeaponData);

	UFUNCTION(BlueprintPure)
	FDataTableRowHandle GetChassisWeaponData() const {return ChassisWeapon->GetVehicleElementDataReference();}

	UFUNCTION(BlueprintCallable)
	void SetBodyData(const FDataTableRowHandle InBodyData);

	UFUNCTION(BlueprintPure)
	FDataTableRowHandle GetBodyData() const {return Body->GetVehicleElementDataReference();}

	UFUNCTION(BlueprintCallable)
	void SetPrimaryWeaponData(const FDataTableRowHandle InWeaponData);

	UFUNCTION(BlueprintGetter)
	FDataTableRowHandle GetWeaponData() const {return BodyRightWeapon->GetVehicleElementDataReference();}

	UFUNCTION(BlueprintCallable)
	void SetSecondaryWeaponData(const FDataTableRowHandle InWeaponData);

	UFUNCTION(BlueprintGetter)
	FDataTableRowHandle GetSecondaryWeaponData() const {return BodyLeftWeapon->GetVehicleElementDataReference();}

#pragma endregion

#pragma region EditorTool

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditInstanceOnly, Category = EditorTool)
	FVehicleData SetupData;

#endif

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION(CallInEditor, Category = EditorTool)
	void ForceChassisSetupData() {SetChassisData(SetupData.ChassisDataReference);}

	UFUNCTION(CallInEditor, Category = EditorTool)
	void ForceChassisWeaponSetupData() {SetChassisData(SetupData.PrimaryWeaponDataReference);}

	UFUNCTION(CallInEditor, Category = EditorTool)
	void ForceBodySetupData() {SetBodyData(SetupData.BodyDataReference);}

	UFUNCTION(CallInEditor, Category = EditorTool)
	void ForcePrimaryWeaponSetupData() {SetPrimaryWeaponData(SetupData.PrimaryWeaponDataReference);}

	UFUNCTION(CallInEditor, Category = EditorTool)
	void ForceSecondarySetupData() {SetSecondaryWeaponData(SetupData.SecondaryWeaponDataReference);}

#endif

#pragma endregion 
	
};
