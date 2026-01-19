#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cDefeatedCommonEnemyAffinityModifierPtr intrusive_ptr<cDefeatedCommonEnemyAffinityModifier>

class cDefeatedCommonEnemyAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("cDefeatedCommonEnemyAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::DefeatedCommonEnemy;
	
	cDefeatedCommonEnemyAffinityModifier();
	~cDefeatedCommonEnemyAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;

};
