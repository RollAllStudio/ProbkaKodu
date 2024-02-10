// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboCombat/Core/Public/RCCheatManager.h"

#include "Kismet/GameplayStatics.h"
#include "RoboCombat/Core/Public/RCGameMode.h"

void URCCheatManager::AddAbsolutePoints(int32 InPlayerIndex, float PointsToAdd)
{
	ARCGameMode* GameMode = Cast<ARCGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->ScorePlayer(PointsToAdd, InPlayerIndex);
	}
}

void URCCheatManager::AddBuildingPoints(int32 InPlayerIndex, float PointsToAdd)
{
	ARCGameMode* GameMode = Cast<ARCGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->RefundPlayerPoints(PointsToAdd, InPlayerIndex);
	}
}
