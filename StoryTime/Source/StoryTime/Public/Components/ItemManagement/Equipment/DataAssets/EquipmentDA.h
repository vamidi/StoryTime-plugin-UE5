// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ItemManagement/Inventory/DataAssets/ItemDA.h"
#include "EquipmentDA.generated.h"

enum class StatModType;

UENUM()
enum class EquipmentCategory : uint8
{
	Weapon		UMETA(DisplayName = "Weapon"),
	Shield		UMETA(DisplayName = "Shield"),
	Head		UMETA(DisplayName = "Head"),
	Body		UMETA(DisplayName = "Body"),
	Accessory	UMETA(DisplayName = "Accessory"),
};

UENUM()
enum class EquipmentType : uint8
{
	None 		UMETA(DisplayName = "None"),
	Sword 		UMETA(DisplayName = "Sword"),
	Armor 		UMETA(DisplayName = "Armor"),
	Dagger 		UMETA(DisplayName = "Dagger"),
	Axe 		UMETA(DisplayName = "Axe"),
	Bow 		UMETA(DisplayName = "Bow"),
	Gun 		UMETA(DisplayName = "Gun"),
	Spear 		UMETA(DisplayName = "Spear"),
	Necklace 	UMETA(DisplayName = "Necklace"),
};

UENUM()
enum class ClassType : uint8
{
	None 		UMETA(DisplayName = "None"),
};

USTRUCT(BlueprintType)
struct FEquipmentStat
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FName Alias;

	UPROPERTY()
	float Flat;

	UPROPERTY()
	StatModType StatType;
};

/**
 *
 */
UCLASS()
class STORYTIME_API UEquipmentDA : public UItemDA
{
	GENERATED_BODY()
public:
	UEquipmentDA() : Super("equipments", "name") { }

	/** Which stats this weapon gives */
	UFUNCTION(BlueprintGetter)
	TArray<FEquipmentStat> GetStats() const { return Stats; }

protected:
	virtual void Initialize() override;
	virtual void OnTableIDChanged() override;

	/** In which category this weapon/armor/accessory falls into. */
	UPROPERTY(BlueprintReadWrite)
	EquipmentCategory Category;

	/** The type of weapon or armor this equipment is. */
	UPROPERTY(BlueprintReadWrite)
	EquipmentType Type;

	/** The class where this equipment belongs to. */
	UPROPERTY(BlueprintReadWrite)
	ClassType ClassType;

	/** Which stats this weapon gives. */
	UPROPERTY(BlueprintReadOnly, BlueprintGetter = GetStats)
	TArray<FEquipmentStat> Stats = TArray<FEquipmentStat>();
};
