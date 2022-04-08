#pragma once

#include "TaskEventDA.generated.h"

enum class ETaskEventType : uint8;
class UCharacterDA;

UCLASS()
class STORYTIME_API UTaskEventDA : public UPrimaryDataAsset
{
	GENERATED_BODY()
protected:
	/** Actor reference. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UCharacterDA> Character = nullptr;

	/** Specify the type of the event. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETaskEventType TaskEventType;
};
