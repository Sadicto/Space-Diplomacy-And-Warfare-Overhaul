#pragma once
#include <Spore\BasicIncludes.h>
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include "cCompositionRoot.h"

using namespace SporeModUtils;
using namespace Simulator;

/// @brief Declares peace between the player's allies and the specified empire.
/// @param empire The empire making peace with the player's allies.
void DeclarePeaceWithPlayerAllianceBlock(cEmpire* empire) {
	cCompositionRoot* compositionRoot = cCompositionRoot::Get();
	cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();
	cDiplomacyEventDispatcher* diplomacyEventDispatcher = compositionRoot->diplomacyEventDispatcher.get();

	for (cEmpirePtr playerAlly : GetPlayerEmpire()->mAllies) {
		if (!EmpireUtils::ValidNpcEmpire(playerAlly.get())) {
			continue;
		}
		if (DiplomacyUtils::War(playerAlly.get(), empire)) {
			diplomacyEventDispatcher->DispatchDiplomacyEvent(DiplomacyEventType::MadePeace, playerAlly.get(), empire);
		}
		// No active war, but creates a truce regardless.
		else {
			persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(playerAlly.get(), empire, PersistedDiplomacyEventType::MadePeace);
		}
	}
}

/// @brief Creates a "Defeated Enemy Together" diplomacy event for every unique pair of enemies belonging to the specified empire.
/// @param empire The empire used to generate pairs among its enemies.
/// @param allianceWithPlayer If true, filters the list to only include enemies that are currently allied with the player.
void CreateDefeatedEnemyTogetherForEnemiesOfEmpire(cEmpire* empire, bool allianceWithPlayer = false) {
	cCompositionRoot* compositionRoot = cCompositionRoot::Get();
	cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();

	const auto& enemies = empire->mEnemies;
	size_t count = enemies.size();
	for (size_t i = 0; i < count; ++i) {
		cEmpirePtr enemy1 = enemies[i];
		if (!EmpireUtils::ValidNpcEmpire(enemy1.get(), true) || (allianceWithPlayer && !DiplomacyUtils::Alliance(GetPlayerEmpire(), enemy1.get()))) {
			continue;
		}

		for (size_t j = i + 1; j < count; ++j) {
			cEmpirePtr enemy2 = enemies[j];
			if (!EmpireUtils::ValidNpcEmpire(enemy2.get(), true) || (allianceWithPlayer && !DiplomacyUtils::Alliance(GetPlayerEmpire(), enemy2.get()))) {
				continue;
			}
			persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(enemy1.get(), enemy2.get(), PersistedDiplomacyEventType::DefeatedEnemyTogether);
		}
	}
}

// Call the affinityTextProc when starting a commEvent with an empire.
member_detour(ShowCommEvent__detour, cCommManager, void(cCommEvent*)) {
	void detoured(cCommEvent * pEvent) {
		original_function(this, pEvent);
		if (IsSpaceGame()) {
			UTFWin::IWindow* mainWindow = WindowManager.GetMainWindow();
			UTFWin::IWindow* affinityTooltipMainWindow = mainWindow->FindWindowByID(0x434EB9AD);
			AffinityTextProc* affinityTextProcAux = nullptr;
			UTFWin::IWinProc* proc = affinityTooltipMainWindow->GetNextWinProc(affinityTextProcAux);
			AffinityTextProc* affinityTextProc = object_cast<AffinityTextProc>(proc);
			affinityTextProc->SetAffinityTooltip(pEvent->mSource);
		}
	}
};

// Create the uplifted by monolith event.
member_detour(ApplyRelationshipMonolith__detour, cRelationshipManager, float(uint32_t, uint32_t, uint32_t, float)) {
	float detoured(uint32_t politicalID, uint32_t causePoliticalID, uint32_t relationshipID, float scale = 1.0f) {
		if (IsSpaceGame() &&
			relationshipID == RelationshipEvents::kRelationshipEventSpaceUpliftedCiv &&
			EmpireUtils::ValidNpcEmpire(StarManager.GetEmpire(politicalID), true) &&
			EmpireUtils::ValidNpcEmpire(StarManager.GetEmpire(causePoliticalID), true)) {

			cCompositionRoot* compositionRoot = cCompositionRoot::Get();
			cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();
			persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(StarManager.GetEmpire(politicalID), StarManager.GetEmpire(causePoliticalID), PersistedDiplomacyEventType::UpliftedByMonolith);

		}
		return original_function(this, politicalID, causePoliticalID, relationshipID, scale);
	}
};

member_detour(DeclareWar__detour, cRelationshipManager, void(cEmpire*, cEmpire*)) {
	void detoured(Simulator::cEmpire * empire1, Simulator::cEmpire * empire2) {
		if (IsSpaceGame() && EmpireUtils::ValidNpcEmpire(empire1, true) && EmpireUtils::ValidNpcEmpire(empire2, true)){
			cCompositionRoot* compositionRoot = cCompositionRoot::Get();
			cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();

			persistedDiplomacyEventManager->DeletePersistedDiplomacyEvent(empire1, empire2, PersistedDiplomacyEventType::NeighborsInPeace);
			// If the player breaks any truce, all their truces are invalidated.
			if ((empire1 == GetPlayerEmpire() || empire2 == GetPlayerEmpire()) &&
				persistedDiplomacyEventManager->GetPersistedDiplomacyEventBetweenEmpires(empire1, empire2, PersistedDiplomacyEventType::MadePeace) != nullptr) {
				cEmpire* otherEmpire;
				if (empire1 == GetPlayerEmpire()) {
					otherEmpire = empire2;
				}
				else {
					otherEmpire = empire1;
				}
				persistedDiplomacyEventManager->DeleteAllPersistedDiplomacyEventsOfType(GetPlayerEmpire(), PersistedDiplomacyEventType::MadePeace);
				cDiplomacyPopupManager* diplomacyPopUpManager = compositionRoot->diplomacyPopUpManager.get();
				diplomacyPopUpManager->ShowTruceBrokenPlayer(otherEmpire);
			}
		}
		original_function(this, empire1, empire2);
	}
};

member_detour(DeclareAlliance__detour, cRelationshipManager, void(cEmpire*, cEmpire*)) {
	void detoured(Simulator::cEmpire * empire1, Simulator::cEmpire * empire2) {
		original_function(this, empire1, empire2);
		if (!IsSpaceGame()) {
			return;
		}
		if (!EmpireUtils::ValidNpcEmpire(empire1, true)) {
			return;
		}
		if (!EmpireUtils::ValidNpcEmpire(empire2, true)) {
			return;
		}
		cCompositionRoot* compositionRoot = cCompositionRoot::Get();
		cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();
		persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(empire1, empire2, PersistedDiplomacyEventType::FormedAlliance);
	}
};

member_detour(BreakAlliance__detour, cRelationshipManager, void(cEmpire*, cEmpire*)) {
	void detoured(Simulator::cEmpire * empire1, Simulator::cEmpire * empire2) {
		original_function(this, empire1, empire2);
		if (!IsSpaceGame()) {
			return;
		}
		if (!EmpireUtils::ValidNpcEmpire(empire1, true)) {
			return;
		}
		if (!EmpireUtils::ValidNpcEmpire(empire2, true)) {
			return;
		}
		cCompositionRoot* compositionRoot = cCompositionRoot::Get();
		cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();

		persistedDiplomacyEventManager->DeletePersistedDiplomacyEvent(empire1, empire2, PersistedDiplomacyEventType::FormedAlliance);
	}
};

member_detour(HandleSpaceCommAction__detour, cCommManager, void(const CnvAction& action,
	uint32_t sourceEmpireID,
	PlanetID planetKey,
	cMission* pMission)){

	void detoured(const CnvAction& action,
		uint32_t sourceEmpireID,
		PlanetID planetKey,
		cMission* pMission) {
		
		original_function(this, action, sourceEmpireID, planetKey, pMission);
		if (!IsSpaceGame()) {
			return;
		}
		cEmpire* npcEmpire = StarManager.GetEmpire(sourceEmpireID);
		if (!EmpireUtils::ValidNpcEmpire(npcEmpire)) {
			return;
		}
		cCompositionRoot* compositionRoot = cCompositionRoot::Get();
		cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();

		if (action.actionID == id("action_peace_offer_1") ||
			action.actionID == id("action_peace_offer_2") ||
			action.actionID == id("action_peace_offer_3") ||
			action.actionID == id("action_peace_offer_4") ||
			action.actionID == id("action_peace_offer_5")) {

			persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(GetPlayerEmpire(), npcEmpire, PersistedDiplomacyEventType::MadePeace);
			DeclarePeaceWithPlayerAllianceBlock(npcEmpire);
			for (cEmpirePtr npcEmpireAlly : npcEmpire->mAllies) {
				if (!EmpireUtils::ValidNpcEmpire(npcEmpireAlly.get())) {
					continue;
				}
				RelationshipManager.ResetRelationship(GetPlayerEmpire()->GetEmpireID(), npcEmpireAlly->GetEmpireID());
				RelationshipManager.DeclarePeace(GetPlayerEmpire(), npcEmpireAlly.get());
				// TODO: Reapply the archetype relationship effects.
				persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(GetPlayerEmpire(), npcEmpireAlly.get(), PersistedDiplomacyEventType::MadePeace);
				DeclarePeaceWithPlayerAllianceBlock(npcEmpireAlly.get());
			}
		}
		else if (action.actionID == id("action_accept_surrender")) {
			persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(GetPlayerEmpire(), npcEmpire, PersistedDiplomacyEventType::MadePeace);
			CreateDefeatedEnemyTogetherForEnemiesOfEmpire(npcEmpire, true);
			// Since original_function is called at the start, the player is no longer an enemy of npcEmpire when 
			// CreateDefeatedEnemyTogetherForEnemiesOfEmpire runs. Therefore, we must create the "DefeatedEnemyTogether" 
			// events for the player outside of that function.
			for (cEmpirePtr enemy : npcEmpire->mEnemies) {
				if (!EmpireUtils::ValidNpcEmpire(enemy.get()) || !DiplomacyUtils::Alliance(enemy.get(), GetPlayerEmpire())) {
					continue;
				}
				persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(enemy.get(), GetPlayerEmpire(), PersistedDiplomacyEventType::DefeatedEnemyTogether);
			}
			DeclarePeaceWithPlayerAllianceBlock(npcEmpire);
		}
		
	}
};

member_detour(ShowEvent__detour, cUIEventLog, uint32_t(uint32_t, uint32_t, int, Math::Vector3*, bool, int)) {
	uint32_t detoured(uint32_t instanceID, uint32_t groupID, int a = 0, Math::Vector3 * b = nullptr, bool dontAllowDuplicates = true, int c = 0) {

		uint32_t ret = original_function(this, instanceID, groupID, a, b, dontAllowDuplicates, c);

		if (!IsSpaceGame() || cCompositionRoot::Get()->diplomacyPopUpManager == nullptr) {
			return ret;
		}
		// If the popup is of "EmpireDestroyed" type, store its ID.
		if (instanceID == 0x88426556) {
			cDiplomacyPopupManager* diplomacyPopUpManager = cCompositionRoot::Get()->diplomacyPopUpManager.get();
			diplomacyPopUpManager->SetLastEmpireDestroyedPopUpID(ret);
		}
		return ret;
	}
};

member_detour(EmpireDestroyed__detour, cEmpire, void()) {
	void detoured() {
		original_function(this);

		if (!IsSpaceGame() || cCompositionRoot::Get()->diplomacyPopUpManager == nullptr) {
			return;
		}

		// If the player hasn't contacted the destroyed empire, hide the 'EmpireDestroyed' popup.
		if (!GetPlayer()->PlayerContactedEmpire(this->GetEmpireID())) {
			cDiplomacyPopupManager* diplomacyPopUpManager = cCompositionRoot::Get()->diplomacyPopUpManager.get();
			uint32_t popupID = diplomacyPopUpManager->GetlastEmpireDestroyedPopUpID();
			UIEventLog.HideEvent(popupID, false);
		}
		CreateDefeatedEnemyTogetherForEnemiesOfEmpire(this);
	}
};