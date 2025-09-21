#include "stdafx.h"
#include "DebugWarfare.h"
#include "cSpaceCombatMetrics.h"
#include "cWarfareConfig.h"
#include "cWarfareStrengthAnalyzer.h"
#include "cEmpireWarfare.h"
#include "cWarfareEventDispatcher.h"
#include "cWarfareEventListener.h"
#include "cPlanetAttackedEvent.h"

DebugWarfare::DebugWarfare()
{
    selectedStar = nullptr;
    selectedEmpire = nullptr;
}


DebugWarfare::~DebugWarfare()
{
}


void DebugWarfare::ParseLine(const ArgScript::Line& line)
{
	auto args = line.GetArguments(1);
	int val = mpFormatParser->ParseInt(args[0]);
	SpacePlayerData* playerData = SpacePlayerData::Get();
	cEmpire* empire = StarManager.GetEmpire(playerData->mpActiveStar->mpStarRecord->mEmpireID);

    ResourceKey warfareConfigKey = ResourceKey(0x0fcf340f, 0, 0x4c9daa54);
    cWarfareConfigPtr warfareConfig = new cWarfareConfig(warfareConfigKey);

    ResourceKey spaceCombatKey = ResourceKey(0x9f9f0139, 0, 0x02ae0c7e);
    cSpaceCombatMetricsPtr spaceCombatMetrics = new cSpaceCombatMetrics(spaceCombatKey);

    cWarfareStrengthAnalyzerPtr warfareStrengthAnalyzer = new cWarfareStrengthAnalyzer(warfareConfig.get(), spaceCombatMetrics.get());

    cWarfareEventDispatcherPtr warfareEventDispatcher = new cWarfareEventDispatcher();

    cEmpireWarfarePtr empireWarfare = new cEmpireWarfare(empire, warfareConfig.get(), warfareStrengthAnalyzer.get(), warfareEventDispatcher.get());

    switch (val) {
    case 0: { // Set the star
        this->selectedStar = GetActiveStarRecord();
        App::ConsolePrintF("currentStar: %d", selectedStar->GetID().GetStarIndex());
        break;
    }
    case 1: { // Set the empire

        this->selectedEmpire = empire;
        App::ConsolePrintF("empire: %d", empire->GetEmpireID());
        break;
    }
    case 2: {
        cWarfareEventListener* warfareEventListener = new cWarfareEventListener();
        MessageManager.AddListener(warfareEventListener, cPlanetAttackedEvent::ID);
        break;
    }
    case 3: {
        //cPlanetRecord* planet = GetActivePlanetRecord();
        uint32_t empireID = GetActiveStarRecord()->mEmpireID;
        cEmpire* empire = StarManager.GetEmpire(empireID);
        //cMission* mission = MissionManager.CreateMission(id("RaidEvent"), planetC, empire);
        cMission* mission = MissionManager.CreateMission(id("RaidWarPlayer"), GetActivePlanetRecord(), empire);
        cPlanetPtr planetTarget;
        StarManager.RecordToPlanet(GetActivePlanetRecord(), planetTarget);
        cRaidEvent* raidEvent = static_cast<cRaidEvent*>(mission);
        //void* raidEventPointer = mission->Cast(cRaidEvent::TYPE);
        //cRaidEvent* raidEvent = static_cast<cRaidEvent*>(raidEventPointer);
        raidEvent->mAttackerEmpire = selectedEmpire->GetEmpireID();
        raidEvent->mpTargetPlanet = planetTarget;
        raidEvent->mNumBombers = 10;
        //raidEvent->mShowDefaultEventLog = false;
        raidEvent->AcceptMission();

        break;
    }
    case 4: {
        float bombers = warfareStrengthAnalyzer->GetBombersProducedByEmpire(empire);

        break;
    }
    case 5: {

        float bombers = warfareStrengthAnalyzer->GetBombersProducedBySystem(GetActiveStarRecord());

        break;
    }
    case 6: {

        float stregnthFactor = warfareStrengthAnalyzer->GetEmpireStrenghtFactor(empire);

        break;
    }
    case 7: {
        int bombersNeeded = warfareStrengthAnalyzer->GetBomberForceForSystem(selectedEmpire, GetActiveStarRecord());
        break;
    }
    case 8: {
        empireWarfare->SelectAndAttackTargets();
        break;

    }
    case 9: {
        bool a = MissionManager.ThereIsEventInPlanet(GetActivePlanetRecord());
        break;

    }
    case 10: {
        bool a = false;
        for (cPlanetRecordPtr planet : GetActiveStarRecord()->GetPlanetRecords()) {
            if (MissionManager.ThereIsEventInPlanet(planet.get())) {
                a = true;
                break;
            }
        }

        break;
    }
    case 11: {
        uint32_t eventId = id("RaidWar");
        Simulator::cEmpire* targetEmpire = StarManager.GetEmpire(GetActivePlanetRecord()->GetStarRecord()->mEmpireID);
        Simulator::cMission* mission = MissionManager.CreateMission(eventId, GetActivePlanetRecord(), targetEmpire);
        Simulator::cRaidEvent* raidEvent = static_cast<Simulator::cRaidEvent*>(mission);
        cPlanetPtr planetTarget;
        StarManager.RecordToPlanet(GetActivePlanetRecord(), planetTarget);
        raidEvent->mAttackerEmpire = GetPlayerEmpire()->GetEmpireID();
        raidEvent->mpTargetPlanet = planetTarget;
        raidEvent->mNumBombers = 6;
        raidEvent->AcceptMission();

        break;
    }
    case 12: {




        break;
    }
    case 13: {

        break;
    }
    case 14: {

        break;
    }
    case 15: {




        break;
    }
    case 16: {

        break;
    }
    case 17: {


        break;
    }
    case 18: {

        break;
    }
    case 19: {


        break;
    }
    case 20: {

        break;
    }
    case 21: {
        break;
    }
    case 22: {
        break;
    }
    default: { // Print the empire and star
        App::ConsolePrintF("empire: %d     star: %d",
            this->selectedEmpire->GetEmpireID(),
            this->selectedStar->GetID().GetStarIndex());
        break;
    }
    }
}

const char* DebugWarfare::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "DebugWarfare: Elaborate description of what this cheat does.";
	}
}
