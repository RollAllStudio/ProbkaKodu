// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VehicleElementsDataBase.generated.h"

class UVehicleElement;
class ABullet;

UENUM(BlueprintType, meta=(DisplayName = ElementType))
enum EVehicleElementType : UINT8
{
	ET_Chassis	UMETA(Displayname = Chassis),
	ET_Body		UMETA(Displayname = Body),
	ET_Weapon	UMETA(Displayname = Weapon)
};

UENUM(BlueprintType, meta=(DisplayName = BulletType))
enum EBulletType : UINT8
{
	Bt_HitScan		UMETA(Displayname = Hitscan),
	Bt_Projectile	UMETA(Displayname = Projectile),
	Bt_Missile		UMETA(Displayname = Missile)
};

USTRUCT(BlueprintType, meta=(DisplayName = BulletData))
struct FBulletsData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	TEnumAsByte<EBulletType> BulletType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	TSubclassOf<ABullet> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	TArray<FName> SpawnSockets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	float DisplayDPS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	TSoftObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	float AnimPlayRate;

	/*
	 *	Use as speed in projectile and missile type
	 *	Use as distance in HitScan type
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	float Range;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta=(EditCondition = "BulletType == EBulletType::Bt_Projectile || BulletType == EBulletType::Bt_Missile", EditConditionHides))
	TSoftObjectPtr<UStaticMesh> ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Missile, meta = (EditCondition = "BulletType == EBulletType::Bt_Missile", EditConditionHides))
	float ActivationTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Missile, meta = (EditCondition = "BulletType == EBulletType::Bt_Missile", EditConditionHides))
	float ChaseStartTime;
	
};

USTRUCT(BlueprintType, meta=(DisplayName = ElementData))
struct FVehicleElementData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	TSoftObjectPtr<UTexture2D> DisplayImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig)
	TEnumAsByte<EVehicleElementType> ElementType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseConfig, meta = (EditCondition = "ElementType != EVehicleElementType::ET_Chassis", EditConditionHides))
	TEnumAsByte<EVehicleElementType> CanBePlacedOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	float UnlockPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	float HealthPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	float Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	float ConstructTime;

	UPROPERTY(	EditAnywhere, BlueprintReadWrite, Category = GamePlay,
				meta = (EditCondition = "ElementType == EVehicleElementType::ET_Chassis", EditConditionHides))
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
	TSoftObjectPtr<USkeletalMesh> SkeletalMeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
	TSubclassOf<UAnimInstance> AnimationBlueprintClass;
	
	UPROPERTY(	EditAnywhere, BlueprintReadWrite, Category = WeaponExternalData,
				meta = (EditCondition = "ElementType == EVehicleElementType::ET_Weapon", EditConditionHides))
	FBulletsData BulletsData;
	
};

USTRUCT(BlueprintType)
struct FVehicleData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle ChassisDataReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle BodyDataReference;

	/*
	 * Chassis top weapon or body right weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle PrimaryWeaponDataReference;

	/*
	 * Body left weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle SecondaryWeaponDataReference;
	
};

USTRUCT(Blueprintable, BlueprintType)
struct FVehicleDisplayStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FullHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DPS;
	
};

/**
 * 
 */
UCLASS()
class ROBOCOMBAT_API UVehicleElementsDataBase : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta=(WorldContext = WorldContext))
	static bool ReadElementData(const FDataTableRowHandle InDataReference, FVehicleElementData& OutData, UObject* WorldContext);

	UFUNCTION(BlueprintCallable, meta=(WorldContext = WorldContext))
	static void ReadPossibleElements(const FDataTableRowHandle InElementData, TArray<FDataTableRowHandle> OutArray, UObject* WorldContext);

	UFUNCTION(BlueprintPure, meta=(WorldContext = WorldContext))
	static FVehicleDisplayStats GetVehicleDisplayStats(FVehicleData InVehicleData, UObject* WorldContext);
	
};
