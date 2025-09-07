#pragma once

#include <Spore\BasicIncludes.h>
#include "cWarfareConfig.h"
#include "cWarfareStrengthAnalyzer.h"

#define cEmpireWarfarePtr intrusive_ptr<cEmpireWarfare>

class cEmpireWarfare 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cEmpireWarfare");
	
	cEmpireWarfare(Simulator::cEmpire* empire, cWarfareConfig* warfareConfig, cWarfareStrengthAnalyzer* warfareStrengthAnalyzer);
	~cEmpireWarfare();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	// Returns the closest enemy-controlled star to the given star within the empire's range.
	// @param star.
	// @return Pointer to the closest enemy star, or nullptr if none found within range.
	Simulator::cStarRecord* GetClosestEnemyStar(Simulator::cStarRecord* star);

	/// @brief Calculates and fills the attackPriorityMap with all enemy stars within the empire's range.
	void CalculateAttackPriorities();

	/// @brief Determines which enemy stars to attack based on calculated priorities and available forces,
	/// then executes the attacks on the selected stars.
	void SelectAndAttackTargets();

	void DebugAttackStar(Simulator::cStarRecord* star);




	// Pointer to the empire this object is managing.
	cEmpirePtr empire;

	// Pointer to the loaded warfareConfig.
	cWarfareConfigPtr warfareConfig;

	// Pointer to the loaded warfareStrengthAnalyzer.
	cWarfareStrengthAnalyzerPtr warfareStrengthAnalyzer;

	// Agression range of the empire.
	float range;

	// Maps each star to its calculated priority for being targeted in an invasion.
	eastl::map<cStarRecordPtr, float> attackPriorityMap;



};
