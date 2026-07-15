#pragma once

#include <Spore\BasicIncludes.h>
#include "cWarfareStrengthAnalyzer.h"
#include "cWarfareEventDispatcher.h"
#include <cSimulationValidator.h>

#define cToolInvasionStrategyPtr intrusive_ptr<cToolInvasionStrategy>

class cToolInvasionStrategy 
	: public Simulator::cToolStrategy
{
public:
	static const uint32_t TYPE = id("SpaceWarfareOverhaul::cToolInvasionStrategy");
	
	cToolInvasionStrategy();
	~cToolInvasionStrategy();

	//int AddRef() override;
	//int Release() override;
	//void* Cast(uint32_t type) const override;

	bool OnSelect(Simulator::cSpaceToolData* pTool) override;

	bool Update(Simulator::cSpaceToolData* pTool, bool showErrors, const char16_t** ppFailText = nullptr) override;

	int ProcessCost(int useCost) override;

	/**
	* @brief Injects the required dependencies into the class.
	* @param simulationValidator.
	* @param empireDiplomacyFactory.
	*/
	void InjectDependencies(cSimulationValidator* simulationValidator, cWarfareStrengthAnalyzer* warfareStrengthAnalyzer, cWarfareEventDispatcher* warfareEventDispatcher);

	void ResetDependencies();

	int useCost;

private:
	// Pointer to the loaded simulation validator object.
	cSimulationValidatorPtr simulationValidator;

	// Pointer to the loaded warfare strength analyzer object.
	cWarfareStrengthAnalyzerPtr warfareStrengthAnalyzer;

	// Pointer to the loaded warfare event dispatcher.
	cWarfareEventDispatcherPtr warfareEventDispatcher;

	// Indicates whether the class’s dependencies have been injected.
	bool dependenciesInjected;
};
