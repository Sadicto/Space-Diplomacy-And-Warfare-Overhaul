#pragma once

#include <Spore\BasicIncludes.h>
#include "PersistedEvent/cPersistedDiplomacyEvent.h"
#include "Config/cArchetypesConfig.h"

#define IAffinityModifierPtr intrusive_ptr<IAffinityModifier>

struct AffinityModifierContext {
	Simulator::cEmpire* empire1;
	Simulator::cEmpire* empire2;
	uint32_t currentTime;
	eastl::vector<cPersistedDiplomacyEventPtr> diplomacyEvents;
	cArchetypesConfig* archetyipesConfig;
	cAffinityConfig* affinityConfig;
};

class IAffinityModifier 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("IAffinityModifier");
	
	bool virtual Applies(const AffinityModifierContext& context) = 0;

	int virtual GetAffinityGain(const AffinityModifierContext& context) = 0;

	bool virtual Upgrading(const AffinityModifierContext& context) = 0;

	uint32_t virtual GetUpgradeTime(const AffinityModifierContext& context) = 0;

	bool virtual Expiring(const AffinityModifierContext& context) = 0;

	uint32_t virtual GetExpireTime(const AffinityModifierContext& context) = 0;

	bool virtual PreventsWars(const AffinityModifierContext& context) = 0;

	int virtual GetPriority(const AffinityModifierContext& context) = 0;

	bool virtual StableRelationsMutuallyExclusive(const AffinityModifierContext& context) = 0;

	bool virtual WarTogetherMutuallyExclusive(const AffinityModifierContext& context) = 0;
	
};
