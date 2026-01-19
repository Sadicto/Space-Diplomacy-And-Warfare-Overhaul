#include "stdafx.h"
#include "cEmbassyAffinityModifier.h"
using namespace Simulator;

cEmbassyAffinityModifier::cEmbassyAffinityModifier()
{
}


cEmbassyAffinityModifier::~cEmbassyAffinityModifier()
{
}

// You can extend this function to return any other types your class implements.
void* cEmbassyAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	CLASS_CAST(cEmbassyAffinityModifier);
	return nullptr;
}

bool cEmbassyAffinityModifier::Active(const AffinityModifierContext& context){
	return false; // TODO.
}

AffinityModifier cEmbassyAffinityModifier::GetAffinityModifier() {
	return cEmbassyAffinityModifier::affinityModifier;
}
