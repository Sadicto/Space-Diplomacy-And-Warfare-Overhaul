#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cUpliftedByMonolithAffinityModifierPtr intrusive_ptr<cUpliftedByMonolithAffinityModifier>

// Affinity modifier granted when one empire uplifts another via a monolith.
class cUpliftedByMonolithAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cUpliftedByMonolithAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::UpliftedByMonolith;
	
	cUpliftedByMonolithAffinityModifier();
	~cUpliftedByMonolithAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;

	bool Active(const AffinityModifierContext& context) override;
};
