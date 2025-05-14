#include "stdafx.h"
#include "DebugDiplomacy.h"
#include <EASTL/string.h>
#include <Spore/Simulator/SubSystem/CommManager.h>
#include "cEmpireDiplomacyManager.h"
using namespace Simulator;
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

    switch (val) {
    case 0: { // Set the star
        cStarRecord* actualStar = playerData->mpActiveStar->mpStarRecord.get();
        this->starToColonize = actualStar;
        App::ConsolePrintF("actualStar: %d", actualStar->GetID().GetStarIndex());
        break;
    }
    case 1: { // Set the empire
        cEmpire* empire = StarManager.GetEmpire(playerData->mpActiveStar->mpStarRecord->mEmpireID);
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
        cEmpire* empire = StarManager.GetEmpire(playerData->mpActiveStar->mpStarRecord->mEmpireID);
        EmpireDiplomacyManager->CreateTributeComm(empire);
        break;
    }
    case 6: {
        cEmpire* empire = StarManager.GetEmpire(playerData->mpActiveStar->mpStarRecord->mEmpireID);
        bool alliance  = RelationshipManager.IsAllied(this->empireColonizer, empire);
        break;
    }
    case 7: {
        cEmpire* empire = StarManager.GetEmpire(playerData->mpActiveStar->mpStarRecord->mEmpireID);
        RelationshipManager.DeclareAlliance(this->empireColonizer, empire);
        break;
    }
    case 8: {
        cEmpire* empire = StarManager.GetEmpire(playerData->mpActiveStar->mpStarRecord->mEmpireID);
        RelationshipManager.BreakAlliance(this->empireColonizer, empire);
        break;

    }
    case 9: {
        cEmpire* empire = StarManager.GetEmpire(playerData->mpActiveStar->mpStarRecord->mEmpireID);
        RelationshipManager.DeclareWar(this->empireColonizer, empire);
        break;

    }
    case 10:{
        RelationshipManager.ApplyRelationship(empireColonizer->GetEmpireID(), GetActiveStarRecord()->mEmpireID, kRelationshipEventSpaceAcceptGift, 2.5f);
        break;
     }
    case 11: {
        cEmpire* playerEmpire = GetPlayerEmpire();
        cPlayer* player = GetPlayer();
        
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
        cEmpire* empire = StarManager.GetEmpire(playerData->mpActiveStar->mpStarRecord->mEmpireID);
        bool encountered = EmpireDiplomacyManager->EmpireEncountered(empire);
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