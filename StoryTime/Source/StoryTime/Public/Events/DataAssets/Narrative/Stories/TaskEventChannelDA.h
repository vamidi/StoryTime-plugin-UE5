#pragma once
#include "Events/DataAssets/EventChannelBaseDA.h"
#include "TaskEventChannelDA.generated.h"

class UTaskDA;
class UTaskEventDA;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTaskEventDelegate, UTaskDA*, task, const UTaskEventDA*, value);

UENUM()
enum class ETaskEventType: uint8
{
	CurrentCharacterRevision
};

UCLASS(BlueprintType)
class UTaskEventChannelDA : public UEventChannelBaseDA
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Task Channel Settings")
	FTaskEventDelegate OnEventRaised;

	/// <summary>
	///
	/// </summary>
	/// <param name="task"></param>
	/// <param name="value"></param>
	UFUNCTION(BlueprintCallable)
	void RaiseEvent(UTaskDA* task, const UTaskEventDA* value);
};
