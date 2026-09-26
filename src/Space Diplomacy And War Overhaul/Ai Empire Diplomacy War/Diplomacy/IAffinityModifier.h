#pragma once

#include <Spore\BasicIncludes.h>
#include "PersistedEvent/cPersistedDiplomacyEvent.h"
#include "Config/cArchetypesConfig.h"
#include "Config/cAffinityConfig.h"

#define IAffinityModifierPtr intrusive_ptr<IAffinityModifier>

// Context passed to affinity modifiers when evaluating their effect
// on the relationship between two empires.
struct AffinityModifierContext {
	Simulator::cEmpire* empire1;
	Simulator::cEmpire* empire2;
	uint32_t currentTime;
	eastl::vector<cPersistedDiplomacyEventPtr> diplomacyEvents;
	cArchetypesConfigPtr archetypesConfig;
	cAffinityConfigPtr affinityConfig;
};

// Aggregated data describing the state and effect of an affinity modifier.
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
	bool effective;
};

// Interface implemented by all affinity modifiers.
// Each modifier evaluates a specific diplomatic condition and
// contributes affinity based on current game state.
class IAffinityModifier 
	: public Object
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::IAffinityModifier");

	/// @brief Returns the affinity modifier type.
	virtual AffinityModifier GetAffinityModifier() = 0;
	
	/// @brief Returns whether the modifier is currently active.
	virtual bool Active(const AffinityModifierContext& context) = 0;

	/// @brief Returns the affinity gain contributed by this modifier.
	virtual int GetAffinityGain(const AffinityModifierContext& context) = 0;

	/// @brief Returns whether the modifier is currently upgrading.
	virtual bool Upgrading(const AffinityModifierContext& context) = 0;

	/// @brief Returns the upgrade time of the modifier.
	virtual uint32_t GetUpgradeTime(const AffinityModifierContext& context) = 0;
	
	/// @brief Returns whether the modifier is currently decaying.
	virtual bool Decaying(const AffinityModifierContext& context) = 0;

	/// @brief Returns the decay time of the modifier.
	virtual uint32_t GetDecayTime(const AffinityModifierContext& context) = 0;

	/// @brief Returns whether the modifier prevents wars.
	virtual bool PreventsWars(const AffinityModifierContext& context) = 0;

	/// @brief Returns the priority of the modifier when resolving conflicts.
	virtual int GetPriority(const AffinityModifierContext& context) = 0;

	/// @brief Returns whether the modifier is mutually exclusive
	/// with other stable-relations modifiers.
	virtual bool StableRelationsMutuallyExclusive(const AffinityModifierContext& context) = 0;

	/// @brief Returns whether the modifier is mutually exclusive
	/// with other war-together modifiers.
	virtual bool WarTogetherMutuallyExclusive(const AffinityModifierContext& context) = 0;

	/// @brief Returns the data structure describing this modifier.
	virtual AffinityModifierData GetAffinityModifierData(const AffinityModifierContext& context) = 0;
};
