#include "stdafx.h"
#include "cToolInvasionStrategy.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace SporeModUtils;
using namespace Simulator;

cToolInvasionStrategy::cToolInvasionStrategy()
{
	simulationValidator = nullptr;
	warfareStrengthAnalyzer = nullptr;
	warfareEventDispatcher = nullptr;
	dependenciesInjected = false;

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
	SpaceContext context = GetCurrentContext();
	bool inGalaxy = context == SpaceContext::Galaxy;
	bool inPlanetOrSystem = context == SpaceContext::Planet || context == SpaceContext::SolarSystem;
	int beforeUsingToolMoney = playerEmpire->mEmpireMoney;

	// OnSelect tries to charge the player for the tool.
	// If the balance doesn’t change, the player can’t afford it,
	// so we exit early without summoning the fleet.
	if (!cToolStrategy::OnSelect(pTool) || beforeUsingToolMoney == playerEmpire->mEmpireMoney  ) return false;

	if (inGalaxy && simulationValidator->ValidStar(GetActiveStarRecord())) {
		for (cPlanetRecordPtr planet : GetActiveStarRecord()->GetPlanetRecords()) {
			if (simulationValidator->ValidPlanet(planet.get()) && planet->GetTechLevel() == TechLevel::Empire) {
				warfareEventDispatcher->DispatchPlanetAttackedEvent(
					playerEmpire,
					planet.get(),
					warfareStrengthAnalyzer->GetBomberForceForPlanet(playerEmpire, planet.get())
				);
			}
		}
	}

	else if (inPlanetOrSystem && simulationValidator->ValidPlanet(GetActivePlanetRecord())) {
		warfareEventDispatcher->DispatchPlanetAttackedEvent(
			playerEmpire,
			GetActivePlanetRecord(),
			warfareStrengthAnalyzer->GetBomberForceForPlanet(playerEmpire, GetActivePlanetRecord())
		);
	}


	return true;

}

bool cToolInvasionStrategy::Update(Simulator::cSpaceToolData* pTool, bool showErrors, const char16_t** ppFailText) {
	if (!dependenciesInjected || !cToolStrategy::Update(pTool, true, ppFailText))
		return false;

	SpaceContext context = GetCurrentContext();
	bool inGalaxy = context == SpaceContext::Galaxy;
	bool inPlanetOrSystem = context == SpaceContext::Planet || context == SpaceContext::SolarSystem;

	if (inGalaxy && !simulationValidator->ValidStar(GetActiveStarRecord()))
		return false;

	if (inPlanetOrSystem && !simulationValidator->ValidPlanet(GetActivePlanetRecord()))
		return false;

	cEmpire* playerEmpire = GetPlayerEmpire();
	cEmpire* targetEmpire = StarManager.GetEmpire(GetActiveStarRecord()->mEmpireID);

	if (!simulationValidator->ValidEmpire(targetEmpire, false, true)) return false;
	if (!DiplomacyUtils::War(playerEmpire, targetEmpire)) return false;

	if (inGalaxy) {
		for (cPlanetRecordPtr planet : GetActiveStarRecord()->GetPlanetRecords()) {
			if (MissionManager.ThereIsEventInPlanet(planet.get()))
				return false;
		}
	}
	else if (inPlanetOrSystem && MissionManager.ThereIsEventInPlanet(GetActivePlanetRecord())) {
		return false;
	}

	return true;
}

int cToolInvasionStrategy::ProcessCost(int useCost) {
	if (warfareStrengthAnalyzer == nullptr) {
		return 0;
	}

	int bombersNeeded = 0;
	SpaceContext context = GetCurrentContext();
	bool inGalaxy = context == SpaceContext::Galaxy;
	bool inPlanetOrSystem = context == SpaceContext::Planet || context == SpaceContext::SolarSystem;

	if (inGalaxy && simulationValidator->ValidStar(GetActiveStarRecord())) {
		bombersNeeded = warfareStrengthAnalyzer->GetBomberForceForSystem(GetPlayerEmpire(), GetActiveStarRecord());
	}
	else if (inPlanetOrSystem && simulationValidator->ValidPlanet(GetActivePlanetRecord())) {
		bombersNeeded = warfareStrengthAnalyzer->GetBomberForceForPlanet(GetPlayerEmpire(), GetActivePlanetRecord());
	}

	return useCost * bombersNeeded;
}

void cToolInvasionStrategy::InjectDependencies(cSimulationValidator* simulationValidator, cWarfareStrengthAnalyzer* warfareStrengthAnalyzer, cWarfareEventDispatcher* warfareEventDispatcher)
{
	this->simulationValidator = simulationValidator;
	this->warfareStrengthAnalyzer = warfareStrengthAnalyzer;
	this->warfareEventDispatcher = warfareEventDispatcher;
	dependenciesInjected = true;
}

void cToolInvasionStrategy::ResetDependencies()
{
	this->simulationValidator.reset();
	this->warfareStrengthAnalyzer.reset();
	this->warfareEventDispatcher.reset();
	dependenciesInjected = false;
}
