#include "stdafx.h"
#include "cCommonEnemyAffinityModifier.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace Simulator;
using namespace SporeModUtils;

cCommonEnemyAffinityModifier::cCommonEnemyAffinityModifier()
{
}


cCommonEnemyAffinityModifier::~cCommonEnemyAffinityModifier()
{
}

// You can extend this function to return any other types your class implements.
void* cCommonEnemyAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	CLASS_CAST(cCommonEnemyAffinityModifier);
	return nullptr;
}

AffinityModifier cCommonEnemyAffinityModifier::GetAffinityModifier(){
	return cCommonEnemyAffinityModifier::affinityModifier;
}

bool cCommonEnemyAffinityModifier::Active(const AffinityModifierContext& context){
	return DiplomacyUtils::CommonEnemy(context.empire1, context.empire2);
}
