#include "Components/Narrative/Stories/DataAssets/TaskDA.h"

#include "Events/DataAssets/Narrative/Stories/TaskEventChannelDA.h"

bool UTaskDA::Validate() const
{
	return m_Count >= RequiredCount;
}

void UTaskDA::Increment()
{
	m_Count++;
}

void UTaskDA::StartTask()
{
	if(StartTaskEvent) StartTaskEvent->RaiseEvent(this, TaskEvent.Get());
}

void UTaskDA::FinishTask()
{
	done = true;
	if (EndTaskEvent) EndTaskEvent->RaiseEvent(this, TaskEvent.Get());
}

void UTaskDA::OnTableIDChanged()
{
	Super::OnTableIDChanged();
	Initialize();
}

void UTaskDA::Initialize()
{
	m_Count = 0;
	done = false;
}
