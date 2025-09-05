#include "stdafx.h"
#include "DebugDiplomacy.h"
#include <EASTL/string.h>
#include <Spore/Simulator/SubSystem/CommManager.h>
#include "cDiplomacySystem.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"
#include "AllianceEnemyButtonProc.h"
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
        RelationshipManager.ApplyRelationship(GetPlayerEmpire()->GetEmpireID(), empire->GetEmpireID(), RelationshipEvents::kRelationshipEventSpacePushedTooFar);
        break;

    }
    case 9: {
        RelationshipManager.ApplyRelationship(empire->GetEmpireID(), GetPlayerEmpire()->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceNewEmpireAndCTMGOwner);
        break;

    }
    case 10:{
        float value = RelationshipManager.GetRelationshipEventValue(empire->GetEmpireID(), GetPlayerEmpire()->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceGiveGift);
        RelationshipManager.ApplyRelationship(empire->GetEmpireID(), GetPlayerEmpire()->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceGiveGift, -value);

        break;
     }
    case 11: {
        eastl::array<int, 9> count;
        for (int i = 0; i < 9; i++) {
            count[i] = 0;
        }
        eastl::map<uint32_t, cEmpirePtr> empires = StarManager.GetEmpires();
        for (const eastl::pair<const uint32_t, cEmpirePtr>& pair : empires) {
            if (pair.second->mArchetype < 9) {
                count[pair.second->mArchetype]++;
            }
        }
        int a = 9;
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