#include "stdafx.h"
#include "cCommonAllyAffinityModifier.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace Simulator;
using namespace SporeModUtils;

cCommonAllyAffinityModifier::cCommonAllyAffinityModifier()
{
}


cCommonAllyAffinityModifier::~cCommonAllyAffinityModifier()
{
}

// You can extend this function to return any other types your class implements.
void* cCommonAllyAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	CLASS_CAST(cCommonAllyAffinityModifier);
	return nullptr;
}

AffinityModifier cCommonAllyAffinityModifier::GetAffinityModifier()
{
	return cCommonAllyAffinityModifier::affinityModifier;
}

bool cCommonAllyAffinityModifier::Active(const AffinityModifierContext& context)
{
	return DiplomacyUtils::AllianceWithAllyOfEmpire(context.empire1, context.empire2);
}
