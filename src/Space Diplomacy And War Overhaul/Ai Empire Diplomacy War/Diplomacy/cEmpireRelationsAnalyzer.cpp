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
	int effectiveStableRelationsAffinityGain = 0;
	int priorityEffectiveStableRelations = -1;
	int positionEfectiveStableRelations = -1;
	int effectiveWarTogetherAffinity = 0;
	int priorityEffectiveWarTogether = -1;
	int positionEffectiveWarTogether = -1;
	// Set effective for each affinityModifierData. If changing something from here refactor this for into something less horrible.
	for (uint32_t i = 0; i < affinityData.size(); i++) {
		AffinityModifierData& affinityModifierData = affinityData[i];
		// TODO: Delete after testing.
		if (affinityModifierData.affinityModifier == AffinityModifier::LongPeace) {
			affinityModifierData.active = true;
			affinityModifierData.upgrading = false;
			affinityModifierData.decaying = false;
			affinityModifierData.upgradeTime = 6000000;
		}
		if (affinityModifierData.affinityModifier == AffinityModifier::DefeatedCommonEnemy) {
			affinityModifierData.active = true;
			affinityModifierData.affinityGain = 1;
			affinityModifierData.decaying = false;
			affinityModifierData.decayTime = 12000000;
		}
		if (affinityModifierData.active) {
			if (affinityModifierData.stableRelationsMutuallyExclusive) {
				if (affinityModifierData.affinityGain > effectiveStableRelationsAffinityGain || 
					(affinityModifierData.affinityGain == effectiveStableRelationsAffinityGain && affinityModifierData.priority > priorityEffectiveStableRelations)) {

					effectiveStableRelationsAffinityGain = affinityModifierData.affinityGain;
					priorityEffectiveStableRelations = affinityModifierData.priority;
					affinityModifierData.effective = true;
					if (positionEfectiveStableRelations > -1) {
						affinityData[positionEfectiveStableRelations].effective = false;
					}
					positionEfectiveStableRelations = i;
				}
				else {
					affinityModifierData.effective = false;
				}
			}
			else if (affinityModifierData.warTogetherMutuallyExclusive) {
				if (affinityModifierData.affinityGain > effectiveWarTogetherAffinity ||
					(affinityModifierData.affinityGain == effectiveWarTogetherAffinity && affinityModifierData.priority > priorityEffectiveWarTogether)) {

					effectiveWarTogetherAffinity = affinityModifierData.affinityGain;
					priorityEffectiveWarTogether = affinityModifierData.priority;
					affinityModifierData.effective = true;
					if (positionEffectiveWarTogether > -1) {
						affinityData[positionEffectiveWarTogether].effective = false;
					}
					positionEffectiveWarTogether = i;
				}
				else {
					affinityModifierData.effective = false;
				}
			}
			else {
				affinityModifierData.effective = true;
			}
		}
		else {
			affinityModifierData.effective = false;
		}
	}
}

int cEmpireRelationsAnalyzer::EmpiresAffinity(cEmpire* empire1, cEmpire* empire2) {
	eastl::vector<AffinityModifierData> affinityModifiersData;
	GetEmpiresAffinityModifiersData(empire1, empire2, affinityModifiersData);
	int affinity = 0;
	for (const AffinityModifierData& affinityModifierData : affinityModifiersData) {
		if (affinityModifierData.effective) {
			affinity += affinityModifierData.affinityGain;
		}
	}
	return affinity;
}
