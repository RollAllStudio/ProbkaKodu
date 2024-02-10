// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQuerryContext_TargetEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API UEnvQuerryContext_TargetEnemy : public UEnvQueryContext
{
	GENERATED_BODY()

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	
};
