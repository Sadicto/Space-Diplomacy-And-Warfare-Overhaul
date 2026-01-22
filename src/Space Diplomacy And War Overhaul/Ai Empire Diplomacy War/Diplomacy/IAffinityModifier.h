#pragma once

#include <Spore\BasicIncludes.h>
#include "PersistedEvent/cPersistedDiplomacyEvent.h"
#include "Config/cArchetypesConfig.h"
#include "Config/cAffinityConfig.h"

#define IAffinityModifierPtr intrusive_ptr<IAffinityModifier>

struct AffinityModifierContext {
	Simulator::cEmpire* empire1;
	Simulator::cEmpire* empire2;
	uint32_t currentTime;
	eastl::vector<cPersistedDiplomacyEventPtr> diplomacyEvents;
	cArchetypesConfigPtr archetypesConfig;
	cAffinityConfigPtr affinityConfig;
};

struct AffinityModifierData {
	AffinityModifier affinityModifier;
	bool active;
	int affinityGain;
	bool upgrading;
	uint32_t upgradeTime;
	bool decaying;
	uint32_t decayTime;
	int priority;
	bool stableRelationsMutuallyExclusive;
	bool warTogetherMutuallyExclusive;
};

class IAffinityModifier 
	: public Object
{
public:
	static const uint32_t TYPE = id("IAffinityModifier");

	virtual AffinityModifier GetAffinityModifier() = 0;
	
	virtual bool Active(const AffinityModifierContext& context) = 0;

	virtual int GetAffinityGain(const AffinityModifierContext& context) = 0;

	virtual bool Upgrading(const AffinityModifierContext& context) = 0;

	virtual uint32_t GetUpgradeTime(const AffinityModifierContext& context) = 0;
	
	virtual bool Decaying(const AffinityModifierContext& context) = 0;

	virtual uint32_t GetDecayTime(const AffinityModifierContext& context) = 0;

	virtual bool PreventsWars(const AffinityModifierContext& context) = 0;

	virtual int GetPriority(const AffinityModifierContext& context) = 0;

	virtual bool StableRelationsMutuallyExclusive(const AffinityModifierContext& context) = 0;

	virtual bool WarTogetherMutuallyExclusive(const AffinityModifierContext& context) = 0;

	virtual AffinityModifierData GetAffinityModifierData(const AffinityModifierContext& context) = 0;
};
