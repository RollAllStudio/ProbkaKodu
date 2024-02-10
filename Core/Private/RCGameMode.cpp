// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboCombat/Core/Public/RCGameMode.h"

#include "RoboCombat/Core/Public/RCCheatManager.h"
#include "RoboCombat/Core/Public/RCGameState.h"
#include "RoboCombat/Player/RCPlayerController.h"
#include "RoboCombat/Player/RCPlayerState.h"

void ARCGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!RCGameState)
	RCGameState = GetGameState<ARCGameState>();
}

void ARCGameMode::ScorePlayer(const float InPoints, const int32 InPlayerIndex)
{
	if (RCGameState)
	RCGameState->ScorePlayer(InPoints, InPlayerIndex);
}

void ARCGameMode::PlayerSpendPoints(const int InPoints, const int32 InPlayerIndex)
{
	if (RCGameState)
		RCGameState->SpendPlayerPoints(InPoints, InPlayerIndex);
}

void ARCGameMode::RefundPlayerPoints(const int InPoints, const int32 InPlayerIndex)
{
	if (RCGameState)
		RCGameState->RefundPlayerPoints(InPoints, InPlayerIndex);
}

void ARCGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerControllers.AddUnique(Cast<ARCPlayerController>(NewPlayer));

	if (!RCGameState)
	RCGameState = GetGameState<ARCGameState>();
	ARCPlayerState* LocalPlayerState = NewPlayer->GetPlayerState<ARCPlayerState>();

	RCGameState->AddPlayer(LocalPlayerState);
	LocalPlayerState->AddBuildingPoints(StartBuildingPoints);
}
