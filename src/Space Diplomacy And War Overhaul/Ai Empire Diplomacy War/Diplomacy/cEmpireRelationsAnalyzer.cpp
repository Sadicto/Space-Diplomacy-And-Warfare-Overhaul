#include "stdafx.h"
#include "cEmpireRelationsAnalyzer.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace Simulator;
using namespace SporeModUtils;

cEmpireRelationsAnalyzer::cEmpireRelationsAnalyzer(cDiplomacyConfig* diplomacyConfig, cArchetypesConfig* archetyipesAffinities)
{
	this->diplomacyConfig = diplomacyConfig;
	this->archetypesConfig = archetyipesAffinities;
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
	return archetypesConfig->GetArchetypeAgressivtyByPowerLevel(empire->mArchetype, EmpireUtils::GetEmpireLevel(empire));
}

int cEmpireRelationsAnalyzer::EmpiresAffinity(cEmpire* empire1, cEmpire* empire2) {
	int affinity = archetypesConfig->GetArchetypesAffinity(empire1->mArchetype, empire2->mArchetype);
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

eastl::vector<pair<AffinityModifier, int>> cEmpireRelationsAnalyzer::GetEmpiresAffinityModifier(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	eastl::vector<pair<AffinityModifier, int>> affinityModifiers;

	int archetypesAffinity = archetypesConfig->GetArchetypesAffinity(empire1->mArchetype, empire2->mArchetype);
	affinityModifiers.emplace_back(AffinityModifier::ArchetypeAffinity, archetypesAffinity);

	int commonAllyAffinity = DiplomacyUtils::AllianceWithAllyOfEmpire(empire1, empire2)
		? diplomacyConfig->GetAffinityGainForAllyOfAlly()
		: 0;
	affinityModifiers.emplace_back(AffinityModifier::CommonAlly, commonAllyAffinity);

	int commonEnemyAffinity = DiplomacyUtils::CommonEnemy(empire1, empire2)
		? diplomacyConfig->GetAffinityGainForEnemyOfEnemy()
		: 0;
	affinityModifiers.emplace_back(AffinityModifier::CommonEnemy, commonEnemyAffinity);

	int warWithAllyAffinity = (DiplomacyUtils::AllianceWithEnemyOfEmpire(empire1, empire2) || DiplomacyUtils::AllianceWithEnemyOfEmpire(empire2, empire1))
		? diplomacyConfig->GetAffinityGainForEnemyOfAlly()
		: 0;
	affinityModifiers.emplace_back(AffinityModifier::WarWithAlly, warWithAllyAffinity);
	return affinityModifiers;
}
