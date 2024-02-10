// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalConstantsDataAsset.h"

FGamePlayerConstants UGlobalConstantsDataAsset::GetDefaultPlayerData(int32 PlayerIndex)
{
	return DefaultPlayersData.IsValidIndex(PlayerIndex) ? DefaultPlayersData[PlayerIndex] : NonPlayerDefaults;
}
