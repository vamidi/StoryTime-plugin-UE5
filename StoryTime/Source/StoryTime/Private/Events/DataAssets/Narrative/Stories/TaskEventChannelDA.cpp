#include "Events/DataAssets/Narrative/Stories/TaskEventChannelDA.h"
#include "Components/Narrative/Stories/DataAssets/TaskDA.h"
#include "Components/Narrative/Stories/DataAssets/TaskEventDA.h"

void UTaskEventChannelDA::RaiseEvent(UTaskDA* task, const UTaskEventDA* value)
{
	OnEventRaised.Broadcast(task, value);
}
