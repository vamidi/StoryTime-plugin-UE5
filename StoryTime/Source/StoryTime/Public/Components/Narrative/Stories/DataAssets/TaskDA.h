#pragma once
#include "Components/Base/DataAssets/TableDA.h"
#include "TaskDA.generated.h"

struct FItemStack;
class UTaskEventChannelDA;
class UTaskEventDA;
class USimpleStoryDA;
class UCharacterDA;

UENUM()
enum class ETaskCompletionType : uint8
{
	None		UMETA(DisplayName = "None"), //
	Collect		UMETA(DisplayName = "Collect"), // collect an or multiple item(s) 1
	Defeat		UMETA(DisplayName = "Defeat"), // defeat a certain enemy. 2
	Talk		UMETA(DisplayName = "Talk"), // Talk to a npc. 3
	Interact	UMETA(DisplayName = "Interact"), // Interact with an object 4
	Defend		UMETA(DisplayName = "Defend") // Defend an object or npc. 5
};

UCLASS()
class STORYTIME_API UTaskDA : public UTableDA
{
	GENERATED_BODY()
public:
	UTaskDA(): Super("tasks", "description") {}

	bool Validate() const;
	void Increment();
	void StartTask();
	void FinishTask();

	UFUNCTION(BlueprintGetter)
	bool IsDone() const { return done; }

protected:
	virtual void OnTableIDChanged() override;

private:
	void Initialize();
protected:

	/** The description of the mission. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText /* LocalizedString */ Description;

	/** Whether the mission is hidden. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool Hidden;

	// the next task that the player needs to perform.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NextId = INT_MAX;

	// Reference to the interactable id, could be a monster or a npc
	// It could be even a reference to a group of monsters of some type.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 npc = INT_MAX;

	/** Which enemy category do we need to hunt. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 EnemyCategory = INT_MAX;

	// Reference to the parent, which is the quest.

	/** Requirement amount to complete the mission. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RequiredCount = 0;

	// Keep reference to the amount we killed or collected.
	UPROPERTY(BlueprintReadOnly)
	int32 m_Count;

	/** The Character this mission belongs or we will need to interact with. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UCharacterDA> Character = nullptr;

	/** The story that will be displayed before an action, if any. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USimpleStoryDA> StoryBeforeTask = nullptr;

	/** The story that will be displayed when the step is achieved. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USimpleStoryDA> CompleteStory = nullptr;

	/** The story that will be displayed if the step is not achieved yet. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USimpleStoryDA> IncompleteStory = nullptr;

	/** The item to check/give/reward (can be multiple) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemStack> Items;

	/** The type of the task. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETaskCompletionType Type;

	/** An event that we trigger when a certain task is started. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTaskEventChannelDA> StartTaskEvent;

	/** An event that we trigger when a certain task is finished. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTaskEventChannelDA> EndTaskEvent;

	/** Task event value we want to attach. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTaskEventDA> TaskEvent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool done;
};
