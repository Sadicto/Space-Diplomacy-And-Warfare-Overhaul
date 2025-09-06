// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "DebugDiplomacy.h"
#include <Spore/Simulator/SubSystem/CommManager.h>
#include <Spore/App/cMessageManager.h>
#include <Spore/App/IMessageManager.h>
#include "cDiplomacySystem.h"
#include <Spore/App/cCameraManager.h>
#include <Spore/Swarm/cEffectsManager.h>
void Initialize()
{
	CheatManager.AddCheat("DebugDiplomacy", new DebugDiplomacy());
	cSimulatorSystem::Get()->AddStrategy(new cDiplomacySystem(), cDiplomacySystem::NOUN_ID);
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

/*
member_detour(DeclareAlliance__detour, Simulator::cRelationshipManager, void(cEmpire* pEmpire1, cEmpire* pEmpire2)) {
	void detoured(cEmpire * pEmpire1, cEmpire * pEmpire2) {
		cDiplomacySystem* EmpireDiplomacyManager = cDiplomacySystem::Get();
		uint32_t eventID = UIEventLog.ShowEvent(id("AllianceBetweenAiEmpires"), GroupIDs::SpacePopups);
		eastl::string16 empire1 = pEmpire1->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire1->mArchetype) + u")";
		eastl::string16 empire2 = pEmpire2->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire2->mArchetype) + u")";
		eastl::string16 str1 = u"The " + empire1 + u" and " + empire2 + u" have formed an alliance.";
		UIEventLog.SetDescription(eventID, str1.c_str());
		original_function(this, pEmpire1, pEmpire2);
	}
};

member_detour(BreakAlliance__detour, Simulator::cRelationshipManager, void(cEmpire* pEmpire1, cEmpire* pEmpire2)) {
	void detoured(cEmpire * pEmpire1, cEmpire * pEmpire2) {
		cDiplomacySystem* EmpireDiplomacyManager = cDiplomacySystem::Get();
		uint32_t eventID = UIEventLog.ShowEvent(id("AllianceEndedBetweenAiEmpires"), GroupIDs::SpacePopups);
		eastl::string16 empire1 = pEmpire1->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire1->mArchetype) + u")";
		eastl::string16 empire2 = pEmpire2->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire2->mArchetype) + u")";
		eastl::string16 str1 = u"The alliance between the " + empire1 + u" and " + empire2 + u" has broken down.";
		UIEventLog.SetDescription(eventID, str1.c_str());
		original_function(this, pEmpire1, pEmpire2);
	}
};

member_detour(DeclareWar__detour, Simulator::cRelationshipManager, void(cEmpire* pEmpire1, cEmpire* pEmpire2)) {
	void detoured(cEmpire * pEmpire1, cEmpire * pEmpire2) {
		cDiplomacySystem* EmpireDiplomacyManager = cDiplomacySystem::Get();
		uint32_t eventID = UIEventLog.ShowEvent(id("WarDeclaredBetweenAiEmpires"), GroupIDs::SpacePopups);
		eastl::string16 empire1 = pEmpire1->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire1->mArchetype) + u")";
		eastl::string16 empire2 = pEmpire2->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire2->mArchetype) + u")";
		eastl::string16 str1 = u"The " + empire1 + u" declared war on the " + empire2;
		original_function(this, pEmpire1, pEmpire2);
		UIEventLog.SetDescription(eventID, str1.c_str());

	}
};
*/
/*
member_detour(TargetMinusOne__detour, cEmpireGrowthSim, void(uint32_t a, uint32_t b)) {
	void detoured(int a, int b) {
		//uint32_t noun = this->GetNounID();
		//uint32_t type = this->GetType();
		char16_t* name = this->GetName(99);
		original_function(this, a, b);
	}
};

member_detour(Creator__detour, cEmpireGrowthSim, cEmpireGrowthSim* ()) {
	cEmpireGrowthSim* detoured() {
		cEmpireGrowthSim* pointer = original_function(this);
		return pointer;
	}
};

member_detour(AIWar__detour, cEmpireGrowthSim, void(int empireID)) {
	void detoured(int empireID) {
		original_function(this, empireID);
		int a = 5;
	}
};
*/

member_detour(AddToBadgeProgress__detour, cBadgeManager, void(BadgeManagerEvent, int)) {
	void detoured(BadgeManagerEvent event, int value) {
		if (event == BadgeManagerEvent::ReqEmpiresMet) {
			int c = 1;
		}
		original_function(this, event, value);
	}
};

virtual_detour(MessageSend__detour, App::cMessageManager, App::IMessageManager, void(uint32_t, void*, App::IUnmanagedMessageListener*)){
	void detoured(uint32_t messageID, void* pMessage, App::IUnmanagedMessageListener* pListener) {
		if (messageID == 1932312912) {
			int a = 0;
		}
		original_function(this, messageID, pMessage, pListener);
	}
};

member_detour(MessageManaged__detour, Object, void(uint32_t, void*)) {
	void detoured(uint32_t messageID, void* message) {
		int a = 0;
		original_function(this, messageID, message);
	}
};

member_detour(CreateSpaceCommEvent__detour, cCommManager, cCommEvent* (uint32_t, PlanetID, uint32_t, uint32_t, void*, int, unsigned int)) {
	cCommEvent* detoured(uint32_t sourceEmpire, PlanetID planetKey, uint32_t fileID, uint32_t dialogID, void* pMission, int priority, unsigned int duration) {
		int a = 0;
		return original_function(this, sourceEmpire, planetKey, fileID, dialogID, pMission, priority, duration);
	}
};

member_detour(KnownEmpire__detour, cPlayer, bool(int)) {
	bool detoured(int politicalID) {
		int a = 0;
		bool ret = original_function(this, politicalID);
		return ret;
	}
};

member_detour(CreateVisualEffect__detour, Swarm::cEffectsManager, bool(uint32_t, uint32_t, IVisualEffectPtr&)) {
	bool detoured(uint32_t instanceID, uint32_t groupID, IVisualEffectPtr & dst) {
		Swarm::IEffectsManager* manager = Swarm::IEffectsManager::Get();
		return original_function(this, instanceID, groupID, dst);
	}
};

void AttachDetours()
{
	//DeclareAlliance__detour::attach(GetAddress(cRelationshipManager, DeclareAlliance));
	//BreakAlliance__detour::attach(GetAddress(cRelationshipManager, BreakAlliance));
	//DeclareWar__detour::attach(GetAddress(cRelationshipManager, DeclareWar));
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
	//TargetMinusOne__detour::attach(Address(0x00fea3a0));
	//Creator__detour::attach(Address(0x00feab60));
	//AIWar__detour::attach(Address(0x00fe9410));
	//AddToBadgeProgress__detour::attach(GetAddress(cBadgeManager, AddToBadgeProgress));
	//MessageSend__detour::attach(GetAddress(App::cMessageManager, MessageSend));
	//MessageManaged__detour::attach(Address(0x00fe3233));
	//CreateSpaceCommEvent__detour::attach(GetAddress(cCommManager, CreateSpaceCommEvent));
	//KnownEmpire__detour::attach(Address(0x00c7a910));
	//CreateVisualEffect__detour::attach(Address(0x00a6cad0));
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

