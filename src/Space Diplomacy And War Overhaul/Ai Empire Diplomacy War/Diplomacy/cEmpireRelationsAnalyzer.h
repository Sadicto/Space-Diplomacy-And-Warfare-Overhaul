#pragma once

#include <Spore\BasicIncludes.h>
#include "Config\cDiplomacyConfig.h"
#include "Config\cArchetypesConfig.h"
#include "Config\cAffinityConfig.h"
#include "PersistedEvent\cPersistedDiplomacyEventManager.h"
#include "IAffinityModifier.h"

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
		cArchetypesConfig* archetypesConfig, 
		cAffinityConfig* affinityConfig, 
		cPersistedDiplomacyEventManager* persistedDiplomacyEventManager,
		eastl::vector<IAffinityModifierPtr> affinityModifiers);
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

	/// @brief Retrieves the list of the data from affinity modifiers that influence the relationship between two empires.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire.
	/// @param affinityData Output vector of AffinityModifierData containing the information for each affinity modifier.
	void GetEmpiresAffinityModifiersData(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, eastl::vector<AffinityModifierData>& affinityData);


	/// @brief Calculates the total affinity score between two empires.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire.
	/// @return An integer representing the diplomatic affinity between the two empires, based on shared enemies, archetypes, and past events.
	int EmpiresAffinity(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	// Pointer to the loaded diplomacy configuration object.
	cDiplomacyConfigPtr diplomacyConfig;

	// Pointer to the loaded archetypes affinities object.
	cArchetypesConfigPtr archetypesConfig;

	// Pointer to the loaded affinity config object.
	cAffinityConfigPtr affinityConfig;

	// Pointer to the loaded persisted diplomacy event manager.
	cPersistedDiplomacyEventManagerPtr persistedDiplomacyEventManager;

	// Pointers to the loaded affinityModifiers.
	eastl::vector<IAffinityModifierPtr> affinityModifiers;

	// Context used by the AffinityModifiers.
	AffinityModifierContext affinityModifierContext;
};
