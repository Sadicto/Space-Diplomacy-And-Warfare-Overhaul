#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cMadePeaceAffinityModifierPtr intrusive_ptr<cMadePeaceAffinityModifier>

class cMadePeaceAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("cMadePeaceAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::MadePeace;
	
	cMadePeaceAffinityModifier();
	~cMadePeaceAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;
};
