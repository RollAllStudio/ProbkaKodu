// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboCombat/VehicleElements/Public/ChassisElementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RoboCombat/Database/Public/VehicleElementsDataBase.h"

void UChassisElementComponent::SetupElement(const FVehicleElementData InData)
{
	Super::SetupElement(InData);

	UCharacterMovementComponent* ParentMovementComponent = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (ParentMovementComponent)
		ParentMovementComponent->MaxWalkSpeed = InData.MovementSpeed;	
}
