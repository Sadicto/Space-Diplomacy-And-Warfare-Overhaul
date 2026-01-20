#pragma once

#include <Spore\BasicIncludes.h>

#define cAffinityConfigPtr intrusive_ptr<cAffinityConfig>

/// Defines the different modifiers that affect the affinity between empires.
enum class AffinityModifier {
	ArchetypeAffinity = 0,
	CommonEnemy = 1,
	CommonAlly = 2,
	WarWithAlly = 3,
	DefeatedCommonEnemy = 4,
	UpliftedByMonolith = 5,
	MadePeace = 6,
	LongPeace = 7,
	LongAlliance = 8,
	Embassy = 9,
	Nothing = MAXINT
};

/// Stores and provides access to config values for every affinity modifier.
class cAffinityConfig 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cAffinityConfig");
	
	cAffinityConfig(ResourceKey affinityConfigKey);
	~cAffinityConfig();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	// Gets the number of affinity modifiers.
	int getNumAffinityModifiers();

	// Gets wheter an affinity modifier upgrades.
	bool AffinityUpgrades(AffinityModifier affinityModifier);

	// Gets the base upgrade time of an affinity modifier.
	uint32_t GetUpgradeTime(AffinityModifier affinityModifier);

	// Gets whether an affinity modifier decays.
	bool AffinityDecays(AffinityModifier affinityModifier);

	// Gets the base decay time of an affinity modifier.
	uint32_t GetDecayTime(AffinityModifier affinityModifier);

	// Gets whether an affinity prevents wars.
	bool AffinityPreventsWars(AffinityModifier affinityModifier);

	// Gets the base affinity gain of an affinity modifier.
	int GetAffinityGain(AffinityModifier affinityModifier);

	// Gets the affinity priority in case of mutual exclusive affinities.
	int GetAffinityPriority(AffinityModifier affinityModifier);

	// Gets whether an affinity modifier is mutually exclusive with other "stable relations" affinity modifiers.
	bool StableRelationsMutuallyExclusive(AffinityModifier affinityModifier);

	// Gets whether an affinity modifier is mutually exclusive with other "war together" affinity modifiers.
	bool WarTogetherMutuallyExclusive(AffinityModifier affinityModifier);


	// Stores the number of affinity modifiers that exist.
	int numAffinityModifiers;

	// Stores whether an affinity modifier upgrades or not.
	eastl::vector<bool> upgrades;

	// Stores the base upgrade time of an affinity modifier.
	eastl::vector<uint32_t> upgradeTime;

	// Stores whether an affinity modifier decays or not.
	eastl::vector<bool> decays;

	// Stores the base decay time of an affinity modifier.
	eastl::vector<uint32_t> decayTime;

	// Stores whether an affinity modifier prevents wars or not.
	eastl::vector<bool> preventWars;

	// Stores the base affinity gain of an affinity modifier.
	eastl::vector<int> affinityGain;

	// Stores the affinity priority in case of mutual exclusive affinities.
	eastl::vector<int> priority;

	// Stores whether an affinity modifier is mutually exclusive with other "stable relations" affinity modifiers.
	eastl::vector<bool> stableRelationsMutuallyExclusive;

	// Stores whether an affinity modifier is mutually exclusive with other "war together" affinity modifiers.
	eastl::vector<bool> warTogetherMutuallyExclusive;
};
