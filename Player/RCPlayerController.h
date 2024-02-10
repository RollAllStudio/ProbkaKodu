// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "ReceiveInteractionComponent.h"
#include "GameFramework/PlayerController.h"
#include "RoboCombat/UI/BasePlayerHUD.h"
#include "RCPlayerController.generated.h"

class UMultipleSelectionComponent;
class APlayerPawn;
class UInputAction;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API ARCPlayerController : public APlayerController
{
	GENERATED_BODY()

	ARCPlayerController();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintGetter)
	ABasePlayerHUD* GetPlayerHUD() const {return Cast<ABasePlayerHUD>(GetHUD());}

#pragma region Interaction

private:

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetInteractionComponent, Category = Interaction)
	UReceiveInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TEnumAsByte<ECollisionChannel> SingleSelectTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TEnumAsByte<ECollisionChannel> MultiSelectTraceChannel;
	

public:

	UFUNCTION(BlueprintGetter)
	UReceiveInteractionComponent* GetInteractionComponent() const {return InteractionComponent;}
	
	UFUNCTION()
	void InteractionTrace();

#pragma endregion

#pragma region PawnConnection

private:
	
	UPROPERTY()
	APlayerPawn* PlayerPawn;

	UPROPERTY()
	UPawnMovementComponent* PlayerMovementComponent;

	virtual void OnPossess(APawn* InPawn) override;

#pragma endregion 

#pragma region EnhancedInputSetup

private:

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetInputMappingContext)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(BlueprintGetter = GetInputLocalSubsystem)
	UEnhancedInputLocalPlayerSubsystem* LocalInputSubsystem;

	UPROPERTY(BlueprintGetter = GetEnhancedInputComponent)
	UEnhancedInputComponent* EnhancedInputComponent;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* RotateAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* RotateSwitchAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* ScrollAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* PrimaryAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* SecondaryAction;

public:

	UFUNCTION(BlueprintGetter)
	UEnhancedInputLocalPlayerSubsystem* GetInputLocalSubsystem() const {return LocalInputSubsystem;}

	UFUNCTION(BlueprintGetter)
	UInputMappingContext* GetInputMappingContext() const {return InputMappingContext;}

	UFUNCTION(BlueprintGetter)
	UEnhancedInputComponent* GetEnhancedInputComponent() const {return EnhancedInputComponent;}

	void SetupEnhancedInput(UEnhancedInputComponent* InEnhancedInputComponent);

#pragma endregion

#pragma region InputActions

private:

	UPROPERTY(EditDefaultsOnly)
	float ScrollActionSpeed = -150;

	UPROPERTY(EditDefaultsOnly)
	FVector2D ScrollActionClamp = FVector2D(2500, 5000);

	UPROPERTY(EditDefaultsOnly)
	FVector2D PitchAxisRotationClamp = FVector2D(0, 20);

	APlayerPawn* GetMyPawn();

public:

	UFUNCTION()
	void MoveAction_Triggered(const FInputActionValue& Value);

	UFUNCTION()
	void RotateAction_Triggered(const FInputActionValue& Value);

	UFUNCTION()
	void RotateSwitchAction_Started(const FInputActionValue& Value);

	UFUNCTION()
	void RotateSwitchAction_Completed(const FInputActionValue& Value);

	UFUNCTION()
	void ScrollAction_Triggered(const FInputActionValue& Value);

	UFUNCTION()
	void PrimaryAction_Started(const FInputActionValue& Value);

	UFUNCTION()
	void PrimaryAction_Ongoing(const FInputActionValue& Value);

	UFUNCTION()
	void PrimaryAction_Completed(const FInputActionValue& Value);
	
	UFUNCTION()
	void SecondaryAction_Triggered(const FInputActionValue& Value);

#pragma endregion
	
};
