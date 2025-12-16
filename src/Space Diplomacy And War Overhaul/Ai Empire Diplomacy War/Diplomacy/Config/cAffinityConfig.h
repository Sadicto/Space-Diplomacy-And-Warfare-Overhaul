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
	UpliftedByMonolith = 5
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

	// Gets whether an affinity modifier expires.
	bool AffinityExpires(AffinityModifier affinityModifier);

	// Gets the expire time of an affinity modifier.
	uint32_t GetExpireTime(AffinityModifier affinityModifier);

	// Gets the affinity gain of an affinity modifier.
	int GetAffinityGain(AffinityModifier affinityModifier);

	// Gets whether an affinity modifier is mutually exclusive with another modifier.
	bool MutuallyExclusive(AffinityModifier affinityModifier);

	// Stores the number of affinity modifiers that exist.
	int numAffinityModifiers;

	// Stores whether an affinity modifier expires or not.
	eastl::vector<bool> expires;

	// Stores the expire time of an affinity modifier.
	eastl::vector<uint32_t> expireTime;

	// Stores the affinity gain of an affinity modifier.
	eastl::vector<int> affinityGain;

	// Stores whether an affinity modifier is mutually exclusive with another modifier.
	eastl::vector<bool> mutuallyExclusive;
};
