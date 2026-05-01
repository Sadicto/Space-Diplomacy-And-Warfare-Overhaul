#pragma once
#include <Spore\BasicIncludes.h>
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include "cCompositionRoot.h"

using namespace SporeModUtils;
using namespace Simulator;

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
			DeclarePeaceWithPlayerAllianceBlock(npcEmpire);
		}
		
	}
};