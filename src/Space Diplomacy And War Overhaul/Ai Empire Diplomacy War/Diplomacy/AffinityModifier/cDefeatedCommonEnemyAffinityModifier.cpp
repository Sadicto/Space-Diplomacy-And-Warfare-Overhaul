#include "stdafx.h"
#include "cDefeatedCommonEnemyAffinityModifier.h"
using namespace Simulator;

cDefeatedCommonEnemyAffinityModifier::cDefeatedCommonEnemyAffinityModifier()
{
}


cDefeatedCommonEnemyAffinityModifier::~cDefeatedCommonEnemyAffinityModifier()
{
}

// You can extend this function to return any other types your class implements.
void* cDefeatedCommonEnemyAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	CLASS_CAST(cDefeatedCommonEnemyAffinityModifier);
	return nullptr;
}

AffinityModifier cDefeatedCommonEnemyAffinityModifier::GetAffinityModifier(){
	return cDefeatedCommonEnemyAffinityModifier::affinityModifier;
}
