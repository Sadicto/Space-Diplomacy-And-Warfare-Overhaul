#include "stdafx.h"
#include "cEmpireRelationsAnalyzer.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"
using namespace Simulator;
using namespace SporeModUtils;

cEmpireRelationsAnalyzer::cEmpireRelationsAnalyzer(cDiplomacyConfig* diplomacyConfig, cArchetypesAffinities* archetyipesAffinities)
{
	this->diplomacyConfig = diplomacyConfig;
	this->archetypesAffinities = archetyipesAffinities;
}


cEmpireRelationsAnalyzer::~cEmpireRelationsAnalyzer()
{
}

// For internal use, do not modify.
int cEmpireRelationsAnalyzer::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cEmpireRelationsAnalyzer::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cEmpireRelationsAnalyzer::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cEmpireRelationsAnalyzer);
	return nullptr;
}

float cEmpireRelationsAnalyzer::GetEmpireDiplomaticRange(cEmpire* empire) {
	int empireLevel = EmpireUtils::GetEmpireLevel(empire);
	return diplomacyConfig->GetDiplomacyRange(empireLevel);
}

int cEmpireRelationsAnalyzer::GetEmpireAgressivity(cEmpire* empire) {
	Archetypes archetype = ArchetypeUtils::GetBaseArchetype(empire->mArchetype);
	int agressivty = diplomacyConfig->GetArchetypeAgressivity(empire->mArchetype);
	agressivty += diplomacyConfig->GetAggressivityGrowthByPowerLevel(EmpireUtils::GetEmpireLevel(empire));
	return agressivty;
}

int cEmpireRelationsAnalyzer::EmpiresAffinity(cEmpire* empire1, cEmpire* empire2) {
	int affinity = archetypesAffinities->GetArchetypesAffinity(empire1->mArchetype, empire2->mArchetype);
	if (DiplomacyUtils::AllianceWithAllyOfEmpire(empire1, empire2)) {
		affinity += diplomacyConfig->GetAffinityGainForAllyOfAlly();
	}
	if (DiplomacyUtils::CommonEnemy(empire1, empire2)) {
		affinity += diplomacyConfig->GetAffinityGainForEnemyOfEnemy();
	}
	if (DiplomacyUtils::AllianceWithEnemyOfEmpire(empire1, empire2) || DiplomacyUtils::AllianceWithEnemyOfEmpire(empire2, empire1)) {
		affinity += diplomacyConfig->GetAffinityGainForEnemyOfAlly();
	}
	return affinity;
}
