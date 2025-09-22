#pragma once

#include <Spore\BasicIncludes.h>
#include "cEmpireRelationsAnalyzer.h"

#define AffinityTextProcPtr intrusive_ptr<AffinityTextProc>

// To avoid repeating UTFWin:: all the time.
using namespace UTFWin;

// Procedure responsible for displaying the empire affinity value in the UI.
class AffinityTextProc 
	: public IWinProc
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("AffinityTextProc");
	
	AffinityTextProc(IWindow* mainWindow, cEmpireRelationsAnalyzer* empireRelationsAnalyzer, ResourceKey affinityTextConfigKey);
	~AffinityTextProc();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	
	int GetEventFlags() const override;
	// This is the function you have to implement, called when a window you added this winproc to received an event
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;

	IWindow* mainWindow;
	IWindow* tooltipWindow;
	IWindow* textWindow;

	cEmpireRelationsAnalyzerPtr empireRelationsAnalyzer;

	Color red;
	Color orange;
	Color yellow;
	Color cyan;
	Color green;

	// Sets the affinity text for the empire with the given ID in the UI.
	// @param empireID The ID of the empire whose affinity value should be displayed.
	void SetAffinityText(uint32_t empireID);

	
};
