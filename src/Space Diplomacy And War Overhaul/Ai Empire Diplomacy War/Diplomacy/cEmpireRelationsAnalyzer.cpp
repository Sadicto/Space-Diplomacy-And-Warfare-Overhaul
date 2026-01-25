#include "stdafx.h"
#include "cEmpireRelationsAnalyzer.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace Simulator;
using namespace SporeModUtils;

cEmpireRelationsAnalyzer::cEmpireRelationsAnalyzer(cDiplomacyConfig* diplomacyConfig, 
	cArchetypesConfig* archetypesConfig, 
	cAffinityConfig* affinityConfig,
	cPersistedDiplomacyEventManager* persistedDiplomacyEventManager,
	eastl::vector<IAffinityModifierPtr> affinityModifiers)
{
	this->diplomacyConfig = diplomacyConfig;
	this->archetypesConfig = archetypesConfig;
	this->affinityConfig = affinityConfig;
	this->persistedDiplomacyEventManager = persistedDiplomacyEventManager;
	this->affinityModifiers = affinityModifiers;

	affinityModifierContext.empire1 = nullptr;
	affinityModifierContext.empire2 = nullptr;
	affinityModifierContext.currentTime = 0;
	affinityModifierContext.archetypesConfig = archetypesConfig;
	affinityModifierContext.affinityConfig = affinityConfig;
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

void cEmpireRelationsAnalyzer::GetEmpiresAffinityModifiersData(cEmpire* empire1, cEmpire* empire2, eastl::vector<AffinityModifierData>& affinityData) {
	affinityModifierContext.empire1 = empire1;
	affinityModifierContext.empire2 = empire2;
	affinityModifierContext.currentTime = persistedDiplomacyEventManager->CurrentTime();
	affinityModifierContext.diplomacyEvents.clear();
	persistedDiplomacyEventManager->GetPersistedDiplomaticEventsBetweenEmpires(affinityModifierContext.diplomacyEvents, empire1, empire2);

	for (IAffinityModifierPtr affinityModifier : affinityModifiers) {
		affinityData.push_back(affinityModifier->GetAffinityModifierData(affinityModifierContext));
	}
}

int cEmpireRelationsAnalyzer::EmpiresAffinity(cEmpire* empire1, cEmpire* empire2) {
	eastl::vector<AffinityModifierData> affinityModifiersData;
	GetEmpiresAffinityModifiersData(empire1, empire2, affinityModifiersData);
	int affinity = 0;
	int maxStableRelationsAffinity = 0;
	int maxWarTogetherAffinity = 0;
	for (const AffinityModifierData& affinityModifierData : affinityModifiersData) {
		if (affinityModifierData.active) {
			int affinityGain = affinityModifierData.affinityGain;
			if (affinityModifierData.stableRelationsMutuallyExclusive) {
				if (affinityGain > maxStableRelationsAffinity) {
					maxStableRelationsAffinity = affinityGain;
				}
				affinityGain = 0;
			}
			else if (affinityModifierData.warTogetherMutuallyExclusive) {
				if (affinityGain > maxWarTogetherAffinity) {
					maxWarTogetherAffinity = affinityGain;
				}
				affinityGain = 0;
			}
			affinity += affinityGain;
		}
	}
	return affinity + maxStableRelationsAffinity + maxWarTogetherAffinity;
}
