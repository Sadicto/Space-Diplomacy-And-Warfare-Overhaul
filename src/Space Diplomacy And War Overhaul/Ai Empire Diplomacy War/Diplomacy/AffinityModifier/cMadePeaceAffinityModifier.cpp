#include "stdafx.h"
#include "cMadePeaceAffinityModifier.h"

cMadePeaceAffinityModifier::cMadePeaceAffinityModifier()
{
}


cMadePeaceAffinityModifier::~cMadePeaceAffinityModifier()
{
}

// You can extend this function to return any other types your class implements.
void* cMadePeaceAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	CLASS_CAST(cMadePeaceAffinityModifier);
	return nullptr;
}

bool cMadePeaceAffinityModifier::Active(const AffinityModifierContext& context){
	return GetPersistedDiplomacyEventByType(context, PersistedDiplomacyEventType::MadePeace) != nullptr;
}

uint32_t cMadePeaceAffinityModifier::GetDecayTime(const AffinityModifierContext& context) {
	cPersistedDiplomacyEvent* madePeaceEvent = GetPersistedDiplomacyEventByType(context, PersistedDiplomacyEventType::MadePeace);
	if (madePeaceEvent != nullptr) {
		return CalculateDecayTime(context, madePeaceEvent->GetCreationTime());
	}
	else {
		return 0;
	}
}

AffinityModifier cMadePeaceAffinityModifier::GetAffinityModifier() {
	return cMadePeaceAffinityModifier::affinityModifier;
}
