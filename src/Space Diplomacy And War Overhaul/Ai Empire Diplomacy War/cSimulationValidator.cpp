#include "stdafx.h"
#include "cSimulationValidator.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace SporeModUtils;
using namespace Simulator;


cSimulationValidator::cSimulationValidator(ResourceKey validatorConfigKey){
	PropertyListPtr validatosConfigProp;
	PropManager.GetPropertyList(validatorConfigKey.instanceID, validatorConfigKey.groupID, validatosConfigProp);
	int empireInvalidationDepthInt;
	App::Property::GetInt32(validatosConfigProp.get(), 0x903BB67C, empireInvalidationDepthInt);
	empireInvalidationDepth = EmpireInvalidationDepth(empireInvalidationDepthInt);

	for (const auto& empire : StarManager.GetEmpires()) {
		if (((empire.second->mFlags & EmpireFlags::kEmpireFlagFromSaveGame) != 0) && empire.second != Simulator::GetPlayerEmpire()) {
			invalidEmpires.insert(empire.second);
			if (empireInvalidationDepth == EmpireInvalidationDepth::directRelations) {
				InvalidateAlliesAndEnemies(empire.second.get());
			}
			else if (empireInvalidationDepth == EmpireInvalidationDepth::indirectRelations) {
				InvalidateAlliesAndEnemies(empire.second.get(), true);
			}
		}
	}
}

cSimulationValidator::~cSimulationValidator()
{
}

// For internal use, do not modify.
int cSimulationValidator::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cSimulationValidator::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cSimulationValidator::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cSimulationValidator);
	return nullptr;
}

void cSimulationValidator::InvalidateAlliesAndEnemies(Simulator::cEmpire* empire, bool includeIndirectRelations){
	for (cEmpirePtr ally : empire->mAllies) {
		if (EmpireUtils::ValidNpcEmpire(ally.get()) && invalidEmpires.find(ally) == invalidEmpires.end()) {
			invalidEmpires.insert(ally);
			if (includeIndirectRelations) {
				InvalidateAlliesAndEnemies(ally.get());
			}
		}
	}
	for (cEmpirePtr enemy : empire->mEnemies) {
		if (EmpireUtils::ValidNpcEmpire(enemy.get()) && invalidEmpires.find(enemy) == invalidEmpires.end()) {
			invalidEmpires.insert(enemy);
			if (includeIndirectRelations) {
				InvalidateAlliesAndEnemies(enemy.get());
			}
		}
	}
}

bool cSimulationValidator::ValidEmpire(Simulator::cEmpire* empire, bool includePlayer, bool includeGrox){
	return EmpireUtils::ValidNpcEmpire(empire, includePlayer, includeGrox) &&
		invalidEmpires.find(empire) == invalidEmpires.end();
}

bool cSimulationValidator::ValidStar(Simulator::cStarRecord* star){
	return StarUtils::ValidStar(star, true, false, false, true, false);
}

bool cSimulationValidator::ValidPlanet(Simulator::cPlanetRecord* planet){
	return PlanetUtils::InteractablePlanet(planet);
}
