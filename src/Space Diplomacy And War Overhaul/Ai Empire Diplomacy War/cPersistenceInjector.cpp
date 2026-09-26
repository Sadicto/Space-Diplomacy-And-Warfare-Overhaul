#include "stdafx.h"
#include "cPersistenceInjector.h"
#include <cPersistedEvent.h>
#include <Diplomacy/PersistedEvent/cPersistedDiplomacyEvent.h>
#include <Diplomacy/PersistedEvent/cPreparingToDeclareWarEvent.h>

cPersistenceInjector::cPersistenceInjector(ISpaceTimeProvider* spaceTimeProvider, 
	cSimulationValidator* simulationValidator, 
	cDiplomacyConfig* diplomacyConfig,
	cEmpireRelationshipController* empireRelationshipController)
{
	this->spaceTimeProvider = spaceTimeProvider;
	this->simulationValidator = simulationValidator;
	this->diplomacyConfig = diplomacyConfig;
	this->empireRelationshipController = empireRelationshipController;
}

cPersistenceInjector::~cPersistenceInjector()
{
}

// For internal use, do not modify.
int cPersistenceInjector::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cPersistenceInjector::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cPersistenceInjector::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cPersistenceInjector);
	return nullptr;
}

void cPersistenceInjector::InjectObjectDependencies(cPersistedObject* persistedObject)
{
	if (persistedObject == nullptr)
	{
		return;
	}
	cPersistedEvent* persistedEvent = object_cast<cPersistedEvent>(persistedObject);
	if (persistedEvent != nullptr)
	{
		persistedEvent->InjectEventDependencies(spaceTimeProvider.get());
	}
	cPersistedDiplomacyEvent* persistedDiplomacyEvent = object_cast<cPersistedDiplomacyEvent>(persistedObject);
	if (persistedDiplomacyEvent != nullptr)
	{
		persistedDiplomacyEvent->InjectDiplomacyEventDependencies(simulationValidator.get());
	}
	cPreparingToDeclareWarEvent* preparingForWarEvent = object_cast<cPreparingToDeclareWarEvent>(persistedObject);
	if (preparingForWarEvent != nullptr)
	{
		preparingForWarEvent->InjectPreparingToDeclareWarEventDependencies(diplomacyConfig.get(), empireRelationshipController.get());
	}
	persistedObject->SetDependenciesInjected(true);
}
