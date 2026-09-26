#pragma once

#include <Spore\BasicIncludes.h>
#include "ISpaceTimeProvider.h"
#include "cSimulationValidator.h"
#include "cPersistedObject.h"
#include "Diplomacy/Config/cDiplomacyConfig.h"
#include "Diplomacy/cEmpireRelationshipController.h"

#define cPersistenceInjectorPtr intrusive_ptr<cPersistenceInjector>

/// Class responsible for injecting dependencies into cPersistedObjects.
class cPersistenceInjector 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyWarfareOverhaul::cPersistenceInjector");
	
	cPersistenceInjector(ISpaceTimeProvider* spaceTimeProvider, 
		cSimulationValidator* simulationValidator, 
		cDiplomacyConfig* diplomacyConfig,
		cEmpireRelationshipController* empireRelationshipController);

	~cPersistenceInjector();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// Inject dependencies into persistedObject according to its type, then calls 'persistedObject->SetDependenciesInjected(true)'.
	void InjectObjectDependencies(cPersistedObject* persistedObject);

private:
	ISpaceTimeProviderPtr spaceTimeProvider;
	cSimulationValidatorPtr simulationValidator;
	cDiplomacyConfigPtr diplomacyConfig;
	cEmpireRelationshipControllerPtr empireRelationshipController;
};
