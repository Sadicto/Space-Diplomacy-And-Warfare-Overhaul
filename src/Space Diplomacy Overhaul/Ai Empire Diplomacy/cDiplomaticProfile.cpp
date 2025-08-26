#include "stdafx.h"
#include "cDiplomaticProfile.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"
#include <cEmpireDiplomacyManager.h>
using namespace Simulator;
using namespace SporeModUtils;

cDiplomaticProfile::cDiplomaticProfile(cEmpire* empire)
{
	this->empire = empire;
	this->strenght = CalculateStrenghtOfEmpire();
	this->srenghtOfAlliance = -1;
	for (cEmpirePtr empireAlly : empire->mAllies) {
		if (EmpireUtils::ValidNpcEmpire(empireAlly.get(), true)) {
			this->allies.push_back(empireAlly);
		}
	}
	for (cEmpirePtr empireEnemy : empire->mEnemies) {
		if (EmpireUtils::ValidNpcEmpire(empireEnemy.get(), true)) {
			this->enemies.push_back(empireEnemy);
		}
	}
	cEmpireDiplomacyManager* diplomacyManager = cEmpireDiplomacyManager::Get();
	diplomacyManager->GetEmpiresInDiplomaticRange(this->empire.get(), neutrals);
	// Delete all the enemies and allies from the neutrals vector.
	neutrals.erase(
		eastl::remove_if(neutrals.begin(), neutrals.end(),
			[&](const cEmpirePtr& empire) {
				return eastl::find(allies.begin(), allies.end(), empire) != allies.end() ||
					eastl::find(enemies.begin(), enemies.end(), empire) != enemies.end();
			}),
		neutrals.end());

}


cDiplomaticProfile::~cDiplomaticProfile()
{
}

// For internal use, do not modify.
int cDiplomaticProfile::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cDiplomaticProfile::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cDiplomaticProfile::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cDiplomaticProfile);
	return nullptr;
}

int cDiplomaticProfile::CalculateStrenghtOfEmpire() {
	return empire->mStars.size() * EmpireUtils::GetEmpireLevel(empire.get());
}

int cDiplomaticProfile::CalculateStrenghtOfAlliance() {
	//TODO
	return 1;
}

void cDiplomaticProfile::OnAllianceBroken(cEmpire* otherEmpire) {
	allies.erase(eastl::remove(allies.begin(), allies.end(), otherEmpire), allies.end());
}

void cDiplomaticProfile::OnWarStarted(cEmpire* otherEmpire) {
	neutrals.erase(eastl::remove(neutrals.begin(), neutrals.end(), otherEmpire), neutrals.end());
	enemies.push_back(cEmpirePtr(otherEmpire));
}

void cDiplomaticProfile::OnAllianceFormed(cEmpire* otherEmpire) {
	neutrals.erase(eastl::remove(neutrals.begin(), neutrals.end(), otherEmpire), neutrals.end());
	allies.push_back(cEmpirePtr(otherEmpire));
}

cEmpire* cDiplomaticProfile::GetBreakAllianceTarget() {
	cEmpireDiplomacyManager* diplomacyManager = cEmpireDiplomacyManager::Get();
	eastl::map<cEmpirePtr, float> probabilitiesOfBreakingAlliance;
	for (cEmpirePtr ally : allies) {
		if (EmpireUtils::ValidNpcEmpire(ally.get(), true)) {
			probabilitiesOfBreakingAlliance[ally] = diplomacyManager->BreakAllianceProbability(empire.get(), ally.get());
		}
	}
	for (auto pair : probabilitiesOfBreakingAlliance) {
		if (pair.second > Math::randf()) {
			return pair.first.get();
		}
	}
	return nullptr;
}

cEmpire* cDiplomaticProfile::GetWarTarget() {
	cEmpireDiplomacyManager* diplomacyManager = cEmpireDiplomacyManager::Get();
	eastl::map<cEmpirePtr, float> probabilitiesOfWar;
	for (cEmpirePtr neutral : neutrals) {
		if (EmpireUtils::ValidNpcEmpire(neutral.get(), true)) {
			probabilitiesOfWar[neutral] = diplomacyManager->DeclareWarProbability(empire.get(), neutral.get());
		}
	}
	for (auto pair : probabilitiesOfWar) {
		if (pair.second > Math::randf()) {
			return pair.first.get();
		}
	}
	return nullptr;
}

cEmpire* cDiplomaticProfile::GetAllianceTarget() {
	cEmpireDiplomacyManager* diplomacyManager = cEmpireDiplomacyManager::Get();
	eastl::map<cEmpirePtr, float> probabilitiesOfAlliance;
	for (cEmpirePtr neutral : neutrals) {
		if (EmpireUtils::ValidNpcEmpire(neutral.get(), true)) {
			probabilitiesOfAlliance[neutral] = diplomacyManager->AllianceProbability(empire.get(), neutral.get());
		}
	}
	for (auto pair : probabilitiesOfAlliance) {
		if (pair.second > Math::randf()) {
			return pair.first.get();
		}
	}
	return nullptr;

}


