// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerHUD.h"
#include "BaseContextWidget.h"
#include "RoboCombat/VehicleFactory/VehicleFactory.h"

void ABasePlayerHUD::SetHUDState(const TEnumAsByte<EPlayerHUDState> InNewState)
{
	if (InNewState != CurrentHUDState)
	{
		CurrentHUDState = InNewState;
		ValidateWidgetsForState(CurrentHUDState);
		OnHUDStateChanged.Broadcast(CurrentHUDState);
	}
}

ESlateVisibility ABasePlayerHUD::GetWidgetVisibilityByState(
	TEnumAsByte<EPlayerHUDState> InState, TSubclassOf<UBaseContextWidget> InWidgetClass)
{
	if (!HUD_StatesConfig.Contains(InState))
	return ESlateVisibility::Collapsed; // If state is not configured, everything will be collapsed
	
	if (HUD_StatesConfig.Find(InState)->WidgetsConfig.Contains(InWidgetClass))
		return *HUD_StatesConfig.Find(InState)->WidgetsConfig.Find(InWidgetClass);

	// if widget is not configured by state -> return collapsed
	return ESlateVisibility::Collapsed;
}

void ABasePlayerHUD::ConstructWidget(TSubclassOf<UBaseContextWidget> InWidgetClass, UBaseContextWidget*& OutWidget, UObject* InContext)
{
	bool WidgetExist;
	OutWidget = FindWidgetByClass(InWidgetClass, WidgetExist);
	if (WidgetExist)
	return;

	OutWidget = Cast<UBaseContextWidget>(CreateWidget(GetOwningPlayerController(), InWidgetClass));
	OutWidget->SetHUDObject(this);
	OutWidget->SetContextObject(InContext);
	OutWidget->AddToViewport(0);
	WidgetsContainer.Add(InWidgetClass, OutWidget);
}

UBaseContextWidget* ABasePlayerHUD::FindWidgetByClass(TSubclassOf<UBaseContextWidget> InWidgetClass, bool& IsWidgetValid) const
{
	if (!WidgetsContainer.Contains(InWidgetClass))
	{
		IsWidgetValid = false;
		return nullptr;
	}
	
	UBaseContextWidget* OutWidget = *WidgetsContainer.Find(InWidgetClass);
	if (OutWidget)
	{
		IsWidgetValid = true;
		return OutWidget;
	}
	
	IsWidgetValid = false;
	return nullptr;
}

void ABasePlayerHUD::ValidateWidgetsForState(TEnumAsByte<EPlayerHUDState> InState)
{
	TArray<TSubclassOf<UBaseContextWidget>> StateWidgetClasses;

	if (!HUD_StatesConfig.Contains(InState))
	return;
	
	HUD_StatesConfig.Find(InState)->WidgetsConfig.GenerateKeyArray(StateWidgetClasses);
	for (int i = 0; i < StateWidgetClasses.Num(); i++)
	{
		TSubclassOf<UBaseContextWidget> CurrentClass = StateWidgetClasses[i];
		bool WidgetExist;
		UBaseContextWidget* CurrentWidget = FindWidgetByClass(CurrentClass, WidgetExist);
		
		if (!WidgetExist)
		ConstructWidget(CurrentClass, CurrentWidget, nullptr);
	}
}

void ABasePlayerHUD::OpenFactoryWidget(AVehicleFactory* InFactoryActor)
{
	SetHUDState(EPlayerHUDState::Phs_Factory);
	bool WidgetExist;
	FindWidgetByClass(BottomPanelClass, WidgetExist)->SetContextObject(InFactoryActor);
	FindWidgetByClass(FactoryWidgetClass, WidgetExist)->SetContextObject(InFactoryActor);
}

UBaseContextWidget* ABasePlayerHUD::GetBottomPanel() const
{
	bool IsWidgetValid;
	return FindWidgetByClass(BottomPanelClass, IsWidgetValid);
}

void ABasePlayerHUD::SetBottomPanelContext(UObject* InContext)
{
	if (GetBottomPanel())
		GetBottomPanel()->SetContextObject(InContext);
}

void ABasePlayerHUD::TryClearBottomPanelContext(UObject* InClearContext)
{
	if (GetBottomPanel())
	{
		if(GetBottomPanel()->GetContextObject(UObject::StaticClass()) == InClearContext)
		{
			GetBottomPanel()->SetContextObject(nullptr);
			SetHUDState(EPlayerHUDState::Phs_HUD);
		}
	}
}
