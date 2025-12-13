#pragma once

#include <Spore\BasicIncludes.h>
#include "cDiplomacyConfig.h"
#include "cEmpireRelationsAnalyzer.h"
#include "cDiplomacyEventDispatcher.h"
#include "cEmpireDiplomacy.h"

#define cEmpireDiplomacyFactoryPtr intrusive_ptr<cEmpireDiplomacyFactory>

/// Factory whose job is to create instances of cEmpireDiplomacy.
class cEmpireDiplomacyFactory 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cEmpireDiplomacyFactory");
	
	cEmpireDiplomacyFactory(cDiplomacyConfig* diplomacyConfig,
		cEmpireRelationsAnalyzer* empireRelationsAnalyzer,
		cDiplomacyEventDispatcher* diplomacyEventDispatcher);
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

	// Pointer to the loaded diplomacy configuration object.
	cDiplomacyConfigPtr diplomacyConfig;

	// Pointer to the loaded empire relations analyzer object.
	cEmpireRelationsAnalyzerPtr empireRelationsAnalyzer;

	// Pointer to the loaded diplomacy event dispatcher.
	cDiplomacyEventDispatcherPtr diplomacyEventDispatcher;
};
