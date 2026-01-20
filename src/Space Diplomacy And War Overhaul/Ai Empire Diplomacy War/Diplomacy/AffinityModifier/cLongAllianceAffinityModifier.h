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

	bool Active(const AffinityModifierContext& context) override;

	int GetAffinityGain(const AffinityModifierContext& context) override;

	bool Upgrading(const AffinityModifierContext& context) override;

	virtual uint32_t GetUpgradeTime(const AffinityModifierContext& context) override;

	AffinityModifier GetAffinityModifier() override;

};
