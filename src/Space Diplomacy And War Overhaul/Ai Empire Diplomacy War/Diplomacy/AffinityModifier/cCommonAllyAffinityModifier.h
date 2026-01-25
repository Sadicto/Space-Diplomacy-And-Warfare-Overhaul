#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cCommonAllyAffinityModifierPtr intrusive_ptr<cCommonAllyAffinityModifier>

// Affinity modifier applied when two empires share a common ally.
// Active only while the diplomatic condition of having a common is active.
class cCommonAllyAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cCommonAllyAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::CommonAlly;
	
	cCommonAllyAffinityModifier();
	~cCommonAllyAffinityModifier();
	void* Cast(uint32_t type) const override;

	AffinityModifier GetAffinityModifier() override;

	bool Active(const AffinityModifierContext& context) override;
};
