#include "stdafx.h"
#include "cLongPeaceAffinityModifier.h"

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
