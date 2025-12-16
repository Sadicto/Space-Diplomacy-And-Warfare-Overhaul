#pragma once

#include <Spore\BasicIncludes.h>
#include "Config\cWarfareConfig.h"
#include "cWarfareStrengthAnalyzer.h"
#include "cWarfareEventDispatcher.h"
#include "cEmpireWarfare.h"

#define cEmpireWarfareFactoryPtr intrusive_ptr<cEmpireWarfareFactory>

/// Factory whose job is to create instances of cEmpireWarfare.
class cEmpireWarfareFactory 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceWarfareOverhaul::cEmpireWarfareFactory");
	
	cEmpireWarfareFactory(cWarfareConfig* warfareConfig,
		cWarfareStrengthAnalyzer* warfareStrengthAnalyzer,
		cWarfareEventDispatcher* warfareEventDispatcher);
	~cEmpireWarfareFactory();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/**
	 * @brief Creates a?cEmpireWarfare instance linked to the given empire.
	 * @param empire  Pointer to the empire that the new warfare object will belong to.
	 * @return Pointer to the newly created cEmpireWarfare associated with empire.
	 */
	cEmpireWarfare* CreateEmpireWarfare(Simulator::cEmpire* empire);

	// Pointer to the loaded warfareConfig.
	cWarfareConfigPtr warfareConfig;

	// Pointer to the loaded warfareStrengthAnalyzer.
	cWarfareStrengthAnalyzerPtr warfareStrengthAnalyzer;

	// Pointer to the loaded warfareEventDispatcher.
	cWarfareEventDispatcherPtr warfareEventDispatcher;
};
