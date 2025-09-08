#include "stdafx.h"
#include "cWarfareEventListener.h"
#include "cPlanetAttackedEvent.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace SporeModUtils;

cWarfareEventListener::cWarfareEventListener()
{
}


cWarfareEventListener::~cWarfareEventListener()
{
}

// For internal use, do not modify.
int cWarfareEventListener::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cWarfareEventListener::Release()
{
	return DefaultRefCounted::Release();
}


// The method that receives the message. The first thing you should do is checking what ID sent this message...
bool cWarfareEventListener::HandleMessage(uint32_t messageID, void* message)
{
	if (messageID == cPlanetAttackedEvent::ID) {
		cPlanetAttackedEvent* planetAttackedEvent = static_cast<cPlanetAttackedEvent*>(message);
		if (planetAttackedEvent != nullptr && 
			EmpireUtils::ValidNpcEmpire(planetAttackedEvent->aggressorEmpire, true) && 
			PlanetUtils::InteractablePlanet(planetAttackedEvent->target) && 
			planetAttackedEvent->target->GetTechLevel() == Simulator::TechLevel::Empire) {

			OnPlanetAttacked(planetAttackedEvent->aggressorEmpire, planetAttackedEvent->target, planetAttackedEvent->bombers);
		}
	}
	return true;
}

void cWarfareEventListener::OnPlanetAttacked(Simulator::cEmpire* aggressorEmpire, Simulator::cPlanetRecord* target, int bombers) {
	uint32_t eventId;
	Simulator::cEmpire* targetEmpire = StarManager.GetEmpire(target->GetStarRecord()->mEmpireID);

	if (targetEmpire == Simulator::GetPlayerEmpire()) {
		eventId = id("RaidWarPlayer");
	}
	else {
		eventId = id("RaidWar");
	}
	Simulator::cMission* mission = MissionManager.CreateMission(eventId, target, targetEmpire);
	Simulator::cRaidEvent* raidEvent = static_cast<Simulator::cRaidEvent*>(mission);
	cPlanetPtr planetTarget;
	StarManager.RecordToPlanet(target, planetTarget);
	raidEvent->mAttackerEmpire = aggressorEmpire->GetEmpireID();
	raidEvent->mpTargetPlanet = planetTarget;
	raidEvent->mNumBombers = bombers;
	raidEvent->AcceptMission();
}
