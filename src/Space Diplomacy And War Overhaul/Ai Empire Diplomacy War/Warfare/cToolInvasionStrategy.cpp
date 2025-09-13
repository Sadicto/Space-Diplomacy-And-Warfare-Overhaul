#include "stdafx.h"
#include "cToolInvasionStrategy.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace SporeModUtils;
using namespace Simulator;

cToolInvasionStrategy::cToolInvasionStrategy()
{
	warfareStrengthAnalyzer = nullptr;
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
	if (Update(pTool, true)) {
		if (cToolStrategy::OnSelect(pTool)) {
			warfareEventDispatcher->DispatchPlanetAttackedEvent(GetPlayerEmpire(), GetActivePlanetRecord(), warfareStrengthAnalyzer->GetBomberForceForPlanet(GetPlayerEmpire(), GetActivePlanetRecord()));
			return true;
		}

	}
}

bool cToolInvasionStrategy::Update(Simulator::cSpaceToolData* pTool, bool showErrors, const char16_t** ppFailText) {
	// TODO check that the empire owner of the planet is an enemy and there's no event and.
	if (warfareEventDispatcher != nullptr && GetCurrentContext() == SpaceContext::Planet) {
		return cToolStrategy::Update(pTool, showErrors, ppFailText);
	}
	else {
		return false;
	}
}

int cToolInvasionStrategy::ProcessCost(int useCost) {
	if ((warfareStrengthAnalyzer != nullptr && GetCurrentContext() == SpaceContext::Planet)) {
		int bombersNeeded = warfareStrengthAnalyzer->GetBomberForceForPlanet(GetPlayerEmpire(), GetActivePlanetRecord());
		return useCost * bombersNeeded;
	}
	return 0;
}