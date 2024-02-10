// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GlobalConstantsDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FGamePlayerConstants
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;
	
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOCOMBAT_API UGlobalConstantsDataAsset : public UDataAsset
{
	GENERATED_BODY()

#pragma region DefaultPlayers

private:

	UPROPERTY(EditDefaultsOnly)
	FGamePlayerConstants NonPlayerDefaults;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGamePlayerConstants> DefaultPlayersData;

public:

	UFUNCTION(BlueprintPure)
	FGamePlayerConstants GetDefaultPlayerData(int32 PlayerIndex);

#pragma endregion

#pragma region ElementsDataBase

private:

	UPROPERTY(EditDefaultsOnly)
	TArray<UDataTable*> ElementsDataBase;

public:

	UFUNCTION(BlueprintPure)
	TArray<UDataTable*> GetElementsDataBase() const {return ElementsDataBase;}

#pragma endregion 
	
};
