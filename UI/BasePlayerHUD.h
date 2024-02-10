// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BasePlayerHUD.generated.h"

class AVehicleActor;
class AVehicleFactory;
class UBaseContextWidget;

UENUM(BlueprintType)
enum EPlayerHUDState : UINT8
{
	Phs_Max				UMETA(DisplayName = Max_UseForMapDefualt),
	Phs_Default			UMETA(DisplayName = Defualt),
	Phs_HUD				UMETA(DisplayName = HUD),
	Phs_Vehicle			UMETA(DisplayName = Vehicle),
	Phs_Army			UMETA(DisplayName = Army),
	Phs_Factory			UMETA(DisplayName = Factory),
	Phs_ControlPoint	UMETA(DisplayName = ControlPoint),
	Phs_Menu			UMETA(DisplayName = Menu)
};

USTRUCT(BlueprintType)
struct FWidgetStateConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<UBaseContextWidget>, ESlateVisibility> WidgetsConfig;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHUDStateChanged_Signature, EPlayerHUDState, InNewState);

/**
 *	HUD class is context based - this means :
 *	Creating widgets is based on array of classes assigned to HUD state
 *	Widgets know state of HUD object and adjust their state to it
 *
 *	**Every widget should be designed as default NOT Visible
 *	**Every Base widget should inherit from UBaseContextWidget
 */

UCLASS(BlueprintType, Blueprintable)
class ROBOCOMBAT_API ABasePlayerHUD : public AHUD
{
	GENERATED_BODY()

#pragma region HUDState

private:
	
	/*
	 *	It's definition of what Widgets should be on viewport
	 *	when HUD changes state.
	 */
	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EPlayerHUDState>, FWidgetStateConfig> HUD_StatesConfig;

	UPROPERTY(BlueprintSetter = SetHUDState, BlueprintGetter = GetHUDState)
	TEnumAsByte<EPlayerHUDState> CurrentHUDState;

public:

	/*
	 *	Created for widgets, so they know when they should be
	 *	hidden or visible on HUD state changed.
	 *	You can use WidgetContextInterface.BindPlayerHUD
	 *	to bind functions to this delegate.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnHUDStateChanged_Signature OnHUDStateChanged;
	
	UFUNCTION(BlueprintSetter)
	void SetHUDState(const TEnumAsByte<EPlayerHUDState> InNewState);

	UFUNCTION(BlueprintGetter)
	TEnumAsByte<EPlayerHUDState> GetHUDState() const {return CurrentHUDState;}

	UFUNCTION(BlueprintPure)
	ESlateVisibility GetWidgetVisibilityByState(TEnumAsByte<EPlayerHUDState> InState, TSubclassOf<UBaseContextWidget> InWidgetClass);

#pragma endregion

#pragma region WidgetsContainer

private:
	
	UPROPERTY()
	TMap<TSubclassOf<UBaseContextWidget>, UBaseContextWidget*> WidgetsContainer;

public:

	UFUNCTION(BlueprintCallable)
	void ConstructWidget(TSubclassOf<UBaseContextWidget> InWidgetClass, UBaseContextWidget*& OutWidget, UObject* InContext);

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType = InWidgetClass))
	UBaseContextWidget* FindWidgetByClass(TSubclassOf<UBaseContextWidget> InWidgetClass, bool& IsWidgetValid) const;

	UFUNCTION()
	void ValidateWidgetsForState(TEnumAsByte<EPlayerHUDState> InState);

#pragma endregion

#pragma region WidgetsOpen

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBaseContextWidget> BottomPanelClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBaseContextWidget> FactoryWidgetClass;
	
	UFUNCTION(BlueprintCallable)
	void OpenFactoryWidget(AVehicleFactory* InFactoryActor);

	UFUNCTION(BlueprintGetter)
	UBaseContextWidget* GetBottomPanel() const;

	UFUNCTION(BlueprintCallable)
	void SetBottomPanelContext(UObject* InContext);

	/*
	 *	Clear context ONLY if current context is equal InClearContext
	 */
	UFUNCTION(BlueprintCallable)
	void TryClearBottomPanelContext(UObject* InClearContext);

#pragma endregion 
	
};