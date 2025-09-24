#include "stdafx.h"
#include "cToolInvasionStrategy.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace SporeModUtils;
using namespace Simulator;

cToolInvasionStrategy::cToolInvasionStrategy()
{
	warfareStrengthAnalyzer = nullptr;
	warfareEventDispatcher = nullptr;

	PropertyListPtr toolInvasionProp;
	PropManager.GetPropertyList(0x3b04cb56, 0x30608f0b, toolInvasionProp);
	App::Property::GetInt32(toolInvasionProp.get(), 0x10609EDF, useCost);
}


cToolInvasionStrategy::~cToolInvasionStrategy()
{
}
/*
// For internal use, do not modify.
int cToolInvasionStrategy::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cToolInvasionStrategy::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cToolInvasionStrategy::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cToolInvasionStrategy);
	return nullptr;
}
*/

bool cToolInvasionStrategy::OnSelect(Simulator::cSpaceToolData* pTool) {
	if (!Update(pTool, true)) return false;

	cEmpire* playerEmpire = GetPlayerEmpire();
	cPlanetRecord* activePlanet = GetActivePlanetRecord();

	if (!cToolStrategy::OnSelect(pTool) || playerEmpire->mEmpireMoney < ProcessCost(useCost)) return false;

	warfareEventDispatcher->DispatchPlanetAttackedEvent(
		playerEmpire,
		activePlanet,
		warfareStrengthAnalyzer->GetBomberForceForPlanet(playerEmpire, activePlanet)
	);

	return true;

}

bool cToolInvasionStrategy::Update(Simulator::cSpaceToolData* pTool, bool showErrors, const char16_t** ppFailText) {
	if (warfareEventDispatcher == nullptr ||
		warfareStrengthAnalyzer == nullptr) {
		return false;
	}

	if (!cToolStrategy::Update(pTool, true, ppFailText)  ||
		GetCurrentContext() != SpaceContext::Planet) {
		return false;
	}

	cEmpire* playerEmpire = GetPlayerEmpire();
	cEmpire* targetEmpire = StarManager.GetEmpire(GetActiveStarRecord()->mEmpireID);
	cPlanetRecord* activePlanet = GetActivePlanetRecord();

	if (!EmpireUtils::ValidNpcEmpire(targetEmpire, false, true, true)) return false;
	if (!DiplomacyUtils::War(playerEmpire, targetEmpire)) return false;
	if (!PlanetUtils::InteractablePlanet(activePlanet)) return false;
	if (MissionManager.ThereIsEventInPlanet(activePlanet)) return false;

	return true;
}

int cToolInvasionStrategy::ProcessCost(int useCost) {
	if ((warfareStrengthAnalyzer != nullptr && GetCurrentContext() == SpaceContext::Planet)) {
		int bombersNeeded = warfareStrengthAnalyzer->GetBomberForceForPlanet(GetPlayerEmpire(), GetActivePlanetRecord());
		return useCost * bombersNeeded;
	}
	return 0;
}