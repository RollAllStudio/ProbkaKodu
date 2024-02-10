// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleFactory.h"

#include "InteractableComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RoboCombat/ControlPoint/ControlPoint.h"
#include "RoboCombat/Core/Public/RCGameMode.h"
#include "RoboCombat/Core/Public/RCGameState.h"
#include "RoboCombat/Player/RCPlayerController.h"
#include "RoboCombat/Player/RCPlayerState.h"
#include "RoboCombat/UI/BaseContextWidget.h"
#include "RoboCombat/UI/BasePlayerHUD.h"

AVehicleFactory::AVehicleFactory()
{
	bReplicates = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VehicleOutLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VehcileOutLocation"));
	VehicleDefaultTarget = CreateDefaultSubobject<USceneComponent>(TEXT("VehicleDefaultTarget"));
	BuildingBlockingVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BuildingBlockingVolume"));
	
	SetRootComponent(Mesh);
	VehicleOutLocation->SetupAttachment(Mesh);
	VehicleDefaultTarget->SetupAttachment(Mesh);
	BuildingBlockingVolume->SetupAttachment(Mesh);
	
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
}

void AVehicleFactory::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ARCGameMode>(UGameplayStatics::GetGameMode(this));
	GameState = Cast<ARCGameState>(UGameplayStatics::GetGameState(this));

	InteractableComponent->OnInteraction_Start.AddUniqueDynamic(this, &AVehicleFactory::OnInteractionStart);
	InteractableComponent->OnInteraction_End.AddUniqueDynamic(this, &AVehicleFactory::OnInteractionEnd);

	BuildingBlockingVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AVehicleFactory::OnBlockingVolumeBeginOverlap);
	BuildingBlockingVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &AVehicleFactory::OnBlockingVolumeEndOverlap);
	
}

void AVehicleFactory::OnBlockingVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVehicleActor* OtherVehicle = Cast<AVehicleActor>(OtherActor);

	if (OtherVehicle)
		BlockingActors.AddUnique(OtherVehicle);	
}

void AVehicleFactory::OnBlockingVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVehicleActor* OtherVehicle = Cast<AVehicleActor>(OtherActor);

	if (OtherVehicle)
	{
		BlockingActors.Remove(OtherVehicle);
		if (BlockingActors.Num() == 0)
		StartBuildingVehicle();
	}
}

void AVehicleFactory::OnInteractionStart(UReceiveInteractionComponent* ByComponent)
{
	ARCPlayerController* PC = Cast<ARCPlayerController>(ByComponent->GetOwner());
	if (PC)
	{
		ABasePlayerHUD* HUD = Cast<ABasePlayerHUD>(PC->GetHUD());
		HUD->OpenFactoryWidget(this);
	}
}

void AVehicleFactory::OnInteractionEnd(UReceiveInteractionComponent* ByComponent)
{
	ARCPlayerController* PC = Cast<ARCPlayerController>(ByComponent->GetOwner());
	if (PC)
	{
		ABasePlayerHUD* HUD = Cast<ABasePlayerHUD>(PC->GetHUD());
		bool WidgetExist;
		HUD->FindWidgetByClass(HUD->BottomPanelClass, WidgetExist)->SetContextObject(nullptr);
		HUD->SetHUDState(EPlayerHUDState::Phs_HUD);
	}
}

bool AVehicleFactory::CanInteract_Implementation(UReceiveInteractionComponent* ByComponent) const
{

	ARCPlayerController* PC = Cast<ARCPlayerController>(ByComponent->GetOwner());
	if (!PC)
		return false;
	
	ARCPlayerState* PlayerState = PC->GetPlayerState<ARCPlayerState>();
	if (PlayerState && GameState)
	{
		int PlayerIndex = GameState->PlayerArray.Find(PlayerState);
		if (PlayerIndex != -1 && PlayerIndex == ControlPoint->GetOwningPlayerIndex())
			
		return true;
	}
	
	return false;
}

void AVehicleFactory::PostEditControlPoint(AControlPoint* InNewPoint)
{
	if (InNewPoint != ControlPoint)
	{
		if (ControlPoint)
		ControlPoint->RemoveFactory(this);

		if (InNewPoint)
		InNewPoint->AddFactory(this);

		ControlPoint = InNewPoint;
	}
}

#if WITH_EDITOR
void AVehicleFactory::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AVehicleFactory, ControlPointBuffer))
	PostEditControlPoint(ControlPointBuffer);
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

FVehicleCreationData AVehicleFactory::ConstructVehicleCreationData(const FVehicleData InVehicleData)
{
	float OutCost = 0;
	float OutTime = 0;
	TArray<FDataTableRowHandle> ElementsToRead = {	InVehicleData.BodyDataReference,
													InVehicleData.ChassisDataReference,
													InVehicleData.PrimaryWeaponDataReference,
													InVehicleData.SecondaryWeaponDataReference};
	FVehicleElementData CurrentElementData;
	for (int i = 0; i < ElementsToRead.Num(); i++)
	{
		if (UVehicleElementsDataBase::ReadElementData(ElementsToRead[i], CurrentElementData, this))
		{
			OutCost += CurrentElementData.Cost;
			OutTime += CurrentElementData.ConstructTime;
		}
	}

	FVehicleCreationData OutData = FVehicleCreationData();
	OutData.Cost = OutCost;
	OutData.Time = OutTime;

	return OutData;
	
}

bool AVehicleFactory::CanPlayerBuildVehicle(float Cost)
{
	return GameState->GetPlayerByIndex(ControlPoint->GetOwningPlayerIndex())->GetBuildingPoints() >= Cost;
}

void AVehicleFactory::SpendBuildCost(float Cost)
{
	GameMode->PlayerSpendPoints(Cost, ControlPoint->GetOwningPlayerIndex());
}

void AVehicleFactory::RefundBuildCost(int32 QueueIndex)
{
	GameMode->RefundPlayerPoints(ConstructVehicleCreationData(BuildingQueue[QueueIndex]).Cost, ControlPoint->GetOwningPlayerIndex());
}

void AVehicleFactory::StartBuildingVehicle()
{
	if (BuildingQueue.Num() == 0 || BlockingActors.Num() != 0)
	return;

	VehicleCreationData = ConstructVehicleCreationData(BuildingQueue[0]);
	GetWorld()->GetTimerManager().SetTimer(BuildingTimer, this, &AVehicleFactory::EndBuildingVehicle, VehicleCreationData.Time);
	OnVehicleBuildingStart.Broadcast();
}

void AVehicleFactory::EndBuildingVehicle()
{
	ESpawnActorCollisionHandlingMethod CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AVehicleActor* NewVehicle = GetWorld()->SpawnActorDeferred<AVehicleActor>(VehicleSpawnClass, VehicleOutLocation->GetComponentTransform(), this, nullptr, CollisionHandlingMethod);
	NewVehicle->SetElementsData(GetCurrentBuildingVehicleData());
	NewVehicle->PlayerIndex = ControlPoint->GetOwningPlayerIndex();
	NewVehicle->FinishSpawning(VehicleOutLocation->GetComponentTransform());

	FVehicleData CreatedVehicleData = BuildingQueue[0];
	BuildingQueue.RemoveAt(0);
	OnVehicleBuildingStop.Broadcast();
	OnQueueEntryRemoved.Broadcast(0, CreatedVehicleData);
	StartBuildingVehicle();
}

void AVehicleFactory::AbortBuildingVehicle()
{
	GetWorld()->GetTimerManager().ClearTimer(BuildingTimer);
	OnVehicleBuildingStop.Broadcast();
	if (BuildingQueue.Num() > 0)
	StartBuildingVehicle();
}

void AVehicleFactory::AddToQueue(const FVehicleData InNewData)
{
	float Cost = ConstructVehicleCreationData(InNewData).Cost;
	if (!CanPlayerBuildVehicle(Cost))
		return;
		
	int32 NewEntryIndex = BuildingQueue.Add(InNewData);
	if (GameMode)
		SpendBuildCost(Cost);
	
	if (BuildingQueue.Num() == 1)
		StartBuildingVehicle();

	OnNewQueueEntry.Broadcast(NewEntryIndex, InNewData);
	
}

void AVehicleFactory::RemoveFromQueue(const int32 InElementIndex)
{
	if (!BuildingQueue.IsValidIndex(InElementIndex))
	return;

	if (GameMode)
	RefundBuildCost(InElementIndex);

	FVehicleData EntryData = BuildingQueue[InElementIndex];
	
	BuildingQueue.RemoveAt(InElementIndex);
	if (InElementIndex == 0)
		AbortBuildingVehicle();

	OnQueueEntryRemoved.Broadcast(InElementIndex, EntryData);
	
}

FVehicleData AVehicleFactory::GetCurrentBuildingVehicleData()
{
	if (BuildingQueue.Num() > 0)
		return BuildingQueue[0];
	
	return FVehicleData();
}

void AVehicleFactory::SwapQueueEntries(int32 FirstEntryIndex, int32 SecondEntryIndex)
{
	BuildingQueue.Swap(FirstEntryIndex, SecondEntryIndex);
	
	if (FirstEntryIndex == 0 || SecondEntryIndex == 0)
		AbortBuildingVehicle();
}

#if	WITH_EDITOR

void AVehicleFactory::AddDataToQueue()
{
	AddToQueue(AddToQueueData);
}

void AVehicleFactory::RemoveDataFromQueue()
{
	RemoveFromQueue(RemoveFromQueueData);
}

#endif