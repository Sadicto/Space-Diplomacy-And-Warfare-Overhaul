#include "stdafx.h"
#include "cLongAllianceAffinityModifier.h"

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