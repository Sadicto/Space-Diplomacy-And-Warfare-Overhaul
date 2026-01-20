#include "stdafx.h"
#include "cLongPeaceAffinityModifier.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace Simulator;
using namespace SporeModUtils;

cLongPeaceAffinityModifier::cLongPeaceAffinityModifier()
{
}


cLongPeaceAffinityModifier::~cLongPeaceAffinityModifier()
{
}

// You can extend this function to return any other types your class implements.
void* cLongPeaceAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	CLASS_CAST(cLongPeaceAffinityModifier);
	return nullptr;
}

AffinityModifier cLongPeaceAffinityModifier::GetAffinityModifier() {
	return cLongPeaceAffinityModifier::affinityModifier;
}

bool cLongPeaceAffinityModifier::Active(const AffinityModifierContext& context){
	return !DiplomacyUtils::War(context.empire1, context.empire2);
}

int cLongPeaceAffinityModifier::GetAffinityGain(const AffinityModifierContext& context) {
	cPersistedDiplomacyEvent* NeighborsInPeaceDiplomacyEvent = GetPersistedDiplomacyEventByType(context, PersistedDiplomacyEventType::NeighborsInPeace);
	if (NeighborsInPeaceDiplomacyEvent != nullptr) {
		return CalculateAffinityGain(context, NeighborsInPeaceDiplomacyEvent->GetCreationTime());
	}
	else {
		return 0;
	}
}

bool cLongPeaceAffinityModifier::Upgrading(const AffinityModifierContext& context) {
	return GetAffinityGain(context) != context.affinityConfig->GetAffinityGain(GetAffinityModifier());
}

uint32_t cLongPeaceAffinityModifier::GetUpgradeTime(const AffinityModifierContext& context) {
	cPersistedDiplomacyEvent* NeighborsInPeaceDiplomacyEvent = GetPersistedDiplomacyEventByType(context, PersistedDiplomacyEventType::NeighborsInPeace);
	if (NeighborsInPeaceDiplomacyEvent != nullptr) {
		return CalculateDecayTime(context, NeighborsInPeaceDiplomacyEvent->GetCreationTime());
	}
	else {
		return 0;
	}
}
