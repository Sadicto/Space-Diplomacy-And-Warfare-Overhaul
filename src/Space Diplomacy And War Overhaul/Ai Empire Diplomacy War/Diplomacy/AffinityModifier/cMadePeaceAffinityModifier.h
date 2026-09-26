#pragma once

#include <Spore\BasicIncludes.h>
#include "cAffinityModifier.h"

#define cMadePeaceAffinityModifierPtr intrusive_ptr<cMadePeaceAffinityModifier>

// Affinity modifier applied after two empires make peace, prevents wars started by the AI.
class cMadePeaceAffinityModifier 
	: public cAffinityModifier
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cMadePeaceAffinityModifier");
	static const AffinityModifier affinityModifier = AffinityModifier::MadePeace;
	
	cMadePeaceAffinityModifier();
	~cMadePeaceAffinityModifier();
	void* Cast(uint32_t type) const override;

	bool Active(const AffinityModifierContext& context) override;

	uint32_t GetDecayTime(const AffinityModifierContext& context) override;

	AffinityModifier GetAffinityModifier() override;
};
