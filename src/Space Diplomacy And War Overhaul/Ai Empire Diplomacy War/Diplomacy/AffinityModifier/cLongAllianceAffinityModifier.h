#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cLongAllianceAffinityModifierPtr intrusive_ptr<cLongAllianceAffinityModifier>

class cLongAllianceAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("cLongAllianceAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::LongAlliance;
	
	cLongAllianceAffinityModifier();
	~cLongAllianceAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;
};
