#pragma once

#include <Spore\BasicIncludes.h>

#define cDiplomacyConfigPtr intrusive_ptr<cDiplomacyConfig>



// Stores all the global configuration data used by the diplomacy system.
// This includes constants and tuning parameters such as base aggressiveness 
// per archetype, thresholds for alliance and war decisions, and AI behavior flags.
class cDiplomacyConfig 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cDiplomacyConfig");
	
	cDiplomacyConfig(ResourceKey configKey);
	~cDiplomacyConfig();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Gets the diplomacy range for the given empire level, in parsecs.
	float GetDiplomacyRange(int level);

	/// @brief Gets the upper limit beyond which increased affinity has no additional effect.
	int GetMaxAffinitySoftCap();

	/// @brief Gets the lower limit below which decreased affinity has no additional effect.
	int GetMinAffinitySoftCap();

	/// @brief Gets the maximum probability of alliance between two empires during a single cycle.
	float GetMaxAllianceProbability();

	/// @brief Gets the maximum probability of war between two empires during a single cycle.
	float GetMaxWarProbability();

	/// @brief Gets the affinity value required for an alliance to be considered stable.
	int GetAffinityThresholdForStableAlliance();

	/// @brief Gets the affinity value at which an alliance is considered unstable.
	int GetAffinityThresholdForUnstableAlliance();

	/// @brief Gets the maximum affinity at which empires may still declare war.
	int GetAffinityThresholdForWar();

	/// @brief Gets the affinity gained between empires that are allies of the same ally.
	int GetAffinityGainForAllyOfAlly();

	/// @brief Gets the affinity gained between empires that share a common enemy.
	int GetAffinityGainForEnemyOfEnemy();

	/// @brief Gets the affinity lost when an empire is an enemy of another's ally (should be negative).
	int GetAffinityGainForEnemyOfAlly();

	/// @brief Gets the base aggressiveness value for the specified archetype.
	int GetArchetypeAgressivity(Simulator::Archetypes archetype);

	/// @brief Gets the extra aggressiveness gained per power level of an empire.
	int GetAggressivityGrowthByPowerLevel(int powerLevel);

	/// @brief Checks whether AI empires automatically declare war on enemies of their allies.
	bool GetAutoDeclareWarOnAllyEnemies();

	/// @brief Checks whether AI empires may start new wars while already at war against non-enemies of allies.
	bool GetStartsWarsWhileAtWar();

	/// @brief Gets the power ratio threshold above which an empire avoids declaring war.
	bool GetPowerThresholdToAvoidWar();


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

	// Affinity gained between empires that are allies of the same ally.
	int affinityGainForAllyOfAlly;

	// Affinity gained between empires that share a common enemy.
	int affinityGainForEnemyOfEnemy;

	// Affinity lost between empires when one is an enemy of the other's ally (should be negative).
	int affinityGainForEnemyOfAlly;

	// Stores the base aggressiveness for each archetype.
	eastl::vector<int> baseAggressivityByArchetype;

	// Extra aggressiveness gained per power level of an empire.
	eastl::vector<int> aggressivityGrowthByPowerLevel;

	eastl::vector<eastl::vector<int>> archetypeAgressivityByPowerLevel;

	// If true, AI empires automatically declare war on enemies of their allies.
	bool autoDeclareWarOnAllyEnemies;

	// If true, AI empires may start new wars while already at war against non-enemies of allies.
	bool startsWarsWhileAtWar;

	// If (power of target + allies) / (power of empire + allies) > powerThresholdToAvoidWar, then the empire will avoid declaring war.
	float powerThresholdToAvoidWar;

};
