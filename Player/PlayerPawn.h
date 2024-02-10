// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "PlayerPawn.generated.h"

class UMultipleSelectionComponent;
class UPawnMovementComponent;
class ARCPlayerController;
class USpringArmComponent;
class UCameraComponent;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class ROBOCOMBAT_API APlayerPawn : public ADefaultPawn
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:
	
	APlayerPawn();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Components

private:
	
	UPROPERTY(VisibleDefaultsOnly)
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintGetter = GetCameraBoom)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* PlayerCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintGetter = GetMultipleSelectionComponent)
	UMultipleSelectionComponent* MultipleSelectionComponent;

	UPROPERTY(VisibleDefaultsOnly)
	UDecalComponent* SelectionDecal;
	
public:

	UFUNCTION(BlueprintGetter)
	USpringArmComponent* GetCameraBoom() const {return CameraBoom;}

	UFUNCTION(BlueprintGetter)
	UMultipleSelectionComponent* GetMultipleSelectionComponent() const {return MultipleSelectionComponent;}

#pragma endregion

#pragma region InputsAndCotroller

private:

	UPROPERTY()
	ARCPlayerController* PlayerController;
	
public:

	

#pragma endregion 

};
