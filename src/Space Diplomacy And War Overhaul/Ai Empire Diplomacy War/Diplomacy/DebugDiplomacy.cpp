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
#include "../cCompositionRoot.h"
#include "../cSimulationValidator.h"

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
    cCompositionRoot* compositionRoot = cCompositionRoot::Get();

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
        RelationshipManager.DeclareWar(empire, empireColonizer);
        break;
    }
    case 3:  {
        
        cSimulationValidatorPtr validator = new cSimulationValidator(ResourceKey(0xeadaafdb, 0x00B1B104, 0x820b6795));
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
            if (persistedDiplomacyEvent->GetEmpire1() == GetPlayerEmpire() || persistedDiplomacyEvent->GetEmpire2() == GetPlayerEmpire()) {
                int b = 1;
            }
        }
        break;

    }
    case 10:{
        auto gameDatas = GetDataByCast<cGameData>();
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
        bool ret = EmpireUtils::ValidNpcEmpire(GetPlayerEmpire(), true);
        int b = 1;
        
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
        eastl::vector<cPersistedDiplomacyEventPtr> events;
        compositionRoot->persistedDiplomacyEventManager->GetPersistedDiplomaticEventsOfEmpire(events, GetPlayerEmpire());
        int b = 1;

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