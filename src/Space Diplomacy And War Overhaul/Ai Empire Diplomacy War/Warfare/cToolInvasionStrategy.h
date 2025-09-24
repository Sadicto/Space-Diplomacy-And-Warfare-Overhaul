#pragma once

#include <Spore\BasicIncludes.h>
#include "cWarfareStrengthAnalyzer.h"
#include "cWarfareEventDispatcher.h"

#define cToolInvasionStrategyPtr intrusive_ptr<cToolInvasionStrategy>

class cToolInvasionStrategy 
	: public Simulator::cToolStrategy
{
public:
	static const uint32_t TYPE = id("cToolInvasionStrategy");
	
	cToolInvasionStrategy();
	~cToolInvasionStrategy();

	//int AddRef() override;
	//int Release() override;
	//void* Cast(uint32_t type) const override;

	bool OnSelect(Simulator::cSpaceToolData* pTool) override;

	bool Update(Simulator::cSpaceToolData* pTool, bool showErrors, const char16_t** ppFailText = nullptr) override;

	int ProcessCost(int useCost) override;

	int useCost;

	cWarfareStrengthAnalyzerPtr warfareStrengthAnalyzer;
	cWarfareEventDispatcherPtr warfareEventDispatcher;
};
