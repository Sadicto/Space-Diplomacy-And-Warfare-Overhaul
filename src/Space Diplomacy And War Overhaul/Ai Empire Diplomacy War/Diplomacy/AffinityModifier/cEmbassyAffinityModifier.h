#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cEmbassyAffinityModifierPtr intrusive_ptr<cEmbassyAffinityModifier>

class cEmbassyAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("cEmbassyAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::Embassy;
	
	cEmbassyAffinityModifier();
	~cEmbassyAffinityModifier();
	void* Cast(uint32_t type) const override;

	bool Active(const AffinityModifierContext& context) override;

	AffinityModifier GetAffinityModifier() override;
};
