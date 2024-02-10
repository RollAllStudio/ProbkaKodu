// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "RCPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "ClickData/MultipleSelectionComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	MultipleSelectionComponent->SetupDecal(SelectionDecal);
	MultipleSelectionComponent->RemoveCollisionFromScene();
}

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerCollision"));

	GetMeshComponent()->DestroyComponent();
	GetCollisionComponent()->DestroyComponent();
	
	SetRootComponent(SceneRoot);
	CameraBoom->SetupAttachment(SceneRoot);
	PlayerCamera->SetupAttachment(CameraBoom);
	PlayerCollision->SetupAttachment(SceneRoot);

	PlayerCollision->SetSphereRadius(32.0f);
	PlayerCollision->SetCollisionProfileName(TEXT("PlayerCollision"));

	MultipleSelectionComponent = CreateDefaultSubobject<UMultipleSelectionComponent>(TEXT("MultipleSelectionComponent"));
	SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionDecalComponent"));

	MultipleSelectionComponent->SetupAttachment(RootComponent);
	SelectionDecal->SetupAttachment(MultipleSelectionComponent);
	SelectionDecal->DecalSize = FVector(1,1,1);

	MultipleSelectionComponent->SetAbsolute(true, true, false);
	
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerController = Cast<ARCPlayerController>(GetController());
	PlayerController->SetupEnhancedInput(Cast<UEnhancedInputComponent>(PlayerInputComponent));
}