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

int cDefeatedCommonEnemyAffinityModifier::GetAffinityGain(const AffinityModifierContext& context){
	cPersistedDiplomacyEvent* defeatedCommonEnemyEvent = GetPersistedDiplomacyEventByType(context, PersistedDiplomacyEventType::DefeatedEnemyTogether);
	if (defeatedCommonEnemyEvent != nullptr) {
		return CalculateAffinityGain(context, defeatedCommonEnemyEvent->GetCreationTime());
	}
	else {
		return 0;
	}
}

uint32_t cDefeatedCommonEnemyAffinityModifier::GetDecayTime(const AffinityModifierContext& context){
	cPersistedDiplomacyEvent* defeatedCommonEnemyEvent = GetPersistedDiplomacyEventByType(context, PersistedDiplomacyEventType::DefeatedEnemyTogether);
	if (defeatedCommonEnemyEvent != nullptr) {
		return CalculateDecayTime(context, defeatedCommonEnemyEvent->GetCreationTime());
	}
	else {
		return 0;
	}
}

AffinityModifier cDefeatedCommonEnemyAffinityModifier::GetAffinityModifier(){
	return cDefeatedCommonEnemyAffinityModifier::affinityModifier;
}
