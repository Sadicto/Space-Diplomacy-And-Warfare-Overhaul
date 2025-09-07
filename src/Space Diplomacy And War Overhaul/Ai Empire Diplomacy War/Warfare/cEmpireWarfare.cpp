#include "stdafx.h"
#include "cEmpireWarfare.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>

using namespace SporeModUtils;
using namespace Simulator;

cEmpireWarfare::cEmpireWarfare(Simulator::cEmpire* empire, cWarfareConfig* warfareConfig, cWarfareStrengthAnalyzer* warfareStrengthAnalyzer)
{
	this->empire = empire;
	this->range = 0;
	this->warfareConfig = warfareConfig;
	this->warfareStrengthAnalyzer = warfareStrengthAnalyzer;
}


cEmpireWarfare::~cEmpireWarfare()
{
}

// For internal use, do not modify.
int cEmpireWarfare::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cEmpireWarfare::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cEmpireWarfare::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cEmpireWarfare);
	return nullptr;
}

Simulator::cStarRecord* cEmpireWarfare::GetClosestEnemyStar(Simulator::cStarRecord* star) {
	eastl::vector<cStarRecordPtr> closeStars;

	StarRequestFilter filter;
	filter.RemoveStarType(Simulator::StarType::None);
	filter.RemoveStarType(Simulator::StarType::GalacticCore);
	filter.RemoveStarType(Simulator::StarType::ProtoPlanetary);
	filter.RemoveStarType(Simulator::StarType::BlackHole);
	filter.techLevels = 0;
	filter.AddTechLevel(Simulator::TechLevel::Empire);
	filter.minDistance = 0;
	filter.maxDistance = range;
	StarManager.FindStars(star->mPosition, filter, closeStars);

	Simulator::cStarRecord* closestStar = nullptr;
	float closestDistance = range + 1.0f;
	for (cStarRecordPtr closeStar : closeStars) {
		cEmpire* starEmpire = StarManager.GetEmpire(closeStar->mEmpireID);
		if (EmpireUtils::ValidNpcEmpire(starEmpire, true) && DiplomacyUtils::War(empire.get(), starEmpire)) {
			float distance = StarUtils::GetDistanceBetweenStars(star, closeStar.get());
			if (distance < closestDistance) {
				closestStar = closeStar.get();
				closestDistance = distance;
			}
		}
	}
	return closestStar;
}

void cEmpireWarfare::CalculateAttackPriorities() {
	cStarRecord* homeworld = EmpireUtils::GetHomeStar(empire.get());

	for (cStarRecordPtr star : empire->mStars) {
		if (star != nullptr) {
			cStarRecord* closestStar = GetClosestEnemyStar(star.get());
			if (closestStar != nullptr) {
				// Priority increases with repetition and decreases with distance from the homeworld.
				attackPriorityMap[closestStar] += 100.0f / StarUtils::GetDistanceBetweenStars(homeworld, closestStar);
			}

		}
	}
}

void cEmpireWarfare::SelectAndAttackTargets() {
	eastl::vector<cStarRecordPtr> debugVector;
	if (empire->mEnemies.size() == 0) {
		return;
	}
	range = warfareConfig->GetWarfareRange(EmpireUtils::GetEmpireLevel(empire.get()));
	CalculateAttackPriorities();

	float bombersPerCycle = warfareStrengthAnalyzer->GetBombersProducedByEmpire(empire.get());
	float currentBombers = bombersPerCycle;
	while (currentBombers > 0 && !attackPriorityMap.empty()) {
		auto bestIt = attackPriorityMap.end();
		float maxPriority = -1.0f;

		for (auto it = attackPriorityMap.begin(); it != attackPriorityMap.end(); ++it) {
			if (it->second > maxPriority) {
				maxPriority = it->second;
				bestIt = it;
			}
		}
		cStarRecordPtr bestTarget = bestIt->first;
		float bombersToAttackTarget = static_cast<float>(warfareStrengthAnalyzer->GetBomberForceForSystem(empire.get(), bestTarget.get()));
		attackPriorityMap.erase(bestIt);
		// If there are not enough bombers to fully overpower the target system, 
		// use a probability based on the ratio of available to required bombers 
		// to decide whether to attempt the attack this turn.
		if (bombersToAttackTarget > currentBombers) {
			float pOfAttack = currentBombers / bombersToAttackTarget;
			float n = Math::randf();
			if (pOfAttack > n) {
				// Attack with bombersToAttackTarget.
				currentBombers = 0;
				debugVector.push_back(bestTarget);
				DebugAttackStar(bestTarget.get());
			}
		}
		else {
			if (attackPriorityMap.empty()) {
				// Attack with all available bombers.
				debugVector.push_back(bestTarget);
				DebugAttackStar(bestTarget.get());
			}
			else {
				// Attack with bombersToAttackTarget.
				debugVector.push_back(bestTarget);
				DebugAttackStar(bestTarget.get());
			}
			currentBombers -= bombersToAttackTarget;
		}
	}
}

void cEmpireWarfare::DebugAttackStar(cStarRecord* star) {
	for (cPlanetRecordPtr planet : star->GetPlanetRecords()) {
		if (planet->GetTechLevel() == TechLevel::Empire && !MissionManager.ThereIsEventInPlanet(planet.get())) {
			uint32_t eventId = id("RaidWar");;
			uint32_t targetEmpireiD = planet->GetStarRecord()->mEmpireID;
			cEmpire* targetEmpire = StarManager.GetEmpire(targetEmpireiD);
			cMission* mission = MissionManager.CreateMission(eventId, planet.get(), targetEmpire);

			cRaidEvent* raidEvent = static_cast<cRaidEvent*>(mission);
			cPlanetPtr planetTargetPtr;
			StarManager.RecordToPlanet(planet.get(), planetTargetPtr);
			raidEvent->mAttackerEmpire = empire->GetEmpireID();
			raidEvent->mpTargetPlanet = planetTargetPtr;
			raidEvent->mNumBombers = warfareStrengthAnalyzer->GetBomberForceForPlanet(empire.get(), planet.get());
			raidEvent->AcceptMission();
		}
	}
}
