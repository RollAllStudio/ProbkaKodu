// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged_Signature, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthDepleted_Signature, UHealthComponent*, InComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOCOMBAT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintGetter = GetMaxHealthPoints, BlueprintSetter = SetMaxHealthPoints, EditDefaultsOnly)
	float MaxHealthPoints = -1;

	UPROPERTY(BlueprintGetter = GetHealthPoints, BlueprintSetter = SetHealthPoints, VisibleAnywhere)
	float HealthPoints;

public:

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged_Signature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnHealthDepleted_Signature OnHealthDepleted;

	UFUNCTION(BlueprintSetter)
	void SetMaxHealthPoints(const float InNewValue);

	UFUNCTION(BlueprintGetter)
	float GetMaxHealthPoints() const {return MaxHealthPoints;}

	UFUNCTION(BlueprintSetter)
	void SetHealthPoints(const float InNewValue);

	UFUNCTION(BlueprintGetter)
	float GetHealthPoints() const {return HealthPoints;}

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(const float InDamage);
	
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
