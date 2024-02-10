// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalConstantsDataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameConstantsStatics.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API UGameConstantsStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, meta=(WorldContext=WorldContext))
	static UGlobalConstantsDataAsset* GetGlobalConstants(UObject* WorldContext);

	UFUNCTION(BlueprintPure, meta=(WorldContext=WorldContext))
	static FGamePlayerConstants GetDefaultPlayerData(int32 InIndex, UObject* WorldContext);

	UFUNCTION(BlueprintPure, meta=(WorldContext=WorldContext))
	static TArray<UDataTable*> GetElementsDataBase(UObject* WorldContext);
	
};
