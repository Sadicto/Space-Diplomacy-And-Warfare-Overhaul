// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "DebugDiplomacy.h"
#include <Spore/Simulator/SubSystem/CommManager.h>
#include "cEmpireDiplomacyManager.h"
void Initialize()
{
	CheatManager.AddCheat("DebugDiplomacy", new DebugDiplomacy());
	cSimulatorSystem::Get()->AddStrategy(new cEmpireDiplomacyManager(), cEmpireDiplomacyManager::NOUN_ID);
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
}

void Dispose()
{
	// This method is called when the game is closing
}

member_detour(DeclareAlliance__detour, Simulator::cRelationshipManager, void(cEmpire* pEmpire1, cEmpire* pEmpire2)) {
	void detoured(cEmpire * pEmpire1, cEmpire * pEmpire2) {
		original_function(this, pEmpire1, pEmpire2);
		cEmpireDiplomacyManagerPtr EmpireDiplomacyManager = cEmpireDiplomacyManager::Get();
		uint32_t playerEmpireId = SpacePlayerData::Get()->mPlayerEmpireID;
		bool encountered = EmpireDiplomacyManager->EmpireEncountered(pEmpire1) || EmpireDiplomacyManager->EmpireEncountered(pEmpire2);
		if (pEmpire1->GetEmpireID() != playerEmpireId && pEmpire2->GetEmpireID() != playerEmpireId && encountered) {
			uint32_t eventID = UIEventLog.ShowEvent(id("AllianceBetweenAiEmpires"), GroupIDs::SpacePopups);
			eastl::string16 empire1 = pEmpire1->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire1->mArchetype) + u")";
			eastl::string16 empire2 = pEmpire2->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire2->mArchetype) + u")";
			eastl::string16 str1 = u"The " + empire1 + u" and " + empire2 + u" have formed an alliance.";
			UIEventLog.SetDescription(eventID, str1.c_str());
		}
	}
};

member_detour(BreakAlliance__detour, Simulator::cRelationshipManager, void(cEmpire* pEmpire1, cEmpire* pEmpire2)) {
	void detoured(cEmpire * pEmpire1, cEmpire * pEmpire2) {
		original_function(this, pEmpire1, pEmpire2);
		cEmpireDiplomacyManagerPtr EmpireDiplomacyManager = cEmpireDiplomacyManager::Get();
		uint32_t playerEmpireId = SpacePlayerData::Get()->mPlayerEmpireID;
		bool encountered = EmpireDiplomacyManager->EmpireEncountered(pEmpire1) || EmpireDiplomacyManager->EmpireEncountered(pEmpire2);
		if (pEmpire1->GetEmpireID() != playerEmpireId && pEmpire2->GetEmpireID() != playerEmpireId && encountered) {
			uint32_t eventID = UIEventLog.ShowEvent(id("AllianceEndedBetweenAiEmpires"), GroupIDs::SpacePopups);
			eastl::string16 empire1 = pEmpire1->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire1->mArchetype) + u")";
			eastl::string16 empire2 = pEmpire2->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire2->mArchetype) + u")";
			eastl::string16 str1 = u"The alliance between the " + empire1 + u" and " + empire2 + u" has broken down.";
			UIEventLog.SetDescription(eventID, str1.c_str());
		}
	}
};

member_detour(DeclareWar__detour, Simulator::cRelationshipManager, void(cEmpire* pEmpire1, cEmpire* pEmpire2)) {
	void detoured(cEmpire * pEmpire1, cEmpire * pEmpire2) {
		cEmpireDiplomacyManagerPtr EmpireDiplomacyManager = cEmpireDiplomacyManager::Get();
		if (!RelationshipManager.IsAtWar(pEmpire1, pEmpire2)) {
			original_function(this, pEmpire1, pEmpire2);
			uint32_t playerEmpireId = SpacePlayerData::Get()->mPlayerEmpireID;
			bool encountered = EmpireDiplomacyManager->EmpireEncountered(pEmpire1) || EmpireDiplomacyManager->EmpireEncountered(pEmpire2);
			if (pEmpire1->GetEmpireID() != playerEmpireId && pEmpire2->GetEmpireID() != playerEmpireId && encountered) {
				uint32_t eventID = UIEventLog.ShowEvent(id("WarDeclaredBetweenAiEmpires"), GroupIDs::SpacePopups);
				eastl::string16 empire1 = pEmpire1->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire1->mArchetype) + u")";
				eastl::string16 empire2 = pEmpire2->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire2->mArchetype) + u")";
				eastl::string16 str1 = u"The " + empire1 + u" declared war on the " + empire2;
				UIEventLog.SetDescription(eventID, str1.c_str());
			}
		}
	}
};

void AttachDetours()
{
	DeclareAlliance__detour::attach(GetAddress(cRelationshipManager, DeclareAlliance));
	BreakAlliance__detour::attach(GetAddress(cRelationshipManager, BreakAlliance));
	DeclareWar__detour::attach(GetAddress(cRelationshipManager, DeclareWar));
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

