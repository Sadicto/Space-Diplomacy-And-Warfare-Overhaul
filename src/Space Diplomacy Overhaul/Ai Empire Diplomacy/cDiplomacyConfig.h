#pragma once

#include <Spore\BasicIncludes.h>

#define cDiplomacyConfigPtr intrusive_ptr<cDiplomacyConfig>

class cDiplomacyConfig 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cDiplomacyConfig");
	
	cDiplomacyConfig(ResourceKey configKey);
	~cDiplomacyConfig();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	// Diplomacy range per empire level, in parsecs.
	eastl::vector<float> diplomacyRange;

	// The upper limit beyond which increased affinity has no additional effect.
	int maxAffinitySoftCap;

	// The lower limit below which decreased affinity has no additional effect.
	int minAffinitySoftCap;

	// The maximum probability of alliance between two empires during a single cycle.
	float maxAllianceProbability;

	// The maximum probability of war between two empires during a single cycle.
	float maxWarProbability;

	// The affinity value required for an alliance to be considered stable.
	int affinityThresholdForStableAlliance;

	// The affinity value in which an alliance is considered unstable.
	int affinityThresholdForUnstableAlliance;

	// The maximum affinity at which empires may still declare war.
	int affinityThresholdForWar;

	// Stores the base aggressiveness for each archetype.
	eastl::vector<int> baseAggressivityByArchetype;

	// Extra aggressiveness gained per power level of an empire.
	eastl::vector<int> aggressivityGrowthByPowerLevel;

	// If true, AI empires automatically declare war on enemies of their allies.
	bool autoDeclareWarOnAllyEnemies;

	// If true, AI empires may start new wars while already at war against non-enemies of allies.
	bool startsWarsWhileAtWar;

	// If (power of target + allies) / (power of empire + allies) > powerThresholdToAvoidWar, then the empire will avoid declaring war.
	float powerThresholdToAvoidWar;

};
