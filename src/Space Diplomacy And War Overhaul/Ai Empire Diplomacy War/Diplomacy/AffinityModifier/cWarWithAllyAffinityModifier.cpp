#include "stdafx.h"
#include "cWarWithAllyAffinityModifier.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace Simulator;
using namespace SporeModUtils;

cWarWithAllyAffinityModifier::cWarWithAllyAffinityModifier()
{
}


cWarWithAllyAffinityModifier::~cWarWithAllyAffinityModifier()
{
}

// You can extend this function to return any other types your class implements.
void* cWarWithAllyAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	CLASS_CAST(cWarWithAllyAffinityModifier);
	return nullptr;
}

AffinityModifier cWarWithAllyAffinityModifier::GetAffinityModifier() {
	return cWarWithAllyAffinityModifier::affinityModifier;
}

bool cWarWithAllyAffinityModifier::Active(const AffinityModifierContext& context) {
	return DiplomacyUtils::AllianceWithEnemyOfEmpire(context.empire1, context.empire2) || DiplomacyUtils::AllianceWithEnemyOfEmpire(context.empire2, context.empire1);
}
