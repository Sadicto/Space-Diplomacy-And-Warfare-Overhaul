#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cCommonEnemyAffinityModifierPtr intrusive_ptr<cCommonEnemyAffinityModifier>

// Affinity modifier applied when two empires share a common enemy.
// Active only while both empires are at war with the same empire.
class cCommonEnemyAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cCommonEnemyAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::CommonEnemy;
	
	cCommonEnemyAffinityModifier();
	~cCommonEnemyAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;

	bool Active(const AffinityModifierContext& context) override;
};
