#pragma once

#include <Spore\BasicIncludes.h>
#include "Config\cDiplomacyConfig.h"
#include "cEmpireRelationsAnalyzer.h"
#include "cDiplomacyEventDispatcher.h"
#include "cEmpireDiplomacy.h"
#include <cSimulationValidator.h>

#define cEmpireDiplomacyFactoryPtr intrusive_ptr<cEmpireDiplomacyFactory>

/// Factory whose job is to create instances of cEmpireDiplomacy.
class cEmpireDiplomacyFactory 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cEmpireDiplomacyFactory");
	
	cEmpireDiplomacyFactory(cSimulationValidator* simulationValidator, 
		cDiplomacyConfig* diplomacyConfig,
		cEmpireRelationsAnalyzer* empireRelationsAnalyzer,
		cDiplomacyEventDispatcher* diplomacyEventDispatcher,
		cPersistedDiplomacyEventManager* persistedDiplomacyEventManager);
	~cEmpireDiplomacyFactory();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/**
	 * @brief Creates a cEmpireDiplomacy instance linked to the given empire.
	 * @param empire  Pointer to the empire that the new diplomacy object will belong to.
	 * @return Pointer to the newly created cEmpireDiplomacy associated with empire.
	 */
	cEmpireDiplomacy* CreateEmpireDiplomacy(Simulator::cEmpire* empire);

private:

	// Pointer to the loaded simulation validator object.
	cSimulationValidatorPtr simulationValidator;

	cDiplomacyConfigPtr diplomacyConfig;

	cEmpireRelationsAnalyzerPtr empireRelationsAnalyzer;

	cDiplomacyEventDispatcherPtr diplomacyEventDispatcher;

	cPersistedDiplomacyEventManagerPtr persistedDiplomacyEventManager;
};
