// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboCombat/Core/Public/RCGameState.h"

#include "RoboCombat/Player/RCPlayerState.h"

void ARCGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARCGameState::AddPlayer(ARCPlayerState* InPlayer)
{
	Players.AddUnique(InPlayer);
}

ARCPlayerState* ARCGameState::GetPlayerByIndex(const int32 InPlayerIndex)
{
	if (Players.IsValidIndex(InPlayerIndex))
	{
		return Players[InPlayerIndex];
	}

	return nullptr;
	
}

void ARCGameState::ScorePlayer(const float Points, int32 InPlayerIndex)
{
	if (Players.IsValidIndex(InPlayerIndex))
	Players[InPlayerIndex]->AddPoints(Points);
}

void ARCGameState::SpendPlayerPoints(const float Points, int32 InPlayerIndex)
{
	if (Players.IsValidIndex(InPlayerIndex))
	Players[InPlayerIndex]->SpendPlayerPoints(Points);
}

void ARCGameState::RefundPlayerPoints(const float Points, int32 InPlayerIndex)
{
	if (Players.IsValidIndex(InPlayerIndex))
	Players[InPlayerIndex]->RefundPlayerPoints(Points);
}
