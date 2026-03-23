#pragma once
#include <Spore\BasicIncludes.h>
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include "cCompositionRoot.h"

using namespace SporeModUtils;
using namespace Simulator;

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
		if (relationshipID == RelationshipEvents::kRelationshipEventSpaceUpliftedCiv &&
			IsSpaceGame() &&
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
		original_function(this, empire1, empire2);
		if (IsSpaceGame() && EmpireUtils::ValidNpcEmpire(empire1, true) && EmpireUtils::ValidNpcEmpire(empire2, true)) {
			cCompositionRoot* compositionRoot = cCompositionRoot::Get();
			cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();
			persistedDiplomacyEventManager->DeletePersistedDiplomacyEvent(empire1, empire2, PersistedDiplomacyEventType::NeighborsInPeace);
			if (empire1 == GetPlayerEmpire() || empire2 == GetPlayerEmpire()) {
				persistedDiplomacyEventManager->DeleteAllPersistedDiplomacyEventsOfType(GetPlayerEmpire(), PersistedDiplomacyEventType::MadePeace);
				// TODO: Notify the player that all of their truces are broken.
			}
		}
	}
};

member_detour(DeclarePeace__detour, cRelationshipManager, void(cEmpire*, cEmpire*)) {
	void detoured(Simulator::cEmpire * empire1, Simulator::cEmpire * empire2) {
		original_function(this, empire1, empire2);
		if (IsSpaceGame() && EmpireUtils::ValidNpcEmpire(empire1, true) && EmpireUtils::ValidNpcEmpire(empire2, true)) {
			cCompositionRoot* compositionRoot = cCompositionRoot::Get();
			cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();
			persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(empire1, empire2, PersistedDiplomacyEventType::MadePeace);
			if (empire1 == GetPlayerEmpire() || empire2 == GetPlayerEmpire()) {
				cEmpire* otherEmpire =
					(empire1 == GetPlayerEmpire()) ? empire2 : empire1;
				for (cEmpirePtr playerAlly : GetPlayerEmpire()->mAllies) {
					if (!EmpireUtils::ValidNpcEmpire(playerAlly.get())) {
						continue;
					}
					if (DiplomacyUtils::War(playerAlly.get(), otherEmpire)) {
						// TODO: Make Peace.
					}
					persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(playerAlly.get(), otherEmpire, PersistedDiplomacyEventType::MadePeace);
					for (cEmpirePtr otherEmpireAlly : otherEmpire->mAllies) {
						if (!EmpireUtils::ValidNpcEmpire(otherEmpireAlly.get())) {
							continue;
						}
						if (DiplomacyUtils::War(playerAlly.get(), otherEmpire)) {
							// TODO: Make Peace.
						}
						persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(playerAlly.get(), otherEmpireAlly.get(), PersistedDiplomacyEventType::MadePeace);
					}
				}
			}
		}
	}
};

member_detour(DeclareAlliance__detour, cRelationshipManager, void(cEmpire*, cEmpire*)) {
	void detoured(Simulator::cEmpire * empire1, Simulator::cEmpire * empire2) {
		original_function(this, empire1, empire2);
		if (IsSpaceGame() && EmpireUtils::ValidNpcEmpire(empire1, true) && EmpireUtils::ValidNpcEmpire(empire2, true)) {
			cCompositionRoot* compositionRoot = cCompositionRoot::Get();
			cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();
			persistedDiplomacyEventManager->CreatePersistedDiplomacyEvent(empire1, empire2, PersistedDiplomacyEventType::FormedAlliance);
		}
	}
};

member_detour(BreakAlliance__detour, cRelationshipManager, void(cEmpire*, cEmpire*)) {
	void detoured(Simulator::cEmpire * empire1, Simulator::cEmpire * empire2) {
		original_function(this, empire1, empire2);
		if (IsSpaceGame() && EmpireUtils::ValidNpcEmpire(empire1, true) && EmpireUtils::ValidNpcEmpire(empire2, true)) {
			cCompositionRoot* compositionRoot = cCompositionRoot::Get();
			cPersistedDiplomacyEventManager* persistedDiplomacyEventManager = compositionRoot->persistedDiplomacyEventManager.get();
			persistedDiplomacyEventManager->DeletePersistedDiplomacyEvent(empire1, empire2, PersistedDiplomacyEventType::FormedAlliance);
		}
	}
};