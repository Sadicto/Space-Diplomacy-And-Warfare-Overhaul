#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cDefeatedCommonEnemyAffinityModifierPtr intrusive_ptr<cDefeatedCommonEnemyAffinityModifier>

// Affinity modifier granted after two empires defeat a common enemy together.
// Its affinity gain decays over time.
class cDefeatedCommonEnemyAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cDefeatedCommonEnemyAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::DefeatedCommonEnemy;
	
	cDefeatedCommonEnemyAffinityModifier();
	~cDefeatedCommonEnemyAffinityModifier();
	void* Cast(uint32_t type) const override;

	int GetAffinityGain(const AffinityModifierContext& context) override;

	uint32_t GetDecayTime(const AffinityModifierContext& context) override;

	AffinityModifier GetAffinityModifier() override;

};
