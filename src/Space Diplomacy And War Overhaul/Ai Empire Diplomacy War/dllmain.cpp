// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Diplomacy\DebugDiplomacy.h"
#include <Spore/Simulator/SubSystem/CommManager.h>
#include <Spore/App/cMessageManager.h>
#include <Spore/App/IMessageManager.h>
#include "Diplomacy\cDiplomacySystem.h"
#include <Spore/App/cCameraManager.h>
#include <Spore/Swarm/cEffectsManager.h>
#include "Warfare/DebugWarfare.h"
#include "Warfare/cWarfareSystem.h"
#include "Warfare/cToolInvasionStrategy.h"
#include "Diplomacy/AffinityTextProc.h"

void Initialize()
{
	//CheatManager.AddCheat("DebugDiplomacy", new DebugDiplomacy());
	//CheatManager.AddCheat("DebugWarfare", new DebugWarfare());
	cSimulatorSystem::Get()->AddStrategy(new cDiplomacySystem(), cDiplomacySystem::NOUN_ID);
	cSimulatorSystem::Get()->AddStrategy(new cWarfareSystem(), cWarfareSystem::NOUN_ID);
	ToolManager.AddStrategy(new cToolInvasionStrategy(), cToolInvasionStrategy::TYPE);
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

// Call the affinityTextProc when starting a commEvent with an empire.
member_detour(ShowCommEvent__detour, cCommManager, void(cCommEvent*)) {
	void detoured(cCommEvent * pEvent) {
		original_function(this, pEvent);
		if (IsSpaceGame()) {
			UTFWin::IWindow* mainWindow = WindowManager.GetMainWindow();
			UTFWin::IWindow* textWindow = mainWindow->FindWindowByID(0x434EB9AD);
			AffinityTextProc* affinityTextProcAux = nullptr;
			UTFWin::IWinProc* proc = textWindow->GetNextWinProc(affinityTextProcAux);
			AffinityTextProc* affinityTextProc = object_cast<AffinityTextProc>(proc);
			affinityTextProc->SetAffinityText(pEvent->mSource);
		}
	}
};

void AttachDetours()
{
	ShowCommEvent__detour::attach(GetAddress(cCommManager, ShowCommEvent));
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

