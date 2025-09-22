#pragma once

#include <Spore\BasicIncludes.h>
#include "cDiplomacyConfig.h"
#include "cArchetypesConfig.h"

#define cEmpireRelationsAnalyzerPtr intrusive_ptr<cEmpireRelationsAnalyzer>

/// Defines the different modifiers that affect the affinity between empires.
enum class AffinityModifier {
	ArchetypeAffinity,
	CommonEnemy,
	CommonAlly,
	WarWithAlly
};

// Responsible for evaluating core bilateral diplomatic metrics between empires,
// including affinity, aggressiveness, and diplomatic range.
class cEmpireRelationsAnalyzer 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cEmpireRelationsAnalyzer");
	
	cEmpireRelationsAnalyzer(cDiplomacyConfig* diplomacyConfig, cArchetypesConfig* archetyipesConfig);
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

	/// @brief Calculates the total affinity score between two empires.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire.
	/// @return An integer representing the diplomatic affinity between the two empires, based on shared enemies, archetypes, and past events.
	int EmpiresAffinity(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Retrieves the list of affinity modifiers that influence the affinity between two empires.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire.
	/// @return A vector of pairs, where each pair contains an AffinityModifier type and its corresponding value.
	eastl::vector<pair<AffinityModifier, int>> GetEmpiresAffinityModifier(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);



	// Pointer to the loaded diplomacy configuration object.
	cDiplomacyConfigPtr diplomacyConfig;

	// Pointer to the loaded archetypes affinities object.
	cArchetypesConfigPtr archetypesConfig;
};
