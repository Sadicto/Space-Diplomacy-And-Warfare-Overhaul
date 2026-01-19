#include "stdafx.h"
#include "cUpliftedByMonolithAffinityModifier.h"
using namespace Simulator;

cUpliftedByMonolithAffinityModifier::cUpliftedByMonolithAffinityModifier()
{
}


cUpliftedByMonolithAffinityModifier::~cUpliftedByMonolithAffinityModifier()
{
}

// You can extend this function to return any other types your class implements.
void* cUpliftedByMonolithAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	CLASS_CAST(cUpliftedByMonolithAffinityModifier);
	return nullptr;
}

AffinityModifier cUpliftedByMonolithAffinityModifier::GetAffinityModifier() {
	return cUpliftedByMonolithAffinityModifier::affinityModifier;
}

bool cUpliftedByMonolithAffinityModifier::Active(const AffinityModifierContext& context) {
	return GetAsociatedPersistedDiplomacyEvent(context) != nullptr;
}
