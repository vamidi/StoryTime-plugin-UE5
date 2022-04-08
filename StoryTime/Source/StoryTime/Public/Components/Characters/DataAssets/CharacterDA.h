// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/DataAssets/TableDA.h"
#include "CharacterDA.generated.h"

class UCharacterClassDA;
class UEquipmentDA;

/**
 *
 */
UCLASS(BlueprintType)
class STORYTIME_API UCharacterDA : public UTableDA
{
	GENERATED_BODY()
public:
	UCharacterDA(): UTableDA("characters", "name") { }

	UFUNCTION()
	void Equip(const UEquipmentDA* equipment) const;

	// Getters
	UFUNCTION(BlueprintGetter)
	TArray<TSoftObjectPtr<UEquipmentDA>> GetEquipments();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character settings")
	FText CharacterName;

	UPROPERTY(BlueprintReadOnly, Category = "Character settings")
	FText CharacterDescription;

	/** All the stats the character currently has. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Settings")
	TSoftObjectPtr<UCharacterClassDA> CharacterClass;

	UPROPERTY(BlueprintReadOnly, Category= "Level")
	int32 InitialLevel = 1;

	UPROPERTY(BlueprintReadWrite, Category= "Level")
	int32 CurrentLevel = INT_MAX;

	UPROPERTY(BlueprintReadWrite, Category= "Level")
	int32 MaxLevel = 99;

	UPROPERTY(BlueprintReadWrite, Category= "EXP")
	int32 CurrentExp = 0;

	UPROPERTY(BlueprintReadWrite, Category= "EXP")
	int32 MaxExp = 0;

	UPROPERTY(BlueprintReadOnly, BlueprintGetter = GetEquipments, Category= "EXP")
	TArray<TSoftObjectPtr<UEquipmentDA>> equipments = TArray<TSoftObjectPtr<UEquipmentDA>>();
};
