#include "stdafx.h"
#include "cWarfareStrengthAnalyzer.h"

#include <Spore-Mod-Utils/Include/SporeModUtils.h>

using namespace SporeModUtils;
using namespace Simulator;

cWarfareStrengthAnalyzer::cWarfareStrengthAnalyzer(cWarfareConfig* warfareConfig, cSpaceCombatMetrics* spaceCombatMetrics, cArchetypeStrengthConfig* archetypeStrengthConfig)
{
    this->warfareConfig = warfareConfig;
	this->spaceCombatMetrics = spaceCombatMetrics;
    this->archetypeStrengthConfig = archetypeStrengthConfig;
    SpiceUtils::GetSpawnableSpiceBaseCosts(spiceCosts);
}


cWarfareStrengthAnalyzer::~cWarfareStrengthAnalyzer()
{
}

// For internal use, do not modify.
int cWarfareStrengthAnalyzer::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cWarfareStrengthAnalyzer::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cWarfareStrengthAnalyzer::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cWarfareStrengthAnalyzer);
	return nullptr;
}

int cWarfareStrengthAnalyzer::GetBomberForceForPlanet(Simulator::cEmpire* empire, Simulator::cPlanetRecord* planet){
    if (planet->GetTechLevel() != TechLevel::Empire) {
        return 0;
    }
    cEmpire* targetEmpire = StarManager.GetEmpire(planet->GetStarRecord()->mEmpireID);
    float empireBomberHealth = spaceCombatMetrics->GetBomberHealthByEmpire(empire);
    float empireFighterHealth = spaceCombatMetrics->GetFighterHealthByEmpire(empire);
    int empireFightersPerBomber = spaceCombatMetrics->GetFightersPerBomber();
    float targetTurretHealth = spaceCombatMetrics->GetTurretHealthByEmpire(targetEmpire);
    float targetDefenderHealth = spaceCombatMetrics->GetDefenderHealthByEmpire(targetEmpire);
    int planetDefenders = spaceCombatMetrics->GetDefendersInPlanet(planet);
    int planetTurrets = spaceCombatMetrics->GetTurretsInPlanet(planet);

    float fightersToDefeatDefenders = max((planetDefenders * targetDefenderHealth) / empireFighterHealth, static_cast<float>(empireFightersPerBomber));
    float bombersToDefeatTurrets = max((planetTurrets * targetTurretHealth) / (empireBomberHealth + empireFightersPerBomber * empireFighterHealth), 1.0f);

    int requiredBombers = static_cast<int>(round(bombersToDefeatTurrets + fightersToDefeatDefenders / empireFightersPerBomber));
    return requiredBombers;
}

int cWarfareStrengthAnalyzer::GetBomberForceForSystem(Simulator::cEmpire* empire, Simulator::cStarRecord* star){
    int requiredBombers = 0;
    for (cPlanetRecordPtr planet : star->GetPlanetRecords()) {
        if (planet->GetTechLevel() == TechLevel::Empire) {
            requiredBombers += GetBomberForceForPlanet(empire, planet.get());
        }
    }
    return requiredBombers;
}

float cWarfareStrengthAnalyzer::GetBombersProducedByFactory(ResourceKey spiceKey){
    float cheapestSpiceCost = SpiceUtils::GetSpiceBaseCost(SpiceUtils::GetCheapestSpice(spiceCosts), spiceCosts);
    float spiceCost = SpiceUtils::GetSpiceBaseCost(spiceKey, spiceCosts);
    // Defaults to the cheapest spice if the given spice is not valid.
    if (spiceCost == 0) {
        spiceCost = cheapestSpiceCost;
    }
    float spiceCostRatio = spiceCost / cheapestSpiceCost;
    return warfareConfig->GetBombersPerRedSpiceFactory() * spiceCostRatio;
}

int cWarfareStrengthAnalyzer::GetFactoryCountInPlanet(Simulator::cPlanetRecord* planet){
    switch (planet->mType) {
    case(PlanetType::T1): {
        return warfareConfig->GetBuildingsInT1Colony() / warfareConfig->GetBuildingsPerFactoryRatio();
    }
    case(PlanetType::T2): {
        return warfareConfig->GetBuildingsInT2Colony() * 2 / warfareConfig->GetBuildingsPerFactoryRatio();
    }
    case(PlanetType::T3): {
        if (PlanetUtils::Homeworld(planet)) {
            return warfareConfig->GetBuildingsInT3Colony() * warfareConfig->GetCitiesInHomeworld() / warfareConfig->GetBuildingsPerFactoryRatio();
        }
        else {
            return warfareConfig->GetBuildingsInT3Colony() * 3 / warfareConfig->GetBuildingsPerFactoryRatio();
        }
    }
    default: {
        return 0;
    }
    }
}

float cWarfareStrengthAnalyzer::GetBombersProducedByPlanet(Simulator::cPlanetRecord* planet){
    if (planet->GetTechLevel() == TechLevel::Empire) {
        return GetFactoryCountInPlanet(planet) * GetBombersProducedByFactory(planet->mSpiceGen);
    }
    else {
        return 0.0f;
    }
}

float cWarfareStrengthAnalyzer::GetBombersProducedBySystem(Simulator::cStarRecord* star){
    float bombersProduced = 0;
    for (cPlanetRecordPtr planet : star->GetPlanetRecords()) {
        if (PlanetUtils::InteractablePlanet(planet.get()) && planet->GetTechLevel() == TechLevel::Empire) {
            bombersProduced += GetBombersProducedByPlanet(planet.get());
        }
    }
    return bombersProduced;
}


float cWarfareStrengthAnalyzer::GetBombersProducedByEmpire(Simulator::cEmpire* empire){
    float bombersProduced = 0;
    for (cStarRecordPtr star : empire->mStars) {
        if (StarUtils::ValidStar(star.get())) {
            bombersProduced += GetBombersProducedBySystem(star.get());
        }
    }
    float baseStrength = archetypeStrengthConfig->GetArchetypeBaseStrength(empire->mArchetype);
    float bonusStrength = archetypeStrengthConfig->GetArchetypeBonusStrength(empire->mArchetype);
    return (bombersProduced + bonusStrength) * baseStrength ;
}

float cWarfareStrengthAnalyzer::GetEmpireStrenghtFactor(Simulator::cEmpire* empire){
    float bombersProducedByEmpire = GetBombersProducedByEmpire(empire);
    float empireBomberHealth = spaceCombatMetrics->GetBomberHealthByEmpire(empire);
    float MaxLevelBomberHealth = spaceCombatMetrics->GetBomberHealthByLevel(4);
    float bomberHealthRatio = MaxLevelBomberHealth / empireBomberHealth;
    return bombersProducedByEmpire * bomberHealthRatio;
}

