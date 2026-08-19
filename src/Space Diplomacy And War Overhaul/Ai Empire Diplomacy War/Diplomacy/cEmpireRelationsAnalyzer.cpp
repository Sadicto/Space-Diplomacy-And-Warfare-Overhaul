#include "stdafx.h"
#include "cEmpireRelationsAnalyzer.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace Simulator;
using namespace SporeModUtils;

cEmpireRelationsAnalyzer::cEmpireRelationsAnalyzer(cDiplomacyConfig* diplomacyConfig, 
	cArchetypesConfig* archetypesConfig, 
	cAffinityConfig* affinityConfig,
	cPersistedDiplomacyEventManager* persistedDiplomacyEventManager,
	ISpaceTimeProvider* spaceTimeProvider,
	eastl::vector<IAffinityModifierPtr> affinityModifiers)
{
	this->diplomacyConfig = diplomacyConfig;
	this->archetypesConfig = archetypesConfig;
	this->affinityConfig = affinityConfig;
	this->persistedDiplomacyEventManager = persistedDiplomacyEventManager;
	this->spaceTimeProvider = spaceTimeProvider;
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
	affinityModifierContext.currentTime = spaceTimeProvider->GetCurrentSpaceTime();
	affinityModifierContext.diplomacyEvents.clear();
	persistedDiplomacyEventManager->GetPersistedDiplomaticEventsBetweenEmpires(affinityModifierContext.diplomacyEvents, empire1, empire2, false);

	for (IAffinityModifierPtr affinityModifier : affinityModifiers) {
		affinityData.push_back(affinityModifier->GetAffinityModifierData(affinityModifierContext));
	}
	for (AffinityModifierData& affinityModifierData :affinityData)
	{
		affinityModifierData.effective = affinityModifierData.active;
	}
	SetEffectiveForGroup(affinityData, MutuallyExclusiveModifierGroup::StableRelations);
	SetEffectiveForGroup(affinityData, MutuallyExclusiveModifierGroup::WarTogether);
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

bool cEmpireRelationsAnalyzer::BelongsToGroup(const AffinityModifierData& data, MutuallyExclusiveModifierGroup group)
{
	switch (group) {
	case MutuallyExclusiveModifierGroup::WarTogether:
	{
		return data.warTogetherMutuallyExclusive;
	}
	case MutuallyExclusiveModifierGroup::StableRelations:
	{
		return data.stableRelationsMutuallyExclusive;
	}
	}
	return false;
}

void cEmpireRelationsAnalyzer::SetEffectiveForGroup(eastl::vector<AffinityModifierData>& affinityData, MutuallyExclusiveModifierGroup group)
{
	AffinityModifierData* winner = nullptr;
	// Find the winner of the group.
	for (AffinityModifierData& affinityModifierData : affinityData)
	{
		if (!affinityModifierData.active || !BelongsToGroup(affinityModifierData, group))
		{
			continue;
		}
		if (winner == nullptr)
		{
			winner = &affinityModifierData;
			continue;
		}
		if (affinityModifierData.affinityGain > winner->affinityGain ||
			(affinityModifierData.affinityGain == winner->affinityGain && affinityModifierData.priority > winner->priority))
		{
			winner = &affinityModifierData;
		}

	}
	// Set effective = true for the winner and effective = false for the rest of the group.
	for (AffinityModifierData & affinityModifierData : affinityData)
	{
		if (!affinityModifierData.active || !BelongsToGroup(affinityModifierData, group))
		{
			continue;
		}
		if (&affinityModifierData == winner)
		{
			affinityModifierData.effective = true;
		}
		else
		{
			affinityModifierData.effective = false;
		}
	}
}
