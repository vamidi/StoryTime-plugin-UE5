#pragma once

#include "EventChannelBaseDA.generated.h"

UCLASS(BlueprintType)
class STORYTIME_API UEventChannelBaseDA : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;
};
