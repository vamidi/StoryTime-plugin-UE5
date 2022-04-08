#pragma once
#include "Components/Base/DataAssets/TableDA.h"
#include "CharacterClassDA.generated.h"

class USkillDA;
struct FCharacterStats;

UCLASS()
class STORYTIME_API UCharacterClassDA : public UTableDA
{
	GENERATED_BODY()
public:
	UCharacterClassDA() : Super("classes", "className") { }

	UFUNCTION(BlueprintGetter)
	TArray<FCharacterStats> GetCharacterStats() const{ return CharacterStats; }

	UFUNCTION(BlueprintGetter)
	TArray<TSoftObjectPtr<USkillDA>> GetSkills() const{ return Skills; }

	FCharacterStats* Find(const FName& alias);

protected:
	virtual void OnTableIDChanged() override;

private:
	void Initialize();
	void ClearModifiers();

protected:
	UPROPERTY(BlueprintReadOnly)
	FText /* LocalizedString */ ClassName;

	FString ExpCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter = GetCharacterStats)
	TArray<FCharacterStats> CharacterStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter = GetSkills)
	TArray<TSoftObjectPtr<USkillDA>> Skills;
};
