#pragma once

#include <Spore\BasicIncludes.h>
#include "Config\cDiplomacyConfig.h"
#include "Config\cArchetypesConfig.h"
#include "Config\cAffinityConfig.h"
#include "PersistedEvent\cPersistedDiplomacyEventManager.h"

#define cEmpireRelationsAnalyzerPtr intrusive_ptr<cEmpireRelationsAnalyzer>

// Responsible for evaluating core bilateral diplomatic metrics between empires,
// including affinity, aggressiveness, and diplomatic range.
class cEmpireRelationsAnalyzer 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cEmpireRelationsAnalyzer");
	
	cEmpireRelationsAnalyzer(cDiplomacyConfig* diplomacyConfig, 
		cArchetypesConfig* archetyipesConfig, 
		cAffinityConfig* affinityConfig, 
		cPersistedDiplomacyEventManager* persistedDiplomacyEventManager);
	~cEmpireRelationsAnalyzer();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Returns the maximum diplomatic interaction range for the specified empire.
	/// @param empire.
	/// @return A float representing the distance (in pc) within which the empire can engage in diplomacy.
	float GetEmpireDiplomaticRange(Simulator::cEmpire* empire);

	/// @brief Calculates the base aggressiveness value for the specified empire.
	/// @param empire.
	/// @return An integer representing the empire's aggressiveness level.
	int GetEmpireAgressivity(Simulator::cEmpire* empire);
	
	/// @brief Checks the diplomacy events between two empires to determine whether one empire was uplifted by the other.
	/// @param diplomacyEvents Vector containing the diplomacy events between two empires.
	/// @return True if one empire was uplifted by the other, false otherwise.
	bool EmpiresUpliftedByMonolith(const eastl::vector<cPersistedDiplomacyEventPtr> diplomacyEvents);

	/// @brief Checks the diplomacy events between two empires to determine the affinity resulting from having defeated a common enemy.
	/// @param diplomacyEvents Vector containing the diplomacy events between two empires.
	/// @return The affinity between the two empires for having defeated a common enemy, or 0 if it does not apply.
	int GetEmpiresDefeatedCommonEnemyAffinity(const eastl::vector<cPersistedDiplomacyEventPtr> diplomacyEvents);


	/// @brief Calculates the total affinity score between two empires.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire.
	/// @return An integer representing the diplomatic affinity between the two empires, based on shared enemies, archetypes, and past events.
	int EmpiresAffinity(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Retrieves the list of affinity modifiers that influence the affinity between two empires.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire.
	/// @return A vector of pairs, where each pair contains an AffinityModifier type and its corresponding value.
	eastl::vector<pair<AffinityModifier, int>> GetEmpiresAffinityModifiers(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	// Pointer to the loaded diplomacy configuration object.
	cDiplomacyConfigPtr diplomacyConfig;

	// Pointer to the loaded archetypes affinities object.
	cArchetypesConfigPtr archetypesConfig;

	// Pointer to the loaded affinity config object.
	cAffinityConfigPtr affinityConfig;

	// Pointer to the loaded persisted diplomacy event manager.
	cPersistedDiplomacyEventManagerPtr persistedDiplomacyEventManager;

	// Set with all the affinityModifiers.
	eastl::set<AffinityModifier> affinityModifiers;
};
