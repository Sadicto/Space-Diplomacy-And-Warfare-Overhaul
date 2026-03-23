#include "stdafx.h"
#include "cLongAllianceAffinityModifier.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include "../PersistedEvent/cFormedAllianceEvent.h"
using namespace Simulator;
using namespace SporeModUtils;

cLongAllianceAffinityModifier::cLongAllianceAffinityModifier()
{
}


cLongAllianceAffinityModifier::~cLongAllianceAffinityModifier()
{
}

// You can extend this function to return any other types your class implements.
void* cLongAllianceAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	CLASS_CAST(cLongAllianceAffinityModifier);
	return nullptr;
}

AffinityModifier cLongAllianceAffinityModifier::GetAffinityModifier() {
	return cLongAllianceAffinityModifier::affinityModifier;
}

bool cLongAllianceAffinityModifier::Active(const AffinityModifierContext& context){
	return DiplomacyUtils::Alliance(context.empire1, context.empire2);
}

int cLongAllianceAffinityModifier::GetAffinityGain(const AffinityModifierContext& context){
	cPersistedDiplomacyEvent* formedAllianceDiplomacyEvent = GetPersistedDiplomacyEventByType(context, cFormedAllianceEvent::NOUN_ID);
	if (formedAllianceDiplomacyEvent != nullptr) {
		return CalculateAffinityGain(context, formedAllianceDiplomacyEvent->GetCreationTime());
	}
	else {
		return 0;
	}
}

bool cLongAllianceAffinityModifier::Upgrading(const AffinityModifierContext& context){
	return GetAffinityGain(context) != context.affinityConfig->GetAffinityGain(GetAffinityModifier());
}

uint32_t cLongAllianceAffinityModifier::GetUpgradeTime(const AffinityModifierContext& context){
	cPersistedDiplomacyEvent* formedAllianceDiplomacyEvent = GetPersistedDiplomacyEventByType(context, cFormedAllianceEvent::NOUN_ID);
	if (formedAllianceDiplomacyEvent != nullptr) {
		return CalculateUpgradeTime(context, formedAllianceDiplomacyEvent->GetCreationTime());
	}
	else {
		return 0;
	}
}
