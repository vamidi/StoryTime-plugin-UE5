#pragma once
#include "Components/Base/DataAssets/TableDA.h"
#include "SkillDA.generated.h"

UENUM()
enum class EDamageType : uint8
{
	Damage	UMETA(DisplayName = "Damage"),
	Recover	UMETA(DisplayName = "Recover"),
	Drain	UMETA(DisplayName = "Drain"),
};

UCLASS()
class STORYTIME_API USkillDA : public UTableDA
{
	GENERATED_BODY()
public:
	USkillDA(): Super("skills", "name") {}

protected:
	virtual void OnTableIDChanged() override;

	/** Used for validation to where this skill belongs to. */
	UPROPERTY(BlueprintReadOnly, Category = "General")
	int32 ClassId;

	/** Can this skill critical hit. */
	UPROPERTY(BlueprintReadOnly, Category = "General")
	bool CriticalChance = false;

	UPROPERTY(BlueprintReadOnly, Category = "General")
	FText /* LocalizedString */ SkillName;

	UPROPERTY(BlueprintReadOnly, Category = "General")
	FText /* LocalizedString */ Description;

	/** Required level for this skill. */
	UPROPERTY(BlueprintReadOnly, Category = "General")
	int32 level = 1;

	/** Attribute we are going to use to subtract the magic cost. */
	UPROPERTY(BlueprintReadOnly, Category = "General")
	FString MagicCurve;

	UPROPERTY(BlueprintReadOnly, Category = "General")
	int32 MagicCost = INT_MAX;

	/** How many enemies/players can this skill hit. */
	UPROPERTY(BlueprintReadOnly, Category = "General")
	int32 Scope = INT_MAX;

	/** How many enemies/players can this skill hit". */
	UPROPERTY(BlueprintReadOnly, Category = "Invocation")
	float speed = INT_MAX;

	/** Calculation rate. */
	UPROPERTY(BlueprintReadOnly, Category = "Invocation")
	float successRate = INT_MAX;

	/** How many times the skill should be repeated. */
	UPROPERTY(BlueprintReadOnly, Category = "Invocation")
	int32 Repeat = INT_MAX;

	/** Attribute we are going to attack/heal on. */
	UPROPERTY(BlueprintReadOnly, Category = "Damage Settings")
	FString Parameter;

	/** Type of damage this skill can do. */
	UPROPERTY(BlueprintReadOnly, Category = "Damage Settings")
	EDamageType Type = EDamageType::Damage;

	/** Formula we are going to use to calculate our dmg/rec/drain. */
	UPROPERTY(BlueprintReadOnly, Category = "Damage Settings")
	FString Formula = "";

	/** How much off we can be from the actual dmg/rec/drain (use this for randomness). */
	UPROPERTY(BlueprintReadOnly, Category = "Damage Settings")
	float Variance = 0.0f;

private:
	void Initialize();
};
