#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cLongPeaceAffinityModifierPtr intrusive_ptr<cLongPeaceAffinityModifier>

class cLongPeaceAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("cLongPeaceAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::LongPeace;
	
	cLongPeaceAffinityModifier();
	~cLongPeaceAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;
};
