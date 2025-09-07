#include "stdafx.h"
#include "cEmpireDiplomacy.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include "cDiplomacySystem.h"
using namespace Simulator;
using namespace SporeModUtils;


cEmpireDiplomacy::cEmpireDiplomacy(Simulator::cEmpire* empire, 
	cDiplomacyConfig* diplomacyConfig, 
	cEmpireRelationsAnalyzer* empireRelationsAnalyzer, 
	cDiplomacyEventDispatcher* diplomacyEventDispatcher)
{
	this->empire = empire;
	this->diplomacyConfig = diplomacyConfig;
	this->empireRelationsAnalyzer = empireRelationsAnalyzer;
	this->diplomacyEventDispatcher = diplomacyEventDispatcher;
}


cEmpireDiplomacy::~cEmpireDiplomacy()
{
}

// For internal use, do not modify.
int cEmpireDiplomacy::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cEmpireDiplomacy::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cEmpireDiplomacy::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cEmpireDiplomacy);
	return nullptr;
}

void cEmpireDiplomacy::ResolveAlliesWar() {
	for (cEmpirePtr ally1 : empire->mAllies) {
		for (cEmpirePtr ally2 : empire->mAllies) {
			if (EmpireUtils::ValidNpcEmpire(ally1.get(), true) && EmpireUtils::ValidNpcEmpire(ally1.get(), true) && 
				ally1 != ally2 && DiplomacyUtils::War(ally1.get(), ally2.get()) &&
				DiplomacyUtils::Alliance(empire.get(), ally1.get()) && DiplomacyUtils::Alliance(empire.get(), ally2.get())) {

				int affinityWIthAlly1 = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), ally1.get());
				int affinityWIthAlly2 = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), ally2.get());
				// affinityWIthAlly1 == affinityWIthAlly2 it's just random.
				if (affinityWIthAlly1 > affinityWIthAlly2) {
					diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::ConflictBreakAlliance, empire.get(), ally2.get());
				}
				else {
					diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::ConflictBreakAlliance, empire.get(), ally1.get());
				}
			}
		}
	}
}

Simulator::cEmpire* cEmpireDiplomacy::GetAllyEnemy() {
	for (cEmpirePtr neutral : neutrals) {
		if (EmpireUtils::ValidNpcEmpire(neutral.get(), true) && DiplomacyUtils::AllianceWithEnemyOfEmpire(empire.get(), neutral.get())) {
			return neutral.get();
		}
	}
	return nullptr;
}

float cEmpireDiplomacy::AllianceProbability(cEmpire* target) {
	int affinity = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), target);

	int affinityThresholdForStableAlliance = diplomacyConfig->GetAffinityThresholdForStableAlliance();
	int affinityThresholdForUnstableAlliance = diplomacyConfig->GetAffinityThresholdForUnstableAlliance();
	int maxAffinitySoftCap = diplomacyConfig->GetMaxAffinitySoftCap();

	float maxAllianceProbability = diplomacyConfig->GetMaxAllianceProbability();
	if ((affinity >= affinityThresholdForStableAlliance) || (affinity >= affinityThresholdForUnstableAlliance && DiplomacyUtils::CommonEnemy(empire.get(), target))) {
		float allianceProbability = 0.2f + (static_cast<float>(affinity - affinityThresholdForUnstableAlliance) / maxAffinitySoftCap);
		return min(allianceProbability, maxAllianceProbability);
	}
	else {
		return 0.0f;
	}
}

float cEmpireDiplomacy::BreakAllianceProbability(cEmpire* target) {
	int affinity = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), target);

	int affinityThresholdForStableAlliance = diplomacyConfig->GetAffinityThresholdForStableAlliance();
	int affinityThresholdForUnstableAlliance = diplomacyConfig->GetAffinityThresholdForUnstableAlliance();
	int maxAffinitySoftCap = diplomacyConfig->GetMaxAffinitySoftCap();
	if ((affinity < affinityThresholdForUnstableAlliance) || (affinity < affinityThresholdForStableAlliance && !DiplomacyUtils::CommonEnemy(empire.get(), target))) {
		float breakAllianceProbability = 0.5f + (abs(static_cast<float>(affinity - affinityThresholdForUnstableAlliance)) / maxAffinitySoftCap);
		return min(breakAllianceProbability, 1.0f);
	}
	else {
		return 0.0f;
	}
}

float cEmpireDiplomacy::DeclareWarProbability(cEmpire* target) {
	int affinityThresholdForWar = diplomacyConfig->GetAffinityThresholdForWar();
	int minAffinitySoftCap = diplomacyConfig->GetMinAffinitySoftCap();
	float maxWarProbability = diplomacyConfig->GetMaxWarProbability();
	int affinity = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), target);
	if ((affinity > affinityThresholdForWar) || (affinity == affinityThresholdForWar && DiplomacyUtils::CommonEnemy(empire.get(), target))) {
		return 0.0f;
	}
	else {
		int aggresivity = empireRelationsAnalyzer->GetEmpireAgressivity(empire.get());
		float warProbability = static_cast<float>(affinity - aggresivity - minAffinitySoftCap - 1) / (minAffinitySoftCap - 1);
		return min (warProbability * maxWarProbability, maxWarProbability);
	}
}


cEmpire* cEmpireDiplomacy::GetBreakAllianceTarget() {
	eastl::map<cEmpirePtr, float> probabilitiesOfBreakingAlliance;
	for (cEmpirePtr ally : empire->mAllies) {
		if (EmpireUtils::ValidNpcEmpire(ally.get())) {
			probabilitiesOfBreakingAlliance[ally] = BreakAllianceProbability(ally.get());
		}
	}
	for (auto pair : probabilitiesOfBreakingAlliance) {
		if (pair.second > Math::randf()) {
			return pair.first.get();
		}
	}
	return nullptr;
}

cEmpire* cEmpireDiplomacy::GetWarTarget() {
	eastl::map<cEmpirePtr, float> probabilitiesOfWar;
	for (cEmpirePtr neutral : neutrals) {
		if (EmpireUtils::ValidNpcEmpire(neutral.get(), false)) { // FALSE FOR DEBUG ONLY
			probabilitiesOfWar[neutral] = DeclareWarProbability(neutral.get());
		}
	}
	for (auto pair : probabilitiesOfWar) {
		if (pair.second > Math::randf()) {
			return pair.first.get();
		}
	}
	return nullptr;
}

cEmpire* cEmpireDiplomacy::GetAllianceTarget() {
	eastl::map<cEmpirePtr, float> probabilitiesOfAlliance;
	for (cEmpirePtr neutral : neutrals) {
		if (EmpireUtils::ValidNpcEmpire(neutral.get(), true)) {
			probabilitiesOfAlliance[neutral] = AllianceProbability(neutral.get());
		}
	}
	for (auto pair : probabilitiesOfAlliance) {
		if (pair.second > Math::randf()) {
			return pair.first.get();
		}
	}
	return nullptr;
}

void cEmpireDiplomacy::ManageAllies() {
	cEmpire* targetBreakAlliance = GetBreakAllianceTarget();
	if (targetBreakAlliance != nullptr) {
		diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::BreakAlliance, empire.get(), targetBreakAlliance);
	}
	for (cEmpirePtr ally : empire->mAllies) {
		if (EmpireUtils::ValidNpcEmpire(ally.get(), true)) {
			int affinity = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), ally.get());
			if (affinity > 0 || (affinity == 0 && DiplomacyUtils::CommonEnemy(empire.get(), ally.get()))) {
				diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::StableAlliance, empire.get(), ally.get());
			}
			else if (affinity == 0) {
				diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::UnstableAlliance, empire.get(), ally.get());
			}
			else {
				diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::HostileAlliance, empire.get(), ally.get());
			}
		}
	}
}

void cEmpireDiplomacy::ManageEnemies() {
	for (cEmpirePtr enemy : empire->mEnemies) {
		if (EmpireUtils::ValidNpcEmpire(enemy.get(), true)) {
			diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::ContinueWar, empire.get(), enemy.get());
		}
	}
}

void cEmpireDiplomacy::ManageNeutrals() {
	cEmpire* wartarget = nullptr;
	if (diplomacyConfig->GetAutoDeclareWarOnAllyEnemies()) {
		wartarget = GetAllyEnemy();
	}
	if (wartarget == nullptr && (diplomacyConfig->GetStartsWarsWhileAtWar() || empire->mEnemies.size() == 0)) {
		wartarget = GetWarTarget();
	}
	if (wartarget != nullptr) {
		diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::DeclareWar, empire.get(), wartarget);
	}
	cEmpire* targetAlliance = GetAllianceTarget();
	if (targetAlliance != nullptr) {
		diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::FormAlliance, empire.get(), targetAlliance);
	}
}

void cEmpireDiplomacy::ManageDiplomacy() {
	ResolveAlliesWar();
	EmpireUtils::GetEmpiresInRangeOfEmpire(empire.get(), empireRelationsAnalyzer->GetEmpireDiplomaticRange(empire.get()), neutrals, true);
	// Delete all the enemies and allies from the neutrals vector.
	neutrals.erase(
		eastl::remove_if(neutrals.begin(), neutrals.end(),
			[&](const cEmpirePtr otherEmpire) {
				return eastl::find(empire->mAllies.begin(), empire->mAllies.end(), otherEmpire) != empire->mAllies.end() ||
					eastl::find(empire->mEnemies.begin(), empire->mEnemies.end(), otherEmpire) != empire->mEnemies.end();
			}),
		neutrals.end());

	ManageEnemies();
	ManageNeutrals();
	ManageAllies();
}

