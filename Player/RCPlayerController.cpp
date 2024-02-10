// Fill out your copyright notice in the Description page of Project Settings.


#include "RCPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerPawn.h"
#include "ClickData/MultipleSelectionComponent.h"
#include "ClickData/PlayerClickData.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARCPlayerController::ARCPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
	InteractionComponent = CreateDefaultSubobject<UReceiveInteractionComponent>(TEXT("ReceiveInteractionComponent"));	
}

void ARCPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	InteractionTrace();
}

void ARCPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetShowMouseCursor(true);
}

void ARCPlayerController::InteractionTrace()
{
	bool TraceMultipleSelection = PlayerPawn->GetMultipleSelectionComponent()->IsCollisionOnScene();
	
	FHitResult LocalHit;
	GetHitResultUnderCursor(TraceMultipleSelection ? MultiSelectTraceChannel : SingleSelectTraceChannel, true, LocalHit);
	InteractionComponent->SetInteractionHitResult(LocalHit);
}

void ARCPlayerController::OnPossess(APawn* InPawn)
{
	PlayerPawn = Cast<APlayerPawn>(InPawn);
	PlayerMovementComponent = PlayerPawn->GetMovementComponent();
	Super::OnPossess(InPawn);
}

void ARCPlayerController::SetupEnhancedInput(UEnhancedInputComponent* InEnhancedInputComponent)
{
	EnhancedInputComponent = InEnhancedInputComponent;
	LocalInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	LocalInputSubsystem->ClearAllMappings();
	LocalInputSubsystem->AddMappingContext(InputMappingContext, 0);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARCPlayerController::MoveAction_Triggered);
	EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ARCPlayerController::RotateAction_Triggered);
	EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &ARCPlayerController::ScrollAction_Triggered);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ARCPlayerController::PrimaryAction_Started);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Ongoing, this, &ARCPlayerController::PrimaryAction_Ongoing);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Completed, this, &ARCPlayerController::PrimaryAction_Completed);
	EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Triggered, this, &ARCPlayerController::SecondaryAction_Triggered);
}

APlayerPawn* ARCPlayerController::GetMyPawn()
{
	if (PlayerPawn)
	return PlayerPawn;

	PlayerPawn = Cast<APlayerPawn>(GetPawn());
	PlayerMovementComponent = PlayerPawn->GetMovementComponent();
	return PlayerPawn;
}

void ARCPlayerController::MoveAction_Triggered(const FInputActionValue& Value)
{

	APlayerPawn* LocalPawn = GetMyPawn();
	if (!LocalPawn)
		return;
	
	FVector2d MoveValue = Value.Get<FVector2d>();
	FRotator MovementRotation = FRotator(0, GetControlRotation().Yaw, 0);

	if (MoveValue.Y != 0.f)
	{
		FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
		PlayerMovementComponent->AddInputVector(Direction * MoveValue.Y);
	}

	if (MoveValue.X != 0.f)
	{
		FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
		PlayerMovementComponent->AddInputVector(Direction * MoveValue.X);
	}
	
}

void ARCPlayerController::RotateAction_Triggered(const FInputActionValue& Value)
{
	FVector2D LookValue = Value.Get<FVector2D>();

	APlayerPawn* LocalPawn = GetMyPawn();
	if (!LocalPawn)
	return;

	if (LookValue.X != 0.f)
		AddYawInput(LookValue.X);

	if (LookValue.Y != 0.f)
	{
		FRotator CurrentRotation = PlayerPawn->GetRootComponent()->GetRelativeRotation();
		CurrentRotation.Pitch = UKismetMathLibrary::FClamp(CurrentRotation.Pitch + LookValue.Y, PitchAxisRotationClamp.X, PitchAxisRotationClamp.Y);
		PlayerPawn->GetRootComponent()->SetRelativeRotation(CurrentRotation);
	}
}

void ARCPlayerController::RotateSwitchAction_Started(const FInputActionValue& Value)
{
	
}

void ARCPlayerController::RotateSwitchAction_Completed(const FInputActionValue& Value)
{
	
}

void ARCPlayerController::ScrollAction_Triggered(const FInputActionValue& Value)
{

	APlayerPawn* LocalPawn = GetMyPawn();
	if (!LocalPawn)
		return;
	
	float ScrollValue = Value.Get<float>();

	if (ScrollValue != 0)
		PlayerPawn->GetCameraBoom()->TargetArmLength = UKismetMathLibrary::FClamp(	PlayerPawn->GetCameraBoom()->TargetArmLength
																					+ ScrollValue * ScrollActionSpeed,
																					ScrollActionClamp.X, ScrollActionClamp.Y);
}

void ARCPlayerController::PrimaryAction_Started(const FInputActionValue& Value)
{
	GetMyPawn()->GetMultipleSelectionComponent()->SetStartPoint(InteractionComponent->GetInteractionHitResult().Location);
}

void ARCPlayerController::PrimaryAction_Ongoing(const FInputActionValue& Value)
{
	GetMyPawn()->GetMultipleSelectionComponent()->SetEndPoint(InteractionComponent->GetInteractionHitResult().Location);
}

void ARCPlayerController::PrimaryAction_Completed(const FInputActionValue& Value)
{
	GetMyPawn()->GetMultipleSelectionComponent()->RemoveCollisionFromScene();
	if (GetMyPawn()->GetMultipleSelectionComponent()->IsAnythingSelected()) return;
	
	if (!InteractionComponent->TryStartInteract())
	{
		InteractionComponent->AbortInteractionTimer();
		InteractionComponent->ClearInteractedComponent();
	}
	
}

void ARCPlayerController::SecondaryAction_Triggered(const FInputActionValue& Value)
{
	UPlayerClickData* PlayerClickData = NewObject<UPlayerClickData>(this);
	PlayerClickData->SetHitResult(InteractionComponent->GetInteractionHitResult());

	if (GetMyPawn()->GetMultipleSelectionComponent()->IsAnythingSelected())
	{
		GetMyPawn()->GetMultipleSelectionComponent()->SendClickData(InteractionComponent, PlayerClickData);
		return;
	}
	
	InteractionComponent->SendData(PlayerClickData);
	
}
