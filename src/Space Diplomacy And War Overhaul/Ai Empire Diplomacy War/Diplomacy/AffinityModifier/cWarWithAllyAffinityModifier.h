#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cWarWithAllyAffinityModifierPtr intrusive_ptr<cWarWithAllyAffinityModifier>

class cWarWithAllyAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("cWarWithAllyAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::WarWithAlly;
	
	cWarWithAllyAffinityModifier();
	~cWarWithAllyAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;

	bool Active(const AffinityModifierContext& context) override;
};
