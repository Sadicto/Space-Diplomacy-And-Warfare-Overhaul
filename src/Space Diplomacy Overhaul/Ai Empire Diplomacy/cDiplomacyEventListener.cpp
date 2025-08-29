#include "stdafx.h"
#include "cDiplomacyEventListener.h"
#include <cDiplomacyEvent.h>
#include "Spore-Mod-Utils/Include/SporeModUtils.h"

using namespace SporeModUtils;
using namespace Simulator;

cDiplomacyEventListener::cDiplomacyEventListener()
{
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
		// Improve Relation with the player empire;
	}
	else {
		RelationshipManager.DeclareAlliance(empire1, empire2);
	}
}

void cDiplomacyEventListener::OnBreakAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	if (empire2 == GetPlayerEmpire()) {
		// This should never happen
	}
	else {
		RelationshipManager.BreakAlliance(empire1, empire2);
	}
}

void cDiplomacyEventListener::OnConflictBreakAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	RelationshipManager.BreakAlliance(empire1, empire2);
	if (empire2 == GetPlayerEmpire()) {
		
	}
	else {

	}
}

void cDiplomacyEventListener::OnStableAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	RelationshipManager.ApplyRelationship(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceCreateAlliance);
}

void cDiplomacyEventListener::OnUnstableAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	if (empire2 == GetPlayerEmpire()) {

	}
	else {

	}
}

void cDiplomacyEventListener::OnHostileAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	if (empire2 == GetPlayerEmpire()) {
		RelationshipManager.ApplyRelationship(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceCreateAlliance, -0.20f);
	}
}

void cDiplomacyEventListener::OnDeclareWar(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	if (empire2 == GetPlayerEmpire()) {
		// Create tribute
	}
	else {
		RelationshipManager.DeclareWar(empire1, empire2);
	}
}

void cDiplomacyEventListener::OnContinueWar(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	RelationshipManager.ApplyRelationship(empire1->GetEmpireID(), empire2->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceStartedWar);
}

