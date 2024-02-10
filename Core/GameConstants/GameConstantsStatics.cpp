// Fill out your copyright notice in the Description page of Project Settings.


#include "GameConstantsStatics.h"

#include "Kismet/GameplayStatics.h"
#include "RoboCombat/Core/Public/RCGameInstance.h"

UGlobalConstantsDataAsset* UGameConstantsStatics::GetGlobalConstants(UObject* WorldContext)
{
	URCGameInstance* GameInstance = Cast<URCGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));
	return GameInstance ? GameInstance->GetGlobalConstants() : nullptr;	
}

FGamePlayerConstants UGameConstantsStatics::GetDefaultPlayerData(int32 InIndex, UObject* WorldContext)
{
	UGlobalConstantsDataAsset* GlobalConstants = GetGlobalConstants(WorldContext);
	return GlobalConstants ? GlobalConstants->GetDefaultPlayerData(InIndex) : FGamePlayerConstants();
}

TArray<UDataTable*> UGameConstantsStatics::GetElementsDataBase(UObject* WorldContext)
{
	UGlobalConstantsDataAsset* GlobalConstants = GetGlobalConstants(WorldContext);
	return GlobalConstants ? GlobalConstants->GetElementsDataBase() : TArray<UDataTable*>{};
}
