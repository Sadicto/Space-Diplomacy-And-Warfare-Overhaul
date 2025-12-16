#include "stdafx.h"
#include "cEmpireRelationsAnalyzer.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace Simulator;
using namespace SporeModUtils;

cEmpireRelationsAnalyzer::cEmpireRelationsAnalyzer(cDiplomacyConfig* diplomacyConfig, cArchetypesConfig* archetyipesAffinities, cAffinityConfig* affinityConfig)
{
	this->diplomacyConfig = diplomacyConfig;
	this->archetypesConfig = archetyipesAffinities;
	this->affinityConfig = affinityConfig;

	// TODO: Swap this out for something that isn’t eye-breaking.
	for (int i = 0; i < affinityConfig->getNumAffinityModifiers(); i++) {
		affinityModifiers.insert(AffinityModifier(i));
	}
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
	int affinity = 0;
	int maxMutuallyExclusiveAffinity = 0;
	for (AffinityModifier modifier : affinityModifiers) {
		int affinityForModifier = 0;

		// This should be a new class.
		switch (modifier) {

		case(AffinityModifier::ArchetypeAffinity): {
			affinityForModifier = archetypesConfig->GetArchetypesAffinity(empire1->mArchetype, empire2->mArchetype);
			break;
		}
		case(AffinityModifier::CommonEnemy): {
			if (DiplomacyUtils::CommonEnemy(empire1, empire2)) {
				affinityForModifier = affinityConfig->GetAffinityGain(modifier);
			}
			break;
		}
		case(AffinityModifier::CommonAlly): {
			if (DiplomacyUtils::AllianceWithAllyOfEmpire(empire1, empire2)) {
				affinityForModifier = affinityConfig->GetAffinityGain(modifier);
			}
			break;
		}
		case(AffinityModifier::WarWithAlly): {
			if (DiplomacyUtils::AllianceWithEnemyOfEmpire(empire1, empire2) || DiplomacyUtils::AllianceWithEnemyOfEmpire(empire2, empire1)) {
				affinityForModifier = affinityConfig->GetAffinityGain(modifier);
			}
			break;
		}
		case(AffinityModifier::DefeatedCommonEnemy): {
			if (false) {
				affinityForModifier = affinityConfig->GetAffinityGain(modifier);
			}
			break;
		}
		case(AffinityModifier::UpliftedByMonolith): {
			if (false) {
				affinityForModifier = affinityConfig->GetAffinityGain(modifier);
			}
			break;
		}
		}
		if (affinityConfig->MutuallyExclusive(modifier)) {
			if (affinityForModifier > maxMutuallyExclusiveAffinity) {
				maxMutuallyExclusiveAffinity = affinityForModifier;
			}
		}
		else {
			affinity += affinityForModifier;
		}
	}
	return affinity + maxMutuallyExclusiveAffinity;
}

eastl::vector<pair<AffinityModifier, int>> cEmpireRelationsAnalyzer::GetEmpiresAffinityModifiers(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {

	eastl::vector<pair<AffinityModifier, int>> empiresAffinityModifiers;

	int maxMutuallyExclusiveAffinity = 0;
	AffinityModifier maxMutuallyExclusiveModifier = AffinityModifier(0);
	for (AffinityModifier modifier : affinityModifiers) {
		int affinityForModifier = 0;

		// This definitely should be a new class.
		switch (modifier) {

		case(AffinityModifier::ArchetypeAffinity): {
			affinityForModifier = archetypesConfig->GetArchetypesAffinity(empire1->mArchetype, empire2->mArchetype);
			break;
		}
		case(AffinityModifier::CommonEnemy): {
			if (DiplomacyUtils::CommonEnemy(empire1, empire2)) {
				affinityForModifier = affinityConfig->GetAffinityGain(modifier);
			}
			break;
		}
		case(AffinityModifier::CommonAlly): {
			if (DiplomacyUtils::AllianceWithAllyOfEmpire(empire1, empire2)) {
				affinityForModifier = affinityConfig->GetAffinityGain(modifier);
			}
			break;
		}
		case(AffinityModifier::WarWithAlly): {
			if (DiplomacyUtils::AllianceWithEnemyOfEmpire(empire1, empire2) || DiplomacyUtils::AllianceWithEnemyOfEmpire(empire2, empire1)) {
				affinityForModifier = affinityConfig->GetAffinityGain(modifier);
			}
			break;
		}
		case(AffinityModifier::DefeatedCommonEnemy): {
			if (false) {
				affinityForModifier = affinityConfig->GetAffinityGain(modifier);
			}
			break;
		}
		case(AffinityModifier::UpliftedByMonolith): {
			if (false) {
				affinityForModifier = affinityConfig->GetAffinityGain(modifier);
			}
			break;
		}
		}
		if (affinityConfig->MutuallyExclusive(modifier)) {
			if (affinityForModifier > maxMutuallyExclusiveAffinity) {
				maxMutuallyExclusiveAffinity = affinityForModifier;
				maxMutuallyExclusiveModifier = modifier;
			}
		}
		else if (affinityForModifier != 0) {
			empiresAffinityModifiers.emplace_back(modifier, affinityForModifier);
		}
	}
	if (maxMutuallyExclusiveAffinity > 0) {
		empiresAffinityModifiers.emplace_back(maxMutuallyExclusiveModifier, maxMutuallyExclusiveAffinity);
	}
	return empiresAffinityModifiers;
}
