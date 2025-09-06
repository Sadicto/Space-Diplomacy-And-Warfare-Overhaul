#include "stdafx.h"
#include "cDiplomacyEventListener.h"
#include "cDiplomacyEvent.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include <Spore/Simulator/SubSystem/CommManager.h>

using namespace SporeModUtils;
using namespace Simulator;

cDiplomacyEventListener::cDiplomacyEventListener(cDiplomacyPopupManager* diplomacyPopUpManager, cEmpireRelationshipController* empireRelationshipController)
{
	this->diplomacyPopUpManager = diplomacyPopUpManager;
	this->empireRelationshipController = empireRelationshipController;
}


cDiplomacyEventListener::~cDiplomacyEventListener()
{
}

// For internal use, do not modify.
int cDiplomacyEventListener::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cDiplomacyEventListener::Release()
{
	return DefaultRefCounted::Release();
}


// The method that receives the message. The first thing you should do is checking what ID sent this message...
bool cDiplomacyEventListener::HandleMessage(uint32_t messageID, void* message)
{
	if (messageID == cDiplomacyEvent::ID) {
		cDiplomacyEvent* diplomacyEvent = static_cast<cDiplomacyEvent*>(message);
		cEmpire* empire1 = diplomacyEvent->empire1;
		cEmpire* empire2 = diplomacyEvent->empire2;
		if (EmpireUtils::ValidNpcEmpire(empire1, true) && EmpireUtils::ValidNpcEmpire(empire2, true)) {
			switch (diplomacyEvent->eventType) {
			case(DiplomacyEventType::FormAlliance): {
				OnFormAlliance(empire1, empire2);
				break;
			}
			case(DiplomacyEventType::BreakAlliance): {
				OnBreakAlliance(empire1, empire2);
				break;
			}
			case(DiplomacyEventType::ConflictBreakAlliance): {
				OnConflictBreakAlliance(empire1, empire2);
				break;
			}
			case(DiplomacyEventType::StableAlliance): {
				OnStableAlliance(empire1, empire2);
				break;
			}
			case(DiplomacyEventType::UnstableAlliance): {
				OnUnstableAlliance(empire1, empire2);
				break;
			}
			case(DiplomacyEventType::HostileAlliance): {
				OnHostileAlliance(empire1, empire2);
				break;
			}
			case(DiplomacyEventType::DeclareWar): {
				OnDeclareWar(empire1, empire2);
				break;
			}
			case(DiplomacyEventType::ContinueWar): {
				OnContinueWar(empire1, empire2);
				break;
			}
			default: {
				return true;
			}
			}
			return true;
		}
	}
	return false;
}

void cDiplomacyEventListener::OnFormAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	if (empire2 == GetPlayerEmpire()) {
		float scaleApplied = empireRelationshipController->ApplyRelationshipEffect(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceAcceptGift);
		if (scaleApplied >= 1.0f) {
			diplomacyPopUpManager->ShowAIRelationImproved(empire1);
		}
	}
	else {
		diplomacyPopUpManager->ShowFormAllianceAI(empire1, empire2);
		RelationshipManager.DeclareAlliance(empire1, empire2);
	}
}

void cDiplomacyEventListener::OnBreakAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	RelationshipManager.BreakAlliance(empire1, empire2);
	if (empire2 == GetPlayerEmpire()) {
		
	}
	else {
		diplomacyPopUpManager->ShowBreakAllianceAI(empire1, empire2);
	}
}

void cDiplomacyEventListener::OnConflictBreakAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	if (empire2 == GetPlayerEmpire()) {
		diplomacyPopUpManager->ShowConflictBreakAlliancePlayer(empire1);
	}
	else {
		diplomacyPopUpManager->ShowConflictBreakAllianceAI(empire1, empire2);
	}
	RelationshipManager.BreakAlliance(empire1, empire2);
	empireRelationshipController->SetRelationshipEffectZero(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceCreateAlliance);
	empireRelationshipController->SetRelationshipEffectZero(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceAcceptGift);
}

void cDiplomacyEventListener::OnStableAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	empireRelationshipController->ApplyRelationshipEffect(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceCreateAlliance);
}

void cDiplomacyEventListener::OnUnstableAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	if (empire2 == GetPlayerEmpire()) {
		empireRelationshipController->DecayRelationshipEffect(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceAcceptGift);
		float decayApplied = empireRelationshipController->DecayRelationshipEffect(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceCreateAlliance);
		if (decayApplied >= 0.1f) {
			diplomacyPopUpManager->ShowUnstableAlliance(empire1);
		}
	}
	else {

	}
}

void cDiplomacyEventListener::OnHostileAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	// TODO no notification if already negative.
	if (empire2 == GetPlayerEmpire()) {
		empireRelationshipController->DecayRelationshipEffect(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceAcceptGift);
		empireRelationshipController->DecayRelationshipEffect(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceCreateAlliance);
		float scaleApplied = empireRelationshipController->ApplyRelationshipEffect(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpacePushedTooFar);
		if (scaleApplied >= 1.0f) {
			diplomacyPopUpManager->ShowHostileAlliance(empire1);
		}
	}
}

void cDiplomacyEventListener::OnDeclareWar(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	if (empire2 == GetPlayerEmpire()) {
		/*
		Simulator::CnvAction action;
		action.actionID = 0x4C182387;
		//when the relation is blue face or better this doesn´t work
		CommManager.HandleSpaceCommAction(action, empire1->GetEmpireID(), empire1->RequireHomePlanet()->GetID(), nullptr);
		*/
		diplomacyPopUpManager->ShowDeclareWarPlayer(empire1);
		empireRelationshipController->SetRelationshipEffectZero(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceCreateAlliance);
		empireRelationshipController->SetRelationshipEffectZero(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceAcceptGift);
		empireRelationshipController->SetRelationshipEffectMax(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceStartedWar);
	}
	else {
		diplomacyPopUpManager->ShowDeclareWarAI(empire1, empire2);
		RelationshipManager.DeclareWar(empire1, empire2);
	}
}

void cDiplomacyEventListener::OnContinueWar(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	empireRelationshipController->ApplyRelationshipEffect(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceStartedWar);
}

