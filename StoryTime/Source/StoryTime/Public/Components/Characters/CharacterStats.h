#pragma once
#include "StatModifier.h"
#include "CharacterStats.generated.h"

struct FStatModifier;

USTRUCT(BlueprintType)
struct STORYTIME_API FCharacterStats
{
	GENERATED_USTRUCT_BODY()

	void Add(const FStatModifier& Modifier);
	bool Remove(const FStatModifier& Modifier);
	void Clear();

	bool RemoveAllFromSource();

	// Getters
	const FName& GetAlias() const& { return Alias; }
	const TArray<FStatModifier>& GetStatModifiers() const& { return StatModifiers; }

	UPROPERTY(BlueprintReadOnly)
	float BaseValue;

	uint32 ID = UINT_MAX;
private:
	float Calculate();

protected:
	/** Database variables */

	UPROPERTY(BlueprintReadOnly)
	int32 ParamId = UINT_MAX;

	UPROPERTY(BlueprintReadOnly)
	FText /* LocalizedString */ StatName;

	UPROPERTY(BlueprintReadOnly)
	FName Alias;

	UPROPERTY(BlueprintReadOnly)
	FString ParamFormula = "";

	UPROPERTY(BlueprintReadOnly)
	float Flat = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float Rate = 0.f;

	bool IsLocalized = false;
	// /* LocalizedSprite */ localizePreviewImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* PreviewImage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FStatModifier> StatModifiers = TArray<FStatModifier>();

private:
	bool m_bIsDirty = true;
	float m_iLastBaseValue = FLT_MIN;
	float m_fValue = 0.0f;
};
