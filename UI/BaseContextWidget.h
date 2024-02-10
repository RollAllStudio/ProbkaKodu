// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "BaseContextWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOCOMBAT_API UBaseContextWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	ABasePlayerHUD* HUDObject;

	UPROPERTY()
	UObject* ContextObject;
	
public:

	UFUNCTION()
	void SetHUDObject(ABasePlayerHUD* InHUDObject);

	UFUNCTION(BlueprintCallable)
	void SetContextObject(UObject* InObject);

	UFUNCTION(BlueprintImplementableEvent)
	void OnContextChanged(UObject* InNewContext);

	UFUNCTION(BlueprintNativeEvent)
	void OnHUDStateChanged(EPlayerHUDState InNewState);

	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = ContextClass))
	UObject* GetContextObject(TSubclassOf<UObject> ContextClass) const {return ContextObject;}
	
};