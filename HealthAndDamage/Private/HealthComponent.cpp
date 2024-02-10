// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboCombat/HealthAndDamage/Public/HealthComponent.h"

void UHealthComponent::SetMaxHealthPoints(const float InNewValue)
{
	if (InNewValue != MaxHealthPoints)
	MaxHealthPoints = InNewValue;
}

void UHealthComponent::SetHealthPoints(const float InNewValue)
{
	if (InNewValue != HealthPoints)
	{
		if (InNewValue <= 0 && HealthPoints > 0)
			OnHealthDepleted.Broadcast(this);
		
		HealthPoints = InNewValue;
		OnHealthChanged.Broadcast(HealthPoints);
	}
}

void UHealthComponent::ApplyDamage(const float InDamage)
{
	SetHealthPoints(GetHealthPoints() - InDamage);
}

UHealthComponent::UHealthComponent()
{

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	HealthPoints = MaxHealthPoints;
	Super::BeginPlay();
}

