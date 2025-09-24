#include "stdafx.h"
#include "cEmpireWarfare.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include <algorithm>
#include <EASTL/sort.h>

using namespace SporeModUtils;
using namespace Simulator;

cEmpireWarfare::cEmpireWarfare(Simulator::cEmpire* empire, 
	cWarfareConfig* warfareConfig, 
	cWarfareStrengthAnalyzer* warfareStrengthAnalyzer, 
	cWarfareEventDispatcher* warfareEventDispatcher)
{
	this->empire = empire;
	this->range = 0;
	this->warfareConfig = warfareConfig;
	this->warfareStrengthAnalyzer = warfareStrengthAnalyzer;
	this->warfareEventDispatcher = warfareEventDispatcher;
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

bool cEmpireWarfare::AtackableStar(Simulator::cStarRecord* star) {
	for (cPlanetRecordPtr planet : star->GetPlanetRecords()) {
		if (MissionManager.ThereIsEventInPlanet(planet.get())) {
			return false;
		}
	}
	return true;
}
/*
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
		if (EmpireUtils::ValidNpcEmpire(starEmpire, true) && DiplomacyUtils::War(empire.get(), starEmpire) && AtackableStar(closeStar.get())) {
			float distance = StarUtils::GetDistanceBetweenStars(star, closeStar.get());
			if (distance < closestDistance) {
				closestStar = closeStar.get();
				closestDistance = distance;
			}
		}
	}
	return closestStar;
}
*/
eastl::vector<cStarRecord*> cEmpireWarfare::GetEnemyStarsInRangeOfStar(Simulator::cStarRecord* star) {
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

	// Store pairs of (distance, star).
	eastl::vector<eastl::pair<float, Simulator::cStarRecord*>> enemyStars;

	for (cStarRecordPtr closeStar : closeStars) {
		cEmpire* starEmpire = StarManager.GetEmpire(closeStar->mEmpireID);
		if (EmpireUtils::ValidNpcEmpire(starEmpire, true)
			&& DiplomacyUtils::War(empire.get(), starEmpire)
			&& AtackableStar(closeStar.get()))
		{
			float distance = StarUtils::GetDistanceBetweenStars(star, closeStar.get());
			enemyStars.push_back({ distance, closeStar.get() });
		}
	}

	// Sort enemy stars by distance.
	eastl::sort(enemyStars.begin(), enemyStars.end(),
		[](const auto& a, const auto& b) {
			return a.first < b.first;
		});

	// Extract only the star pointers in sorted order.
	eastl::vector<Simulator::cStarRecord*> orderedStars;
	orderedStars.reserve(enemyStars.size());
	for (auto& pair : enemyStars) {
		orderedStars.push_back(pair.second);
	}

	return orderedStars;

}

void cEmpireWarfare::CalculateAttackPriorities() {
	cStarRecord* homeworld = EmpireUtils::GetHomeStar(empire.get());

	for (cStarRecordPtr star : empire->mStars) {
		if (star != nullptr) {
			eastl::vector<cStarRecord*> enemyStarsInRangeOfStar = GetEnemyStarsInRangeOfStar(star.get());
			if (!enemyStarsInRangeOfStar.empty()) {
				// Handle closest star.
				cStarRecord* closestStar = enemyStarsInRangeOfStar.front();
				attackPriorityMap[closestStar] += 100.0f / StarUtils::GetDistanceBetweenStars(homeworld, closestStar);

				// Handle the rest.
				for (auto it = enemyStarsInRangeOfStar.begin() + 1; it != enemyStarsInRangeOfStar.end(); ++it) {
					cStarRecord* closeStar = *it;
					attackPriorityMap[closeStar] += 1.0f / StarUtils::GetDistanceBetweenStars(homeworld, closeStar);
				}
			}
		}
	}
}


void cEmpireWarfare::AttackStar(Simulator::cStarRecord* star, int bombers) {
	eastl::vector<pair<cPlanetRecordPtr, int>> bombersPerPlanet;
	int totalRequiredBombers = 0;
	for (cPlanetRecordPtr planet : star->GetPlanetRecords()) {
		if (PlanetUtils::InteractablePlanet(planet.get()) && planet->GetTechLevel() == TechLevel::Empire && !MissionManager.ThereIsEventInPlanet(planet.get())) {
			int planetRequiredBombers = warfareStrengthAnalyzer->GetBomberForceForPlanet(empire.get(), planet.get());
			totalRequiredBombers += planetRequiredBombers;
			bombersPerPlanet.push_back(make_pair(planet, planetRequiredBombers));
		}
	}
	if (bombersPerPlanet.size() > 0) {
		int extraBombersPerPlanet = (bombers - totalRequiredBombers) / bombersPerPlanet.size();
		int surplusBombers = (bombers - totalRequiredBombers) % bombersPerPlanet.size();
		for (auto& bomberForPlanet : bombersPerPlanet) {
			bomberForPlanet.second += extraBombersPerPlanet;
			if (surplusBombers > 0) {
				bomberForPlanet.second++;
				surplusBombers--;
			}
			warfareEventDispatcher->DispatchPlanetAttackedEvent(empire.get(), bomberForPlanet.first.get(), bomberForPlanet.second);
		}
	}
}

void cEmpireWarfare::SelectAndAttackTargets() {
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
				AttackStar(bestTarget.get(), static_cast<int>(round(bombersToAttackTarget)));
			}
			currentBombers = 0;
		}
		else {
			// If there´s no more targets attack with all available bombers.
			if (attackPriorityMap.empty()) {
				AttackStar(bestTarget.get(), static_cast<int>(round(max(1.0f, currentBombers))));
				currentBombers = 0;
			}
			else {
				AttackStar(bestTarget.get(), static_cast<int>(round(bombersToAttackTarget)));
				currentBombers -= bombersToAttackTarget;
			}
			
		}
	}
}
