
#include "VehicleActor.h"

#include "VectorMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RoboCombat/AI/Vehicle/VehicleAIController.h"
#include "RoboCombat/Core/GameConstants/GameConstantsStatics.h"
#include "RoboCombat/Core/Public/RCGameState.h"
#include "RoboCombat/HealthAndDamage/Public/VehicleHealthComponent.h"
#include "RoboCombat/Player/RCPlayerController.h"
#include "RoboCombat/Player/RCPlayerState.h"
#include "RoboCombat/Player/ClickData/PlayerClickData.h"
#include "RoboCombat/VehicleElements/Public/ChassisElementComponent.h"
#include "RoboCombat/VehicleElements/Public/VehicleElement.h"

void AVehicleActor::BeginPlay()
{
	SetupElements();
	Super::BeginPlay();

	GameState = Cast<ARCGameState>(UGameplayStatics::GetGameState(this));

	HealthComponent->OnHealthDepleted.AddUniqueDynamic(this, &AVehicleActor::OnVehicleDied);
	
	WeaponRangeCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &AVehicleActor::OnWeaponRangeCollisionBeginOverlap);
	WeaponRangeCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &AVehicleActor::AVehicleActor::OnWeaponRangeCollisionEndOverlap);

	InteractableComponent->OnInteraction_Start.AddUniqueDynamic(this, &AVehicleActor::OnInteractionStart);
	InteractableComponent->OnInteraction_End.AddUniqueDynamic(this, &AVehicleActor::OnInteractionEnd);
	InteractableComponent->OnDataReceived.AddUniqueDynamic(this, &AVehicleActor::OnInteractionDataReceived);

	InteractableComponent->OnHovered.AddUniqueDynamic(this, &AVehicleActor::OnHovered);
	InteractableComponent->OnUnhovered.AddUniqueDynamic(this, &AVehicleActor::OnUnhovered);
	
	InteractionDecalMaterial = UMaterialInstanceDynamic::Create(InteractionDecal->GetMaterial(0), this);
	InteractionDecal->SetMaterial(0, InteractionDecalMaterial);

	OnStartRegisterOverlaps();
	
}

void AVehicleActor::OnVehicleDied(UHealthComponent* InHealthComponent)
{
	SetActorTickEnabled(false);
	TryActivateWeapons(false);
}

AVehicleActor::AVehicleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Destroy base character mesh
	GetMesh()->DestroyComponent();

	// Base character capsule adjust
	GetCapsuleComponent()->SetCapsuleHalfHeight(200.f);
	GetCapsuleComponent()->SetCapsuleRadius(200.f);

#pragma region VehicleElements
	
	ChassisElementComponent = CreateDefaultSubobject<UChassisElementComponent>(TEXT("Chassis"));
	ChassisElementComponent->SetupAttachment(RootComponent);

	BodyWeaponComponent			= CreateDefaultSubobject<UWeaponElementComponent>(TEXT("BodyWeaponComponent"));
	BodyElementComponent		= CreateDefaultSubobject<UVehicleElement>(TEXT("Body"));
	RightExtendWeapon		= CreateDefaultSubobject<UWeaponElementComponent>(TEXT("RightExtendWeapon"));
	LeftExtendWeapon			= CreateDefaultSubobject<UWeaponElementComponent>(TEXT("LeftExtendWeapon"));
	
	ChassisElementComponent->SetRelativeLocation(FVector(0,0, -1*GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	BodyWeaponComponent->SetupAttachment(ChassisElementComponent, TEXT("Child_Placement"));
	BodyElementComponent->SetupAttachment(ChassisElementComponent, TEXT("Child_Placement"));

	RightExtendWeapon->SetupAttachment(BodyElementComponent, TEXT("RightWeapon_Placement"));
	LeftExtendWeapon->SetupAttachment(BodyElementComponent, TEXT("LeftWeapon_Placement"));
	
	for (const auto Element : GetAllElements())
		Element->SetReceivesDecals(false);
	
#pragma endregion 

#pragma region Health&Range
	
	HealthComponent = CreateDefaultSubobject<UVehicleHealthComponent>(TEXT("HealthComponent"));
	WeaponRangeCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponRangeCollision"));
	WeaponRangeCollision->SetupAttachment(ChassisElementComponent);
	WeaponRangeCollision->SetCollisionProfileName(TEXT("WeaponRangeCollision"));

#pragma endregion 
	
#pragma region Interaction
	
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	
	InteractionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("InteractionDecal"));
	InteractionDecal->SetupAttachment(GetRootComponent());
	InteractionDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	InteractionDecal->SetVisibility(false);

	InteractionDecal->SetUsingAbsoluteRotation(true);
	
#pragma endregion

#pragma region OrderVisualisation

	OrderMeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrderMeshBase"));
	OrderMeshMain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrderMeshMain"));

	OrderMeshBase->SetupAttachment(GetRootComponent());
	OrderMeshMain->SetupAttachment(OrderMeshBase);

	OrderMeshBase->SetAbsolute(true, true, true);
	OrderMeshBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OrderMeshBase->SetVisibility(false, true);

	OrderMeshMain->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
#pragma endregion 
	
}

void AVehicleActor::UpdateOrderVisualisationMesh(EVehicleStateSelector InOrderType)
{
	switch (InOrderType)
	{
	case EVehicleStateSelector::Vss_Passive :
		OrderMeshMain->SetStaticMesh(MoveToOrderMesh);
		OrderMeshBase->SetMaterial(0, MoveToOrderMesh->GetMaterial(0));
		break;
	
	case EVehicleStateSelector::Vss_Aggressive :
		OrderMeshMain->SetStaticMesh(AttackOrderMesh);
		OrderMeshBase->SetMaterial(0, AttackOrderMesh->GetMaterial(0));
		break;
	
	case EVehicleStateSelector::Vss_Defensive :
		OrderMeshMain->SetStaticMesh(DefenseOrderMesh);
		OrderMeshBase->SetMaterial(0, DefenseOrderMesh->GetMaterial(0));
		break;
	}
}

void AVehicleActor::UpdateOrderVisualisationLocation(FVector InNewLocation)
{
	OrderMeshBase->SetWorldLocation(UVectorMathLibrary::AddZOffset(InNewLocation, OrderVisualiserZOffset));
}

void AVehicleActor::AddOrderVisualisationFloatingTime(float InDeltaTime)
{
	if (!OrderVisualisationFloatingCurve)
	return;

	float NewTime = OrderVisualisationFloatingTime + InDeltaTime;

	FVector2d CurveTimeRange;
	OrderVisualisationFloatingCurve->GetTimeRange(CurveTimeRange.X, CurveTimeRange.Y);

	if (NewTime > CurveTimeRange.Y)
		NewTime = NewTime - CurveTimeRange.Y + CurveTimeRange.X;

	OrderVisualisationFloatingTime = NewTime;
	OrderMeshMain->SetRelativeLocation(FVector(0,0, OrderVisualisationFloatingCurve->GetFloatValue(NewTime)));
}

TArray<UVehicleElement*> AVehicleActor::GetAllElements()
{
	return {ChassisElementComponent, BodyElementComponent, RightExtendWeapon, LeftExtendWeapon};
}

void AVehicleActor::SetElementsData(const FVehicleData InElementsData)
{
	ElementsData = InElementsData;
	SetupElements();
}

bool AVehicleActor::SetupElementChecked(UVehicleElement* InElement, const FDataTableRowHandle InElementData)
{

	InElement->SetVehicleElementDataReference(InElementData);
	
	if (InElementData.IsNull())
	return false;
	
	return true;
}

void AVehicleActor::SetupElements()
{
	if (SetupElementChecked(ChassisElementComponent, ElementsData.ChassisDataReference))
	{
		if (SetupElementChecked(BodyElementComponent, ElementsData.BodyDataReference))
		{
			BodyWeaponComponent->SetVehicleElementDataReference(FDataTableRowHandle());
			RightExtendWeapon->SetVehicleElementDataReference(ElementsData.PrimaryWeaponDataReference);
			LeftExtendWeapon->SetVehicleElementDataReference(ElementsData.SecondaryWeaponDataReference);
			SetupWeaponRange();
			return;
		}
		BodyWeaponComponent->SetVehicleElementDataReference(ElementsData.PrimaryWeaponDataReference);
		SetupWeaponRange();
		return;
	}

	// Clear all elements
	for (auto Element : GetAllElements())
		Element->SetVehicleElementDataReference(FDataTableRowHandle());
}

#if WITH_EDITOR
void AVehicleActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AVehicleActor, ElementsData))
		SetupElements();
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void AVehicleActor::SetupWeaponRange()
{
	WeaponRangeCollision->SetSphereRadius(GetWeaponMaxRange());

	if (VehicleAI)
		VehicleAI->SetWeaponsRangeInBlackBoard();
	
}

void AVehicleActor::SetDesiredShotTarget(AActor* InActor)
{
	DesiredShotTarget = InActor;
	SetTargetValid(DesiredShotTarget ? true : false);
}

float AVehicleActor::GetWeaponMinRange() const
{
	float MinValue = UKismetMathLibrary::Min(BodyWeaponComponent->BulletsData.Range ,UKismetMathLibrary::FMin(RightExtendWeapon->BulletsData.Range, LeftExtendWeapon->BulletsData.Range));
	if (MinValue > 0)
		return MinValue;
	return GetWeaponMaxRange();
}

float AVehicleActor::GetWeaponMaxRange() const
{
	return UKismetMathLibrary::Max(BodyWeaponComponent->BulletsData.Range, UKismetMathLibrary::Max(RightExtendWeapon->BulletsData.Range, LeftExtendWeapon->BulletsData.Range));
}

void AVehicleActor::SetTargetValid(bool InValue)
{
	if (InValue == bTargetValid)
	return;

	bTargetValid = InValue;
	FRotator CurrentWeaponRotation = GetWeaponRotationComponent()->GetComponentRotation();
	GetWeaponRotationComponent()->SetUsingAbsoluteRotation(bTargetValid);
	GetWeaponRotationComponent()->SetWorldRotation(CurrentWeaponRotation);
}

bool AVehicleActor::RotateWeapons(const float DeltaTime)
{
	const float LookAtTargetYaw = FMath::RInterpTo(GetWeaponRotationComponent()->GetComponentRotation(), GetDesiredWeaponFacingRotation(), DeltaTime, 1).Yaw;
	const bool ReturnBuffer = FMath::IsNearlyEqual(LookAtTargetYaw, GetWeaponRotationComponent()->GetComponentRotation().Yaw, FireRotationThreshold);
	GetWeaponRotationComponent()->SetWorldRotation(FRotator(0, LookAtTargetYaw, 0));
	return ReturnBuffer;	
}

void AVehicleActor::TryActivateWeapons(bool InNewActive)
{
	if (InNewActive) 
	{
		BodyWeaponComponent->StartFire(DesiredShotTarget);
		RightExtendWeapon->StartFire(DesiredShotTarget);
		LeftExtendWeapon->StartFire(DesiredShotTarget);
	}
	else
	{
		BodyWeaponComponent->StopFire();
		RightExtendWeapon->StopFire();
		LeftExtendWeapon->StopFire();
	}
}

FRotator AVehicleActor::GetDesiredWeaponFacingRotation() const
{
	
	return DesiredShotTarget ?	UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DesiredShotTarget->GetActorLocation()) :
								GetWeaponRotationComponent()->GetAttachParent()->GetSocketRotation(GetWeaponRotationComponent()->GetAttachSocketName());
}

void AVehicleActor::OnStartRegisterOverlaps()
{
	TArray<AActor*> OverlappingActors;
	WeaponRangeCollision->GetOverlappingActors(OverlappingActors, AVehicleActor::StaticClass());

	for (int i = 0; i < OverlappingActors.Num(); i ++)
		OnDetectedVehicle(Cast<AVehicleActor>(OverlappingActors[i]));
	
}

void AVehicleActor::RegisterEnemy(AVehicleActor* InEnemy)
{
	SeenEnemies.AddUnique(InEnemy);
	InEnemy->HealthComponent->OnHealthDepleted.AddUniqueDynamic(this, &AVehicleActor::OnEnemyDied);
}

void AVehicleActor::RegisterAlly(AVehicleActor* InAlly)
{
	SeenAllies.AddUnique(InAlly);
	InAlly->HealthComponent->OnHealthDepleted.AddUniqueDynamic(this, &AVehicleActor::OnAllyDied);
}

void AVehicleActor::OnAllyDied(UHealthComponent* InHealthComponent)
{
	AVehicleActor* AsVehicle = Cast<AVehicleActor>(InHealthComponent->GetOwner());
	OnVehicleMissed(AsVehicle);
}

void AVehicleActor::OnEnemyDied(UHealthComponent* InHealthComponent)
{
	AVehicleActor* AsVehicle = Cast<AVehicleActor>(InHealthComponent->GetOwner());
	OnVehicleMissed(AsVehicle);
}

void AVehicleActor::OnWeaponRangeCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVehicleActor* OtherVehicle = Cast<AVehicleActor>(OtherActor);
	if (OtherVehicle)
	OnDetectedVehicle(OtherVehicle);
}

void AVehicleActor::OnWeaponRangeCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVehicleActor* OtherVehicle = Cast<AVehicleActor>(OtherActor);
	if (OtherVehicle)
		OnVehicleMissed(OtherVehicle);
}

void AVehicleActor::UpdateInteractionDecal()
{
	if (bIsSelected)
	{
		InteractionDecal->SetVisibility(true);
		InteractionDecalMaterial->SetVectorParameterValue
		(TEXT("Color"),UGameConstantsStatics::GetDefaultPlayerData(PlayerIndex, this).Color);
		return;
	}
	if (bIsHovered)
	{
		InteractionDecal->SetVisibility(true);
		InteractionDecalMaterial->SetVectorParameterValue(TEXT("Color"),FColor::White);
		return;
	}
	
	InteractionDecal->SetVisibility(false);
}

void AVehicleActor::OnInteractionStart(UReceiveInteractionComponent* ByComponent)
{

	ARCPlayerController* PC = Cast<ARCPlayerController>(ByComponent->GetOwner());

	if (PC)
	{
		bIsSelected = true;
		PC->GetPlayerHUD()->SetBottomPanelContext(this);
		PC->GetPlayerHUD()->SetHUDState(EPlayerHUDState::Phs_Vehicle);
		UpdateInteractionDecal();

		OrderMeshBase->SetVisibility(true, true);
		OrderVisualisationFloatingTime = 0;
	}
	
	// Any code above blueprint call
	OnInteractionStart_Blueprint_Implementation(ByComponent);
}

void AVehicleActor::OnInteractionEnd(UReceiveInteractionComponent* ByComponent)
{

	ARCPlayerController* PC = Cast<ARCPlayerController>(ByComponent->GetOwner());

	if (PC)
	{
		bIsSelected = false;
		PC->GetPlayerHUD()->TryClearBottomPanelContext(this);
		UpdateInteractionDecal();

		OrderMeshBase->SetVisibility(false, true);
	}

	// Any code above blueprint call
	OnInteractionEnd_Blueprint_Implementation(ByComponent);
}

void AVehicleActor::OnInteractionDataReceived(UReceiveInteractionComponent* Sender, UObject* InDataObject)
{

	UPlayerClickData* PlayerClickData = Cast<UPlayerClickData>(InDataObject);
	if (PlayerClickData && HealthComponent->GetHealthPoints() > 0)
		VehicleAI->ReceivePlayerClickData(PlayerClickData);

	// Any code above blueprint call
	OnInteractionDataReceived_Blueprint_Implementation(Sender, InDataObject);
}

void AVehicleActor::OnHovered(UReceiveInteractionComponent* ByComponent)
{
	bIsHovered = true;
	UpdateInteractionDecal();
}

void AVehicleActor::OnUnhovered(UReceiveInteractionComponent* ByComponent)
{
	bIsHovered = false;
	UpdateInteractionDecal();
}

bool AVehicleActor::CanInteract_Implementation(UReceiveInteractionComponent* ByComponent) const
{
	ARCPlayerController* PC = Cast<ARCPlayerController>(ByComponent->GetOwner());
	if (!PC)
		return false;
	
	ARCPlayerState* LocalPlayerState = PC->GetPlayerState<ARCPlayerState>();
	if (LocalPlayerState && GameState)
	{
		int InteractingPlayerIndex = GameState->PlayerArray.Find(LocalPlayerState);
		if (InteractingPlayerIndex == PlayerIndex)
			
			return true;
	}
	
	return false;
}

void AVehicleActor::OnInteractionDataReceived_Blueprint_Implementation(UReceiveInteractionComponent* Sender,
                                                                       UObject* InDataObject)
{
}

void AVehicleActor::OnInteractionEnd_Blueprint_Implementation(UReceiveInteractionComponent* ByComponent)
{
}

void AVehicleActor::OnInteractionStart_Blueprint_Implementation(UReceiveInteractionComponent* ByComponent)
{
}

void AVehicleActor::OnVehicleMissed_Implementation(AVehicleActor* OverlappedVehicle)
{
	if (SeenEnemies.Contains(OverlappedVehicle))
	{
		SeenEnemies.Remove(OverlappedVehicle);
		OverlappedVehicle->HealthComponent->OnHealthDepleted.RemoveDynamic(this, &AVehicleActor::OnEnemyDied);
	}
	if (SeenAllies.Contains(OverlappedVehicle))
	{
		SeenAllies.Remove(OverlappedVehicle);
		OverlappedVehicle->HealthComponent->OnHealthDepleted.RemoveDynamic(this, &AVehicleActor::OnAllyDied);
	}
}

void AVehicleActor::OnDetectedVehicle_Implementation(AVehicleActor* OverlappedVehicle)
{
	if (OverlappedVehicle->PlayerIndex != PlayerIndex && OverlappedVehicle->HealthComponent->GetHealthPoints() > 0)
	{
		RegisterEnemy(OverlappedVehicle);
		return;
	}

	if (OverlappedVehicle != this)
	RegisterAlly(OverlappedVehicle);
	
}

void AVehicleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryActivateWeapons(RotateWeapons(DeltaTime) && DesiredShotTarget);

	if (bIsSelected)
	AddOrderVisualisationFloatingTime(DeltaTime);
}
