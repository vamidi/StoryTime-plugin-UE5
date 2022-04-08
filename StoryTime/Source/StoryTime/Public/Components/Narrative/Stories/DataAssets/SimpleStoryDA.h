#pragma once

#include "Components/Base/DataAssets/TableDA.h"
#include "SimpleStoryDA.generated.h"

// struct FDialogueLine;
class UCharacterDA;

UENUM()
enum class EStoryType : uint8
{
	All				UMETA(DisplayName = "All"),
	WorldQuests		UMETA(DisplayName = "WorldQuests"),
	SideQuests		UMETA(DisplayName = "SideQuests"),
	// Extend with own quest types.
};

UENUM()
enum class EDialogueType : uint8
{
	StartDialogue	UMETA(DisplayName = "StartDialogue"),
	WinDialogue		UMETA(DisplayName = "WinDialogue"),
	LoseDialogue	UMETA(DisplayName = "LoseDialogue"),
	DefaultDialogue	UMETA(DisplayName = "DefaultDialogue")
};

UENUM()
enum class EChoiceActionType : uint8
{
	DoNothing			UMETA(DisplayName = "DoNothing"),
	ContinueWithQuest	UMETA(DisplayName = "ContinueWithQuest"),
	ContinueWithTask	UMETA(DisplayName = "ContinueWithTask"),
};

class UCharacterDA;
UCLASS(Blueprintable)
class STORYTIME_API USimpleStoryDA : public UTableDA
{
	GENERATED_BODY()
public:
	USimpleStoryDA(): UTableDA("stories", "title", "parentId") { }

	UFUNCTION()
	virtual void Reset() override;

	UFUNCTION(BlueprintCallable)
	UCharacterDA* GetCharacter() { return Character.Get(); }

	UFUNCTION(BlueprintGetter)
	bool IsDone() const { return done; }

	UFUNCTION(BlueprintGetter)
	int32 GetParentID() const { return ParentId; }

	UFUNCTION(BlueprintGetter)
	int32 GetChildID() const { return ChildId; }

protected:
	virtual void ConvertRow() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Story Settings")
	TSoftObjectPtr<UCharacterDA> Character;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Story Settings")
	// TArray<FDialogueLine> DialogueLines;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter = GetParentID, Category = "Story Settings")
	int32 ParentId = UINT_MAX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter = GetChildID, Category = "Story Settings")
	int32 ChildId = UINT_MAX;

	UPROPERTY(BlueprintGetter = IsDone, Category = "Story Settings")
	bool done = false;
};

