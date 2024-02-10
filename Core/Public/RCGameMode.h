// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RCGameMode.generated.h"

class ARCPlayerController;
class ARCGameState;
/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API ARCGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UPROPERTY()
	ARCGameState* RCGameState;

#pragma region Points

private:

	UPROPERTY(EditDefaultsOnly)
	int StartBuildingPoints;	

public:
	
	UFUNCTION(BlueprintCallable)
	void ScorePlayer(const float InPoints, const int32 InPlayerIndex);

	UFUNCTION(BlueprintCallable)
	void PlayerSpendPoints(const int InPoints, const int32 InPlayerIndex);

	UFUNCTION(BlueprintCallable)
	void RefundPlayerPoints(const int InPoints, const int32 InPlayerIndex);
	
#pragma endregion 

#pragma region Players

private:
	
	UPROPERTY()
	TArray<ARCPlayerController*> PlayerControllers;

	virtual void PostLogin(APlayerController* NewPlayer) override;

#pragma endregion 
	
};