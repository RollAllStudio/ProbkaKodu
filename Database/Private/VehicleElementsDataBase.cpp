// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RoboCombat/Database/Public/VehicleElementsDataBase.h"
#include "RoboCombat/Core/GameConstants/GameConstantsStatics.h"

bool UVehicleElementsDataBase::ReadElementData(const FDataTableRowHandle InDataReference, FVehicleElementData& OutData, UObject* WorldContext)
{
	if (InDataReference.DataTable)
	{
		const TArray<FName> Rows = InDataReference.DataTable->GetRowNames();
		if (Rows.Contains(InDataReference.RowName))
		{
			OutData = *InDataReference.DataTable->FindRow<FVehicleElementData>(InDataReference.RowName, "");
			return true;
		}
	}

	return false;
	
}

void UVehicleElementsDataBase::ReadPossibleElements(const FDataTableRowHandle InElementData, TArray<FDataTableRowHandle> OutArray, UObject* WorldContext)
{
	FVehicleElementData ElementData;
	if (!ReadElementData(InElementData, ElementData, WorldContext))
	return;

	TArray<UDataTable*> DataBase = UGameConstantsStatics::GetElementsDataBase(WorldContext);

	for (int i = 0; i < DataBase.Num(); i++)
	{
		if (UDataTable* CurrentTable = DataBase[i])
		{
			TArray<FName> RowNames = CurrentTable->GetRowNames();

			for (int j = 0; j < RowNames.Num(); j++)
			{
				FName CurrentRow = RowNames[j];
				FVehicleElementData CurrentElementData;
				
				FDataTableRowHandle NewHandle;
				NewHandle.DataTable = CurrentTable;
				NewHandle.RowName = CurrentRow;
				
				ReadElementData(FDataTableRowHandle(NewHandle), CurrentElementData, WorldContext);

				if (CurrentElementData.CanBePlacedOn == ElementData.ElementType)
				OutArray.AddUnique(NewHandle);
			}
		}
	}
}

FVehicleDisplayStats UVehicleElementsDataBase::GetVehicleDisplayStats(FVehicleData InVehicleData, UObject* WorldContext)
{
	FVehicleDisplayStats OutputBuffer = FVehicleDisplayStats();
	FVehicleElementData CurrentDataBuffer = FVehicleElementData();
	TArray<FDataTableRowHandle> ElementsDataRows = {InVehicleData.ChassisDataReference,
													InVehicleData.BodyDataReference,
													InVehicleData.PrimaryWeaponDataReference,
													InVehicleData.SecondaryWeaponDataReference};

	for (auto ElementDataRow : ElementsDataRows)
	{
		if (ReadElementData(ElementDataRow, CurrentDataBuffer, WorldContext))
		{
			OutputBuffer.MovementSpeed += CurrentDataBuffer.MovementSpeed;
			OutputBuffer.FullHP += CurrentDataBuffer.HealthPoints;
			OutputBuffer.DPS += CurrentDataBuffer.BulletsData.DisplayDPS;
		}
	}

	return OutputBuffer;
	
}