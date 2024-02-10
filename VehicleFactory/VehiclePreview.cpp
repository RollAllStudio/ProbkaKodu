// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePreview.h"
#include "Components/SceneCaptureComponent2D.h"

AVehiclePreview::AVehiclePreview()
{
	Chassis = CreateDefaultSubobject<UVehicleElement>(TEXT("Chassis"));
	ChassisWeapon = CreateDefaultSubobject<UVehicleElement>(TEXT("ChassisWeapon"));
	Body = CreateDefaultSubobject<UVehicleElement>(TEXT("Body"));
	BodyRightWeapon = CreateDefaultSubobject<UVehicleElement>(TEXT("PrimaryWeapon"));
	BodyLeftWeapon = CreateDefaultSubobject<UVehicleElement>(TEXT("SecondaryWeapon"));
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));

	SetRootComponent(Chassis);
	ChassisWeapon->SetupAttachment(Chassis, TEXT("Child_Placement"));
	Body->SetupAttachment(Chassis,"Child_Placement");
	BodyLeftWeapon->SetupAttachment(Body, TEXT("LeftWeapon_Placement"));
	BodyRightWeapon->SetupAttachment(Body, TEXT("RightWeapon_Placement"));
	SceneCaptureComponent->SetupAttachment(Chassis);
}

void AVehiclePreview::SetVehicleData(const FVehicleData InData)
{
	SetChassisData(InData.ChassisDataReference);
	SetBodyData(InData.BodyDataReference);
	SetChassisWeaponData(InData.PrimaryWeaponDataReference);
	SetPrimaryWeaponData(InData.PrimaryWeaponDataReference);
	SetSecondaryWeaponData(InData.SecondaryWeaponDataReference);
}

void AVehiclePreview::SetChassisData(const FDataTableRowHandle InChassisData)
{
	VehicleData.ChassisDataReference = InChassisData;
	Chassis->SetVehicleElementDataReference(InChassisData);
}

void AVehiclePreview::SetChassisWeaponData(const FDataTableRowHandle InChassisWeaponData)
{
	if (VehicleData.BodyDataReference.IsNull())
	{
		ChassisWeapon->SetVehicleElementDataReference(InChassisWeaponData);
		VehicleData.PrimaryWeaponDataReference = InChassisWeaponData;

		SetSecondaryWeaponData(FDataTableRowHandle());
		SetPrimaryWeaponData(FDataTableRowHandle());
		return;
	}

	if (!ChassisWeapon->GetVehicleElementDataReference().IsNull())
	{
		ChassisWeapon->SetVehicleElementDataReference(FDataTableRowHandle());
		VehicleData.PrimaryWeaponDataReference = InChassisWeaponData;
	}
	
}

void AVehiclePreview::SetBodyData(const FDataTableRowHandle InBodyData)
{
	VehicleData.BodyDataReference = InBodyData;
	Body->SetVehicleElementDataReference(InBodyData);

	if (InBodyData.IsNull() && (!BodyLeftWeapon->GetVehicleElementDataReference().IsNull() || !BodyRightWeapon->GetVehicleElementDataReference().IsNull()))
	{
		SetPrimaryWeaponData(FDataTableRowHandle());
		SetSecondaryWeaponData(FDataTableRowHandle());
	}

	if (!InBodyData.IsNull() && !ChassisWeapon->GetVehicleElementDataReference().IsNull())
	SetChassisWeaponData(FDataTableRowHandle());	
}

void AVehiclePreview::SetPrimaryWeaponData(const FDataTableRowHandle InWeaponData)
{
	if (!VehicleData.BodyDataReference.IsNull())
	{
		VehicleData.PrimaryWeaponDataReference = InWeaponData;
		BodyRightWeapon->SetVehicleElementDataReference(InWeaponData);
		return;
	}

	BodyRightWeapon->SetVehicleElementDataReference(FDataTableRowHandle());
	
}

void AVehiclePreview::SetSecondaryWeaponData(const FDataTableRowHandle InWeaponData)
{
	if (!VehicleData.BodyDataReference.IsNull())
	{
		VehicleData.SecondaryWeaponDataReference = InWeaponData;
		BodyLeftWeapon->SetVehicleElementDataReference(InWeaponData);
		return;
	}
	
	BodyLeftWeapon->SetVehicleElementDataReference(FDataTableRowHandle());
}

#if WITH_EDITOR

void AVehiclePreview::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AVehiclePreview, SetupData))
	{
		SetChassisData(SetupData.ChassisDataReference);
		SetBodyData(SetupData.BodyDataReference);
		SetChassisWeaponData(SetupData.PrimaryWeaponDataReference);
		SetPrimaryWeaponData(SetupData.PrimaryWeaponDataReference);
		SetSecondaryWeaponData(SetupData.SecondaryWeaponDataReference);
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif
