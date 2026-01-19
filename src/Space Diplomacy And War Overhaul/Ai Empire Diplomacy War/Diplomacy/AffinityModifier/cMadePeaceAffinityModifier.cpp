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

AffinityModifier cMadePeaceAffinityModifier::GetAffinityModifier() {
	return cMadePeaceAffinityModifier::affinityModifier;
}
