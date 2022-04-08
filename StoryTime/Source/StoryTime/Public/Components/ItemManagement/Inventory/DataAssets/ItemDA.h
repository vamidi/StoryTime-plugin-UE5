// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/DataAssets/TableDA.h"
#include "ItemDA.generated.h"

class UItemTypeDA;
struct FStatModifier;

/**
 *
 */
UCLASS()
class STORYTIME_API UItemDA : public UTableDA
{
	GENERATED_BODY()
public:
	UItemDA(const FString& name, const FString& dropdownColumn, const FString& linkedColumn = "",
		uint32 linkedId = UINT_MAX, const FString& linkedTable = "") : Super(name, dropdownColumn, linkedColumn, linkedId, linkedTable) { }

	UItemDA(): Super("items", "name") {}

	UFUNCTION(BlueprintGetter)
	TArray<FStatModifier> GetStatModifiers() { return StatModifiers; }

protected:
	virtual void Initialize();

	/** The name of the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText /* LocalizedString */ ItemName;

	/** A description of the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText /* LocalizedString */ Description;

	/** A preview image for the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* PreviewImage = nullptr;

	/** The type of item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UItemTypeDA> ItemType;

	/** A prefab reference for the model of the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UBlueprint> Prefab = nullptr;

	/** If the player is able to sell this item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool Sellable = false;

	/** If the item is sellable, how much will it cost. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	double SellValue = 0.0;

	/** Stat modifiers. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter = GetStatModifiers)
	TArray<FStatModifier> StatModifiers = TArray<FStatModifier>();

	/** A localized preview image for the item. */
	// UPROPERTY(BlueprintReadWrite)
	// LocalizedSprite localizePreviewImage;

	/** A checkbox for localized asset. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsLocalized = false;

};
