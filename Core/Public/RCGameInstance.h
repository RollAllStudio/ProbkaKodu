// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RCGameInstance.generated.h"

class UGlobalConstantsDataAsset;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ROBOCOMBAT_API URCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UGlobalConstantsDataAsset* GlobalConstants;

public:

	UFUNCTION(BlueprintPure)
	UGlobalConstantsDataAsset* GetGlobalConstants() const {return GlobalConstants;}
	
};
