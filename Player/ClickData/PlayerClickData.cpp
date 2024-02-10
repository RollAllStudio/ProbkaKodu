// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerClickData.h"
#include "InteractableComponent.h"

AActor* UPlayerClickData::GetHitActor()
{

	AActor* HitResultActor = InHitResult.GetActor();
	
	if (HitResultActor)
	{
		UInteractableComponent* InteractableComponent = Cast<UInteractableComponent>(HitResultActor->GetComponentByClass(UInteractableComponent::StaticClass()));
		if (InteractableComponent)
		return HitResultActor;
	}

	return nullptr;
}
