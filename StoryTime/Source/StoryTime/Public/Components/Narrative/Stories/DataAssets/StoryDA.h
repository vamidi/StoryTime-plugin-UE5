// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Narrative/Stories/DataAssets/SimpleStoryDA.h"
#include "StoryDA.generated.h"

class UTaskDA;
/**
 *
 */
UCLASS()
class STORYTIME_API UStoryDA : public USimpleStoryDA
{
	GENERATED_BODY()
public:
	void FinishStory() { done = true; }
protected:
	virtual void ConvertRow() override;

private:
	void Initialize();
protected:

	/** The collection of tasks composing the Quest. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UTaskDA>> Tasks = TArray<TSoftObjectPtr<UTaskDA>>();

	/** The title of the quest. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText /* LocalizedString */ Title;

	/** The description of the quest. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText/* LocalizedString */ Description;

	/** Show the type of the quest. i.e could be part of the main story. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EStoryType TypeId = EStoryType::WorldQuests;
};
