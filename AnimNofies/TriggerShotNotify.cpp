// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerShotNotify.h"

#include "RoboCombat/VehicleElements/Public/WeaponElementComponent.h"

void UTriggerShotNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UWeaponElementComponent* WeaponElementComponent = Cast<UWeaponElementComponent>(MeshComp);
	if (WeaponElementComponent)
		WeaponElementComponent->SpawnBullets();
}
