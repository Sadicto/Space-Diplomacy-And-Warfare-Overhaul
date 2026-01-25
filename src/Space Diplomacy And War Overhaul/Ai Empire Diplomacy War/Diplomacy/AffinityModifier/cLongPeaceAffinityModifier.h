#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cLongPeaceAffinityModifierPtr intrusive_ptr<cLongPeaceAffinityModifier>

// Affinity modifier that increases over time while two empires remain at peace.
class cLongPeaceAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cLongPeaceAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::LongPeace;
	
	cLongPeaceAffinityModifier();
	~cLongPeaceAffinityModifier();
	void* Cast(uint32_t type) const override;

	bool Active(const AffinityModifierContext& context) override;

	int GetAffinityGain(const AffinityModifierContext& context) override;

	bool Upgrading(const AffinityModifierContext& context) override;

	virtual uint32_t GetUpgradeTime(const AffinityModifierContext& context) override;

	AffinityModifier GetAffinityModifier() override;
};
