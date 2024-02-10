// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RCGameState.generated.h"

class ARCPlayerState;
/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API ARCGameState : public AGameStateBase
{
	GENERATED_BODY()

#pragma region Players

private:
	
	UPROPERTY(Replicated)
	TArray<ARCPlayerState*> Players;

	void virtual GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UFUNCTION(BlueprintCallable)
	void AddPlayer(ARCPlayerState* InPlayer);

	UFUNCTION(BlueprintPure)
	ARCPlayerState* GetPlayerByIndex(const int32 InPlayerIndex);

	void ScorePlayer(const float Points, int32 InPlayerIndex);

	void SpendPlayerPoints(const float Points, int32 InPlayerIndex);
	void RefundPlayerPoints(const float Points, int32 InPlayerIndex);

#pragma endregion
	
};
