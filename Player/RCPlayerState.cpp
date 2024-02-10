// Fill out your copyright notice in the Description page of Project Settings.


#include "RCPlayerState.h"

#include "Kismet/KismetMathLibrary.h"

void ARCPlayerState::AddPoints(const float InPoints)
{
	PointsAbsolute += InPoints;
	SetDisplayPoints(UKismetMathLibrary::FFloor(PointsAbsolute));
}

void ARCPlayerState::SetDisplayPoints(const int InNewValue)
{
	if (DisplayPoints != InNewValue)
	{
		int Dif = InNewValue - DisplayPoints;
		DisplayPoints = InNewValue;
		BuildingPoints += Dif;
	}
}
