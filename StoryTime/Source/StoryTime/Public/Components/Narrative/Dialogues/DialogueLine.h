#pragma once

#include "DialogueLine.generated.h"

struct FDialogueChoice;
enum class EDialogueType: uint8;

USTRUCT(BlueprintType)
struct STORYTIME_API FDialogueLine
{
	GENERATED_USTRUCT_BODY()

	uint32 ID = UINT_MAX;
protected:
	/// <summary>
	/// Calculated through the node editor data.
	/// </summary>
	FDialogueLine* NextDialogue = nullptr;

	uint32 nextDialogueID = UINT_MAX;

	UPROPERTY(EditDefaultsOnly)
	EDialogueType DialogueType;

	FText /* LocalizedString */ CharacterName;

	/** Sentence that will showed when interacting */
	UPROPERTY(EditDefaultsOnly)
	FText /* LocalizedString */ Sentence;

	/** Event that will be fired once filled in. */

	/** Choices the player can make. */
	UPROPERTY(EditDefaultsOnly)
	TArray<FDialogueChoice> m_Choices = TArray<FDialogueChoice>();
};
