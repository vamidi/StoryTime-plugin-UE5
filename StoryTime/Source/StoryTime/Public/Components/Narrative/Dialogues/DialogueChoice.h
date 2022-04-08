#pragma once

#include "DialogueChoice.generated.h"

struct FDialogueLine;
enum class EChoiceActionType : uint8;

USTRUCT(BlueprintType)
struct STORYTIME_API FDialogueChoice
{
	GENERATED_USTRUCT_BODY()

	uint32 ID = UINT_MAX;
protected:
	/// <summary>
	/// The text we use to display.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText /* LocalizedString */ Text;

	// This needs to be calculated
	FDialogueLine* NextDialogue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText EventName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EChoiceActionType ActionType;
};
