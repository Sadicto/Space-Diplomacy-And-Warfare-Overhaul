#pragma once

#include <Spore\BasicIncludes.h>
#include "cWarfareConfig.h"
#include "cSpaceCombatMetrics.h"


#define cWarfareStrengthAnalyzerPtr intrusive_ptr<cWarfareStrengthAnalyzer>

// Calculates the number of ships an empire can deploy and the force required
// to invade a planet or system.
class cWarfareStrengthAnalyzer 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cWarfareStrengthAnalyzer");
	
	cWarfareStrengthAnalyzer(cWarfareConfig* warfareConfig, cSpaceCombatMetrics* spaceCombatMetrics);
	~cWarfareStrengthAnalyzer();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Calculates the number of bombers required for an empire to conquer a specific planet.
	/// @param empire Pointer to the empire attempting the conquest.
	/// @param planet Pointer to the target planet.
	/// @return Number of bombers needed to successfully invade the planet.
	int GetBomberForceForPlanet(Simulator::cEmpire* empire, Simulator::cPlanetRecord* planet);

	/// @brief Calculates the number of bombers required for an empire to conquer a specific system.
	/// @param empire Pointer to the empire attempting the conquest.
	/// @param system Pointer to the target star system.
	/// @return Number of bombers needed to successfully invade the system.
	int GetBomberForceForSystem(Simulator::cEmpire* empire, Simulator::cStarRecord* star);

	/// @brief Returns the number of bombers produced by a factory based on its spice type.
	/// @param spiceKey ResourceKey identifying the spice type of the planet.
	/// @return Number of bombers produced per factory with the given spice.
	float GetBombersProducedByFactory(ResourceKey spiceKey);

	/// @brief Returns the number of factories on a given planet.
	/// The factory count is determined by the planet's terra-score.
	/// @param planet.
	/// @return Number of factories on the planet.
	int GetFactoryCountInPlanet(Simulator::cPlanetRecord* planet);

	/// @brief Returns the total number of bombers produced by all factories on a planet.
	/// @param planet Pointer to the planet record.
	/// @return Number of bombers produced by the specified planet.
	float GetBombersProducedByPlanet(Simulator::cPlanetRecord* planet);

	/// @brief Returns the total number of bombers produced by all planets in a system.
	/// @param star Pointer to the star record (system).
	/// @return Number of bombers produced by the specified system.
	float GetBombersProducedBySystem(Simulator::cStarRecord* star);

	/// @brief Returns the number of bombers an empire can produce in a cycle.
	/// @param empire Pointer to the empire.
	/// @return Number of bombers..
	float GetBombersProducedByEmpire(Simulator::cEmpire* empire);

	/// @brief Calculates the empire's strength factor based on its bombers and level.
	/// @param empire Pointer to the empire.
	/// @return Strength factor.
	float GetEmpireStrenghtFactor(Simulator::cEmpire* empire);

	// Pointer to the loaded warfareConfig.
	cWarfareConfigPtr warfareConfig;

	// Pointer to the loaded spaceCombatMetrics.
	cSpaceCombatMetricsPtr spaceCombatMetrics;

	// spiceCosts map used to calculate factory output.
	eastl::map<ResourceKey, float> spiceCosts;
};
