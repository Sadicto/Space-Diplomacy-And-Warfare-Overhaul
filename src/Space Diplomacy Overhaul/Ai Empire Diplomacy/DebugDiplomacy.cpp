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
        RelationshipManager.ApplyRelationship(empire->GetEmpireID(), GetPlayerEmpire()->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceMissionComplete);
        break;
    }
    case 3:  {
        RelationshipManager.ApplyRelationship(GetPlayerEmpire()->GetEmpireID(), empire->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceMissionComplete);
        break;
    }
    case 4: {
        RelationshipManager.ApplyRelationship(GetPlayerEmpire()->GetEmpireID(), empire->GetEmpireID(), RelationshipEvents::kRelationshipEventSpaceStartedWar, 1.0f);
        break;
    }
    case 5: {

        break;
    }
    case 6: {

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