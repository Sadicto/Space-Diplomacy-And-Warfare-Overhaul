#pragma once

#include <Spore\BasicIncludes.h>

#define cSpaceCombatMetricsPtr intrusive_ptr<cSpaceCombatMetrics>

// Stores unit and defense stats used in space combat and
// calculates those values given specific empires or planets.
class cSpaceCombatMetrics 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceWarfareOverhaul::cSpaceCombatMetrics");
	
	cSpaceCombatMetrics(ResourceKey spaceCombatKey);
	~cSpaceCombatMetrics();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Returns the health of a bomber ship for a given empire level.
	/// @param empireLevel.
	/// @return The health value of the bomber at the specified empire level.
	float GetBomberHealthByLevel(int empireLevel);

	/// @brief Returns the health of a fighter ship for a given empire level.
	/// @param empireLevel.
	/// @return The health value of the fighter at the specified empire level.
	float GetFighterHealthByLevel(int empireLevel);

	/// @brief Returns the health of a defender ship for a given empire level.
	/// @param empireLevel.
	/// @return The health value of the defender at the specified empire level.
	float GetDefenderHealthByLevel(int empireLevel);

	/// @brief Returns the health of a turret for a given empire level.
	/// @param empireLevel.
	/// @return The health value of the turret at the specified empire level.
	float GetTurretHealthByLevel(int empireLevel);

	/// @brief Returns the number of defender units in outposts.
	/// @return The number of defenders in an outpost.
	int GetDefendersInOutpost();

	/// @brief Returns the number of defender units in standard colonies.
	/// @return The number of defenders in a colony.
	int GetDefendersInColony();

	/// @brief Returns the number of fighter escorts assigned to each bomber.
	/// @return Number of fighters per bomber.
	int GetFightersPerBomber();

	/// @brief Returns the number of turrets in T1 planets.
	/// @return Number of turrets in T1 defenses.
	int GetTurretsInT1();

	/// @brief Returns the number of turrets in T2 planets.
	/// @return Number of turrets in T2 defenses.
	int GetTurretsInT2();

	/// @brief Returns the number of turrets in T3 planets.
	/// @return Number of turrets in T3 defenses.
	int GetTurretsInT3();

	/// @brief Returns the health of bomber ships for a given empire.
	/// @param empire..
	/// @return Health value of bombers for the specified empire.
	float GetBomberHealthByEmpire(Simulator::cEmpire* empire);

	/// @brief Returns the health of fighter ships for a given empire.
	/// @param empire.
	/// @return Health value of fighters for the specified empire.
	float GetFighterHealthByEmpire(Simulator::cEmpire* empire);

	/// @brief Returns the health of defender ships for a given empire.
	/// @param empire.
	/// @return Health value of defenders for the specified empire.
	float GetDefenderHealthByEmpire(Simulator::cEmpire* empire);

	/// @brief Returns the health of turrets for a given empire.
	/// @param empire.
	/// @return Health value of turrets for the specified empire.
	float GetTurretHealthByEmpire(Simulator::cEmpire* empire);

	/// @brief Returns the average number of defenders on a given planet.
	/// @param planet.
	/// @return Total number of defenders on the specified planet.
	int GetDefendersInPlanet(Simulator::cPlanetRecord* planet);

	/// @brief Returns the average of turrets on a given planet.
	/// @param planet.
	/// @return Number of turrets on the specified planet.
	int GetTurretsInPlanet(Simulator::cPlanetRecord* planet);



	// Health of bomber ships per empire level.
	eastl::vector<float> healthBomber;

	// Health of fighter ships per empire level.
	eastl::vector<float> healthFighter;

	// Health of defender ships per empire level.
	eastl::vector<float> healthDefender;

	// Health of turrets per empire level.
	eastl::vector<float> healthTurret;

	// Number of defender in outposts.
	int numDefendersInOutpost;

	// Number of defender units in standard colonies.
	int numDefendersInColony;

	// Number of fighter escorts assigned to each bomber.
	int numFightersPerBomber;

	// Number of turrets in T1 planets.
	int numTurretsInT1;

	// Number of turrets in T2 planets.
	int numTurretsInT2;

	// Number of turrets in T3 planets.
	int numTurretsInT3;

};
