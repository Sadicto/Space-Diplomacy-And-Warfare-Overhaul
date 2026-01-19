#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cArchetypeAffinityModifierPtr intrusive_ptr<cArchetypeAffinityModifier>

class cArchetypeAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("cArchetypeAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::ArchetypeAffinity;
	
	cArchetypeAffinityModifier();
	~cArchetypeAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;

	bool Active(const AffinityModifierContext& context) override;

	int GetAffinityGain(const AffinityModifierContext& context) override;
};
