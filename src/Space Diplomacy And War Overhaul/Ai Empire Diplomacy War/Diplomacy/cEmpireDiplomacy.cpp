#include "stdafx.h"
#include "cEmpireDiplomacy.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include "cDiplomacySystem.h"
using namespace Simulator;
using namespace SporeModUtils;


cEmpireDiplomacy::cEmpireDiplomacy(Simulator::cEmpire* empire,
	cSimulationValidator* simulationValidator,
	cDiplomacyConfig* diplomacyConfig, 
	cEmpireRelationsAnalyzer* empireRelationsAnalyzer, 
	cDiplomacyEventDispatcher* diplomacyEventDispatcher,
	cPersistedDiplomacyEventManager* persistedDiplomacyEventManager)
{
	this->empire = empire;
	this->simulationValidator = simulationValidator;
	this->diplomacyConfig = diplomacyConfig;
	this->empireRelationsAnalyzer = empireRelationsAnalyzer;
	this->diplomacyEventDispatcher = diplomacyEventDispatcher;
	this->persistedDiplomacyEventManager = persistedDiplomacyEventManager;

	this->joiningAllyWar = false;
	this->decidedToPrepareWarDeclaration = false;
	this->readyToDeclareWar = false;
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
			if (simulationValidator->ValidEmpire(ally1.get(), true) && simulationValidator->ValidEmpire(ally1.get(), true) && 
				ally1 != ally2 && DiplomacyUtils::War(ally1.get(), ally2.get()) &&
				DiplomacyUtils::Alliance(empire.get(), ally1.get()) && DiplomacyUtils::Alliance(empire.get(), ally2.get())) {

				int affinityWIthAlly1 = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), ally1.get());
				int affinityWIthAlly2 = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), ally2.get());
				// if  affinityWIthAlly1 == affinityWIthAlly2 it's just random.
				if (affinityWIthAlly1 > affinityWIthAlly2) {
					diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::ConflictBreakAlliance, empire.get(), ally2.get());
					neutrals.push_back(ally2);
				}
				else {
					diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::ConflictBreakAlliance, empire.get(), ally1.get());
					neutrals.push_back(ally1);
				}
			}
		}
	}
}

Simulator::cEmpire* cEmpireDiplomacy::FindAllyEnemy() {
	for (cEmpirePtr neutral : neutrals) {
		if (simulationValidator->ValidEmpire(neutral.get(), true)) {
			for (cEmpirePtr ally : empire->mAllies) {
				if (DiplomacyUtils::War(neutral.get(), ally.get()) &&
					empireRelationsAnalyzer->EmpiresAffinity(empire.get(), ally.get()) >
					empireRelationsAnalyzer->EmpiresAffinity(empire.get(), neutral.get())) {
					joiningAllyWar = true;
					return neutral.get();
				}
			}
		}
	}
	return nullptr;
}

Simulator::cEmpire* cEmpireDiplomacy::GetPreparingToDeclareWarTarget()
{
	cPersistedDiplomacyEvent* preparingToDeclareWarEvent = persistedDiplomacyEventManager->GetPersistedDiplomacyEventOfType(
		empire.get(), PersistedDiplomacyEventType::PreparingToDeclareWarEvent, EmpireRole::Empire1);
	if (preparingToDeclareWarEvent == nullptr)
	{
		return nullptr;
	}
	return preparingToDeclareWarEvent->GetEmpire2();
}

float cEmpireDiplomacy::AllianceProbability(cEmpire* target) {
	int affinity = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), target);

	int affinityThresholdForStableAlliance = diplomacyConfig->GetAffinityThresholdForStableAlliance();
	int affinityThresholdForUnstableAlliance = diplomacyConfig->GetAffinityThresholdForUnstableAlliance();
	int maxAffinitySoftCap = diplomacyConfig->GetMaxAffinitySoftCap();

	float maxAllianceProbability = diplomacyConfig->GetMaxAllianceProbability();
	if ((affinity >= affinityThresholdForStableAlliance) || (affinity >= affinityThresholdForUnstableAlliance && DiplomacyUtils::CommonEnemy(empire.get(), target))) {
		float allianceProbability = min(0.1f + (float(affinity - affinityThresholdForUnstableAlliance) / float(maxAffinitySoftCap)), 1.0f);
		return allianceProbability * maxAllianceProbability;
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
	int affinity = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), target);
	if ((affinity > affinityThresholdForWar) || (affinity == affinityThresholdForWar && DiplomacyUtils::CommonEnemy(empire.get(), target))) {
		return 0.0f;
	}
	else {
		int maxAgressivity = diplomacyConfig->GetMaxAgressivity();
		int minAgressivity = diplomacyConfig->GetMinAgressivity();
		int minAffinitySoftCap = diplomacyConfig->GetMinAffinitySoftCap();
		float aggressivityWeightForWar = diplomacyConfig->GetAgressivityWeightForWar();
		float affinityWeightForWar = diplomacyConfig->GetAffinityWeightForWar();
		float maxWarProbability = diplomacyConfig->GetMaxWarProbability();
		float minWarProbability = diplomacyConfig->GetMinWarProbability();

		int aggresivity = empireRelationsAnalyzer->GetEmpireAgressivity(empire.get());

		float effectiveAffinity = float(pow((float(affinity) / float(minAffinitySoftCap)), 2.0f));
		float effectiveAgressivity = float(pow((float(aggresivity) + maxAgressivity) / (maxAgressivity - minAgressivity), 3.0f));

		float warProbability = min (effectiveAgressivity * (aggressivityWeightForWar + affinityWeightForWar * effectiveAffinity), 1.0f);
		warProbability = warProbability * maxWarProbability;
		return max (warProbability, minWarProbability);
	}
}



cEmpire* cEmpireDiplomacy::GetBreakAllianceTarget() {
	eastl::map<cEmpirePtr, float> probabilitiesOfBreakingAlliance;
	for (cEmpirePtr ally : empire->mAllies) {
		if (simulationValidator->ValidEmpire(ally.get())) {
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
	cEmpire* targetEmpire = GetPreparingToDeclareWarTarget();
	if (targetEmpire != nullptr)
	{
		readyToDeclareWar = true;
		return targetEmpire;
	}
	eastl::map<cEmpirePtr, float> probabilitiesOfWar;
	for (cEmpirePtr neutral : neutrals) {
		if (simulationValidator->ValidEmpire(neutral.get(), true) && 
			persistedDiplomacyEventManager->GetPersistedDiplomacyEventBetweenEmpires(empire.get(), neutral.get(), PersistedDiplomacyEventType::MadePeace) == nullptr) {
			probabilitiesOfWar[neutral] = DeclareWarProbability(neutral.get());
		}
	}
	for (auto pair : probabilitiesOfWar) {
		if (pair.second > Math::randf()) {
			decidedToPrepareWarDeclaration = true;
			return pair.first.get();
		}
	}
	return nullptr;
}

cEmpire* cEmpireDiplomacy::GetAllianceTarget() {
	eastl::map<cEmpirePtr, float> probabilitiesOfAlliance;
	for (cEmpirePtr neutral : neutrals) {
		if (simulationValidator->ValidEmpire(neutral.get(), true)) {
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
		if (simulationValidator->ValidEmpire(ally.get(), true)) {
			int affinity = empireRelationsAnalyzer->EmpiresAffinity(empire.get(), ally.get());
			if (affinity > 0 || (affinity == 0 && DiplomacyUtils::CommonEnemy(empire.get(), ally.get()))) {
				diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::StableAlliance, empire.get(), ally.get());
			}
			else {
				diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::UnstableAlliance, empire.get(), ally.get());
			}
		}
	}
}

void cEmpireDiplomacy::ManageEnemies() {
	for (cEmpirePtr enemy : empire->mEnemies) {
		if (simulationValidator->ValidEmpire(enemy.get(), true)) {
			diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::ContinueWar, empire.get(), enemy.get());
		}
	}
}

void cEmpireDiplomacy::ManageNeutrals() {
	cEmpire* wartarget = nullptr;
	if (diplomacyConfig->GetAutoDeclareWarOnAllyEnemies()) {
		wartarget = FindAllyEnemy();
	}
	if (wartarget == nullptr && (diplomacyConfig->GetStartsWarsWhileAtWar() || empire->mEnemies.size() == 0)) {
		wartarget = GetWarTarget();
	}
	if (wartarget != nullptr) {
		if (joiningAllyWar)
		{
			diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::JoinAllyWar, empire.get(), wartarget);
		}
		else if (decidedToPrepareWarDeclaration)
		{
			diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::PreparingToDeclareWar, empire.get(), wartarget);
		}
		else if (readyToDeclareWar)
		{
			diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::UnprovokedWar, empire.get(), wartarget);
		}
		auto it = eastl::find(neutrals.begin(), neutrals.end(), wartarget);
		if (it != neutrals.end()) {
			neutrals.erase(it);
		}
	}
	cEmpire* targetAlliance = GetAllianceTarget();
	if (targetAlliance != nullptr) {
		diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::FormAlliance, empire.get(), targetAlliance);
		if (targetAlliance != GetPlayerEmpire()){
			auto it = eastl::find(neutrals.begin(), neutrals.end(), targetAlliance);
			if (it != neutrals.end()) {
				neutrals.erase(it);
			}
		}
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

	for (cEmpirePtr neutral : neutrals) {
		if (simulationValidator->ValidEmpire(neutral.get(), true)){
			diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::NeighborsInPeace, empire.get(), neutral.get());
		}
	}

	for (cEmpirePtr ally : empire->mAllies) {
		if (simulationValidator->ValidEmpire(ally.get(), true)) {
			diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::NeighborsInPeace, empire.get(), ally.get());
		}
	}
}

