#include "Components/Characters/DataAssets/SkillDA.h"

void USkillDA::OnTableIDChanged()
{
	Super::OnTableIDChanged();
	Initialize();
}

void USkillDA::Initialize()
{
}
