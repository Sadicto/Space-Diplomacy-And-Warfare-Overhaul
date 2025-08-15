#include "stdafx.h"
#include "DebugDiplomacy.h"
#include <EASTL/string.h>
#include <Spore/Simulator/SubSystem/CommManager.h>
#include "cEmpireDiplomacyManager.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"
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
    cEmpireDiplomacyManager* EmpireDiplomacyManager = cEmpireDiplomacyManager::Get();
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
        cCommManager* comma = cCommManager::Get();
        //cCommEvent* comm = CommManager.CreateSpaceCommEvent(this->empireColonizer->GetEmpireID(), this->empireColonizer->RequireHomePlanet()->GetID(), 0x95bbbbfc, 0x7FC02FBC, nullptr, 2, 0);
        CnvAction action;
        action.actionID = 0x4C182387;
        CommManager.HandleSpaceCommAction(action, this->empireColonizer->GetEmpireID(), this->empireColonizer->RequireHomePlanet()->GetID(), nullptr);
        //CommManager.ShowCommEvent(comm);
        //comm->mDuration = 100000;
        //comm->mbVisibleInGalaxy = true;
        break;
    }
    case 3:  {
        CommManager.CreateSpaceCommEvent(this->empireColonizer->GetEmpireID(), this->empireColonizer->mHomePlanet, 0x95bbbbfc, id("demand_tribute"), nullptr, 1, 120);
        break;
    }
    case 4: {
        cEmpire* empire = StarManager.GetEmpire(playerData->mpActiveStar->mpStarRecord->mEmpireID);
        float AllianceProbability = EmpireDiplomacyManager->AllianceProbability(this->empireColonizer, empire);
        float BreakAllianceProbability = EmpireDiplomacyManager->BreakAllianceProbability(this->empireColonizer, empire);
        float DeclareWarProbability = EmpireDiplomacyManager->DeclareWarProbability(this->empireColonizer, empire);
        break;
    }
    case 5: {
        int agressivty = EmpireDiplomacyManager->GetEmpireAgressivity(empire);
        break;
    }
    case 6: {
        int affinity = EmpireDiplomacyManager->EmpiresAffinity(this->empireColonizer, empire);
        break;
    }
    case 7: {
        float allianceProbability = EmpireDiplomacyManager->AllianceProbability(empire, this->empireColonizer);
        break;
    }
    case 8: {
        float breakAllianceProbability = EmpireDiplomacyManager->BreakAllianceProbability(empire, this->empireColonizer);
        break;

    }
    case 9: {
        float warProbability = EmpireDiplomacyManager->DeclareWarProbability(empire, this->empireColonizer);
        break;

    }
    case 10:{
        eastl::vector<cEmpirePtr> empiresInRange;
        eastl::vector<cEmpirePtr> empireAllies;
        eastl::vector<cEmpirePtr> empireEnemies;
        DiplomacyUtils::GetEmpiresInRangeByRelation(empire, 30, empiresInRange, empireAllies, empireEnemies);

        break;
     }
    case 11: {
        eastl::vector<cEmpirePtr> empireAllies;
        DiplomacyUtils::GetAlliesInRange(empire, 30, empireAllies);
        int systemCount = EmpireUtils::GetSystemCountWithAllies(empire, empireAllies);
        
        break;
    }
    case 12: {
        eastl::vector<cEmpirePtr> empiresAround;
        float radius = 40.0f;
        EmpireUtils::GetEmpiresInRadius(GetActiveStarRecord()->mPosition, radius, empiresAround, true);
        for (cEmpirePtr empireAround : empiresAround) {
            eastl::vector<cEmpirePtr> empiresInRange;
            eastl::vector<cEmpirePtr> empireAllies;
            eastl::vector<cEmpirePtr> empireEnemies;
            DiplomacyUtils::GetEmpiresInRangeByRelation(empireAround.get(), 10, empiresInRange, empireAllies, empireEnemies);
        }
        int b = 0;

        
        break;
    }
    case 13: {
        EmpireDiplomacyManager->ManageEmpireDiplomacy(empire);
        break;
    }
    case 14: {
        break;
    }
    case 15: {
        
        break;
    }
    case 16: {
        uint32_t base = baseAddress;
        int c = 0;
        break;
    }
    case 17: {
        cBadgeManager* badgeManager = SimulatorSpaceGame.mpBadgeManager.get();
        int a = 4;
        
        break;
    }
    case 18: {
        cPlayer* player = Simulator::GetPlayer();
        int empirePoliticalId = GetActiveStarRecord()->mEmpireID;
        bool ret = CALL(Address(0x00c7a910), bool, Args(cPlayer*, int), Args(player, empirePoliticalId));
        int a = 9;
        break;
    }
    case 19: {
        eastl::vector<cEmpirePtr> empires;
        EmpireUtils::GetEmpiresInRangeOfEmpire(GetPlayerEmpire(), 40, empires);
        for (cEmpirePtr empire : empires) {
            bool found = DiplomacyUtils::PlayerContactedEmpire(empire.get());
            int b = 5;
        }
        
        break;
    }
    case 20: {
        cPlayer* player = Simulator::GetPlayer();
        int empirePoliticalId = GetActiveStarRecord()->mEmpireID;
        auto it = player->mPlayerSpecificEmpireData.find(empirePoliticalId);
        if (it != player->mPlayerSpecificEmpireData.end()) {
            int value = it->second;
            int a = 4;
        }
        break;
    }
    case 21: {
        int empirePoliticalId = GetActiveStarRecord()->mEmpireID;
        bool ret = CALL(Address(0x00c7a8b0), bool, Args(Simulator::cPlayer*, int), Args(Simulator::GetPlayer(), empirePoliticalId));
        int b = 4;
        break;
    }
    case 22: {
        eastl::vector<cEmpirePtr> empires;
        EmpireUtils::GetEmpiresInRangeOfEmpire(GetPlayerEmpire(), 40, empires);
        for (cEmpirePtr empire : empires) {
            bool found = DiplomacyUtils::EmpireIsAwareOfPlayer(empire.get());
            int b = 5;
        }
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