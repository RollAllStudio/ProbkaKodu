// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseContextWidget.h"

#include "BasePlayerHUD.h"

void UBaseContextWidget::SetHUDObject(ABasePlayerHUD* InHUDObject)
{
	if (InHUDObject != HUDObject)
	{
		HUDObject = InHUDObject;
		HUDObject->OnHUDStateChanged.AddUniqueDynamic(this, &UBaseContextWidget::OnHUDStateChanged);
	}
}

void UBaseContextWidget::SetContextObject(UObject* InObject)
{
	if (ContextObject != InObject)
	{
		ContextObject = InObject;
		OnContextChanged(ContextObject);
	}
}

void UBaseContextWidget::OnHUDStateChanged_Implementation(EPlayerHUDState InNewState)
{
	SetVisibility(HUDObject->GetWidgetVisibilityByState(InNewState, GetClass()));
}