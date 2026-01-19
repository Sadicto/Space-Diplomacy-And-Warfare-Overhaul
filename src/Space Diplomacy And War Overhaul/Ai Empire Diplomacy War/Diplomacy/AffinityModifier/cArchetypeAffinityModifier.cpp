#include "stdafx.h"
#include "cArchetypeAffinityModifier.h"

cArchetypeAffinityModifier::cArchetypeAffinityModifier()
{
}


cArchetypeAffinityModifier::~cArchetypeAffinityModifier()
{
}

// You can extend this function to return any other types your class implements.
void* cArchetypeAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	CLASS_CAST(cArchetypeAffinityModifier);
	return nullptr;
}

AffinityModifier cArchetypeAffinityModifier::GetAffinityModifier(){
	return cArchetypeAffinityModifier::affinityModifier;
}

bool cArchetypeAffinityModifier::Active(const AffinityModifierContext& context){
	return true;
}

int cArchetypeAffinityModifier::GetAffinityGain(const AffinityModifierContext& context){
	return context.archetypesConfig->GetArchetypesAffinity(context.empire1->mArchetype, context.empire2->mArchetype);
}
