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
    cEmpireDiplomacyManagerPtr EmpireDiplomacyManager = cEmpireDiplomacyManager::Get();
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

        break;
    }
    case 8: {

        break;

    }
    case 9: {

        break;

    }
    case 10:{

        break;
     }
    case 11: {

        
        break;
    }
    case 12: {
        cEmpire* playerEmpire = GetPlayerEmpire();
        cEmpire* knownEmpire = playerEmpire->mEnemies[0].get();
        cEmpire* unkownEmpire = playerEmpire->mEnemies[1].get();
        bool shouldBeTrue = RelationshipManager.RelationshipExists(playerEmpire->GetEmpireID(), knownEmpire->GetEmpireID());
        bool shouldBefalse = RelationshipManager.RelationshipExists(playerEmpire->GetEmpireID(), unkownEmpire->GetEmpireID());

        
        break;
    }
    case 13: {
        break;
    }
    case 14: {
        eastl::vector<int> field = StarManager.field_3C;
        eastl::vector<cStar*>& starVector = *reinterpret_cast<eastl::vector<cStar*>*>(&field);
        for (cStar* star : starVector) {
            int c = 0;
        }
        break;
    }
    case 15: {
        RelationshipManager.ApplyRelationship(playerData->mpActiveStar->mpStarRecord->mEmpireID, GetPlayerEmpireID(), kRelationshipEventSpaceCreateAlliance, 1.0f);
        
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