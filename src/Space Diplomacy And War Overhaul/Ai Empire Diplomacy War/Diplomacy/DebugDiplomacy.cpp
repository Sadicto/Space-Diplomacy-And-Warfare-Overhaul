#include "stdafx.h"
#include "DebugDiplomacy.h"
#include <EASTL/string.h>
#include <Spore/Simulator/SubSystem/CommManager.h>
#include "cDiplomacySystem.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include "AllianceEnemyButtonProc.h"
#include "Diplomacy/PersistedEvent/cPersistedDiplomacyEvent.h"
#include "Diplomacy/AffinityModifier/cCommonEnemyAffinityModifier.h"
#include "Diplomacy/PersistedEvent/cUpliftedByMonolithEvent.h"

using namespace Simulator;
using namespace SporeModUtils;
DebugDiplomacy::DebugDiplomacy()
{
}


DebugDiplomacy::~DebugDiplomacy()
{
}


void DebugDiplomacy::ParseLine(const ArgScript::Line& line) {
    auto args = line.GetArguments(1);
    int val = mpFormatParser->ParseInt(args[0]);
    cDiplomacySystem* EmpireDiplomacyManager = cDiplomacySystem::Get();
    SpacePlayerData* playerData = SpacePlayerData::Get();
    cEmpire* empire = StarManager.GetEmpire(playerData->mpActiveStar->mpStarRecord->mEmpireID);

    switch (val) {
    case 0: { // Set the star
        cStarRecord* actualStar = playerData->mpActiveStar->mpStarRecord.get();
        this->starToColonize = actualStar;
        App::ConsolePrintF("actualStar: %d", actualStar->GetID().GetStarIndex());
        break;
    }
    case 1: { // Set the empire
        
        this->empireColonizer = empire;
        App::ConsolePrintF("empire: %d", empire->GetEmpireID());
        break;
    }
    case 2: {
        RelationshipManager.ApplyRelationship(empire->GetEmpireID(), GetPlayerEmpire()->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceTradeComplete);
        break;
    }
    case 3:  {
        RelationshipManager.ApplyRelationship(GetPlayerEmpire()->GetEmpireID(), empire->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceTradeRouteSpice);
        break;
    }
    case 4: {
        RelationshipManager.ApplyRelationship(GetPlayerEmpire()->GetEmpireID(), empire->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceAcceptGift);
        break;
    }
    case 5: {
        Simulator::CnvAction action;
        action.actionID = 0x4C182387;
        //when the relation is blue face or better this doesn´t work
        CommManager.HandleSpaceCommAction(action, empire->GetEmpireID(), empire->RequireHomePlanet()->GetID(), nullptr);
        break;
    }
    case 6: {
        GameTimeManager.SetSpeed(0);

        break;
    }
    case 7: {
        GameTimeManager.SetSpeedFactors(1.0, 2.0, 4.0, 20.0);
        GameTimeManager.SetSpeed(3);
        break;
    }
    case 8: {
        cPersistedDiplomacyEventPtr persistedDiplomacyEvent = simulator_new<cPersistedDiplomacyEvent>();
        persistedDiplomacyEvent->SetCreationTime(4);
        persistedDiplomacyEvent->SetExpirationTime(0);
        persistedDiplomacyEvent->SetExpires(false);
        persistedDiplomacyEvent->SetEmpire1(GetPlayerEmpire());
        persistedDiplomacyEvent->SetEmpire2(empire);
        break;

    }
    case 9: {
        auto persistedDiplomacyEvents = GetDataByCast<cPersistedDiplomacyEvent>();
        for (cPersistedDiplomacyEventPtr persistedDiplomacyEvent : persistedDiplomacyEvents) {
            bool b = persistedDiplomacyEvent->Valid();
        }
        break;

    }
    case 10:{
        auto persistedDiplomacyEvents = GetData<cUpliftedByMonolithEvent>();
        for (cPersistedDiplomacyEventPtr persistedDiplomacyEvent : persistedDiplomacyEvents) {
            bool b = persistedDiplomacyEvent->Valid();
        }
        break;
     }
    case 11: {
        auto persistedEvents = GetData<cPersistedEvent>();
        for (cPersistedEventPtr persistedEvent : persistedEvents) {
            int b = 1;
        }
        break;
    }
    case 12: {
        cCommonEnemyAffinityModifierPtr  commonEnemyAffinityModifier = new cCommonEnemyAffinityModifier();
        
        break;
    }
    case 13: {
        bool inRange = EmpireUtils::EmpireInRangeOfEmpire(GetPlayerEmpire(), empire, 10.0f);
        break;
    }
    case 14: {
        uint32_t lAddress = baseAddress;

        break;
    }
    case 15: {
        cPlanetRecord* planet = GetActivePlanetRecord();
        planet->mTribeData.clear();
        

        
        break;
    }
    case 16: {
        Simulator::StarRequestFilter filter;
        filter.RemoveStarType(Simulator::StarType::None);
        filter.RemoveStarType(Simulator::StarType::GalacticCore);
        filter.RemoveStarType(Simulator::StarType::ProtoPlanetary);
        filter.RemoveStarType(Simulator::StarType::BlackHole);
        filter.techLevels = 0;

        //even stars with all planet in T0 have this techLevel
        filter.AddTechLevel(Simulator::TechLevel::Empire);
        filter.minDistance = 0;
        filter.maxDistance = 100;
        eastl::vector<cStarRecordPtr> stars;
        StarManager.FindStars(GetActiveStarRecord()->mPosition, filter, stars);
        for (cStarRecordPtr star : stars) {
            eastl::vector<cPlanetRecordPtr> planets = star->GetPlanetRecords();
            for (cPlanetRecordPtr planet : planets) {
                if (planet->GetTechLevel() == TechLevel::Empire && !planet->mTribeData.empty()) {
                    planet->mTribeData.clear();
                }
            }
        }

        break;
    }
    case 17: {
        UTFWin::IWindow* mainWindow = WindowManager.GetMainWindow();
        UTFWin::IWindow* textWindow = mainWindow->FindWindowByID(0x04C71F00);
        
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
        App::ConsolePrintF("empire: %d     actualStar: %d",
            this->empireColonizer->GetEmpireID(),
            this->starToColonize->GetID().GetStarIndex());
        break;
    }
    }
}

const char* DebugDiplomacy::GetDescription(ArgScript::DescriptionMode mode) const
{
    if (mode == ArgScript::DescriptionMode::Basic) {
        return "This cheat does something.";
    }
    else {
        return "MyCheat: Elaborate description of what this cheat does.";
    }
}