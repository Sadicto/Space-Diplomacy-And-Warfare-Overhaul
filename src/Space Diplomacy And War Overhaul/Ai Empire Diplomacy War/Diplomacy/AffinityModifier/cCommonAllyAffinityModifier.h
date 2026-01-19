#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cCommonAllyAffinityModifierPtr intrusive_ptr<cCommonAllyAffinityModifier>

class cCommonAllyAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("cCommonAllyAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::CommonAlly;
	
	cCommonAllyAffinityModifier();
	~cCommonAllyAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;

	bool Active(const AffinityModifierContext& context) override;
};
