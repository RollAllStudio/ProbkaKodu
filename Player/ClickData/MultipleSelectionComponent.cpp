// Fill out your copyright notice in the Description page of Project Settings.


#include "MultipleSelectionComponent.h"

#include "InteractableComponent.h"
#include "VectorMathLibrary.h"
#include "Components/DecalComponent.h"
#include "RoboCombat/Player/RCPlayerController.h"

UMultipleSelectionComponent::UMultipleSelectionComponent()
{
	SetCollisionProfileName(TEXT("SelectionComponent"));
}

void UMultipleSelectionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddUniqueDynamic(this, &UMultipleSelectionComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddUniqueDynamic(this, &UMultipleSelectionComponent::OnEndOverlap);

	ARCPlayerController* OwningPC = Cast<ARCPlayerController>(GetOwner()->GetInstigatorController());
	if (OwningPC) OwnerInteractionComponent = OwningPC->GetInteractionComponent();
	
}

void UMultipleSelectionComponent::SetupDecal(UDecalComponent* InDecal)
{
	if (!DynamicDecalMaterial) DynamicDecalMaterial = UMaterialInstanceDynamic::Create(DecalMaterial, this);
	InDecal->SetMaterial(0, DynamicDecalMaterial);
	DecalComponent = InDecal;
}

void UMultipleSelectionComponent::SetLocationAndSize(FVector InLocation, FVector InSize)
{
	SetWorldLocation(InLocation);
	SetBoxExtent(InSize);

	UpdateOverlaps();

	if (DecalComponent)
	{
		DecalComponent->SetWorldLocation(InLocation);
		DecalComponent->DecalSize = FVector(InSize.Z, InSize.Y, InSize.X);
	}
}

void UMultipleSelectionComponent::SetStartPoint(const FVector InStartPoint)
{
	StartPoint = InStartPoint;
	ClearSelection();
}

void UMultipleSelectionComponent::SetEndPoint(const FVector InEndPoint)
{
	EndPoint = InEndPoint;

	if (FVector::DistSquared(EndPoint, StartPoint) >= MinDrawThreshold)
	{
		if (!bIsCollisionOnScene)
		AddCollisionOnScene();
		
		DrawCollision();
		return;
	}

	if (bIsCollisionOnScene)
	RemoveCollisionFromScene();
	
}

void UMultipleSelectionComponent::DrawCollision()
{
	AddCollisionOnScene();
	FVector2d BoxSize = UVectorMathLibrary::GetSquareSize(FVector2d(StartPoint.X, StartPoint.Y), FVector2d(EndPoint.X, EndPoint.Y));
	SetLocationAndSize(UVectorMathLibrary::GetMidPoint(StartPoint, EndPoint), FVector(BoxSize.X, BoxSize.Y, CollisionHeight));
	UpdateOverlaps();
}

void UMultipleSelectionComponent::AddCollisionOnScene()
{
	bIsCollisionOnScene = true;
	SelectedInteractables.Empty();
}

void UMultipleSelectionComponent::RemoveCollisionFromScene()
{
	// Clear hover - if Distance allow to select, clearing will select them
	ClearHoveredInteractables(FVector::DistSquared(StartPoint, EndPoint) >= MinDrawThreshold);
	
	SetLocationAndSize(RemovedCollisionLocation, FVector(0,0,0));
	bIsCollisionOnScene = false;
}

void UMultipleSelectionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsCollisionOnScene)
	return;
	
	UInteractableComponent* OverlappedInteractable = Cast<UInteractableComponent>(OtherActor->GetComponentByClass(UInteractableComponent::StaticClass()));
	if (OverlappedInteractable)
		AddHoveredInteractable(OverlappedInteractable);
}

void UMultipleSelectionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIsCollisionOnScene)
	return;
	
	UInteractableComponent* OverlappedInteractable = Cast<UInteractableComponent>(OtherActor->GetComponentByClass(UInteractableComponent::StaticClass()));
	if (OverlappedInteractable)
		RemoveHoveredInteractable(OverlappedInteractable);
}

void UMultipleSelectionComponent::AddHoveredInteractable(UInteractableComponent* InInteractable)
{
	if (!HoveredInteractables.Contains(InInteractable))
	{
		HoveredInteractables.Add(InInteractable);
		OnInteractableHovered.Broadcast(InInteractable);
		InInteractable->OnHovered.Broadcast(OwnerInteractionComponent);
	}
}

void UMultipleSelectionComponent::RemoveHoveredInteractable(UInteractableComponent* InInteractable)
{
	if (HoveredInteractables.Contains(InInteractable))
	{
		HoveredInteractables.Remove(InInteractable);
		OnInteractableUnHovered.Broadcast(InInteractable);
		InInteractable->OnUnhovered.Broadcast(OwnerInteractionComponent);
	}
}

void UMultipleSelectionComponent::ClearSelection()
{
	while (SelectedInteractables.Num() > 0)
	{
		SelectedInteractables[0]->OnInteraction_End.Broadcast(OwnerInteractionComponent);
		SelectedInteractables.RemoveAt(0);
	}
}

void UMultipleSelectionComponent::ClearHoveredInteractables(bool bSelect)
{
	while(HoveredInteractables.Num() > 0)
	{
		UInteractableComponent* CurrentInteractable = HoveredInteractables[0];
		RemoveHoveredInteractable(CurrentInteractable);

		if (bSelect)
		{
			SelectedInteractables.AddUnique(CurrentInteractable);
			CurrentInteractable->OnInteraction_Start.Broadcast(OwnerInteractionComponent);
		}
	}
}

void UMultipleSelectionComponent::SendClickData(UReceiveInteractionComponent* Sender, UObject* InContextObject)
{
	for (auto CurrentInteractable : SelectedInteractables)
	{
		CurrentInteractable->ReceiveNewData(Sender, InContextObject);
	}
}
