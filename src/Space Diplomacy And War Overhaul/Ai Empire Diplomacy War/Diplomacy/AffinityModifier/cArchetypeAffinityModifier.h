#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cArchetypeAffinityModifierPtr intrusive_ptr<cArchetypeAffinityModifier>

// Affinity modifier based on the archetype relationship between two empires.
// Always active and derives its affinity gain directly from the archetypes configuration.
class cArchetypeAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cArchetypeAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::ArchetypeAffinity;
	
	cArchetypeAffinityModifier();
	~cArchetypeAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;

	bool Active(const AffinityModifierContext& context) override;

	int GetAffinityGain(const AffinityModifierContext& context) override;
};
