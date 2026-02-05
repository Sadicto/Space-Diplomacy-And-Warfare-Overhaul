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
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::AffinityTextProc");
	
	AffinityTextProc(IWindow* affinityTooltipMainWindow, cEmpireRelationsAnalyzer* empireRelationsAnalyzer, ResourceKey affinityTextConfigKey);
	~AffinityTextProc();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	
	int GetEventFlags() const override;
	// This is the function you have to implement, called when a window you added this winproc to received an event
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;

	// Sets the affinity text for the empire with the given ID in the UI.
	// @param empireID The ID of the empire whose affinity value should be displayed.
	void SetAffinityTooltip(uint32_t empireID);

	// Gets the text for a given affinity modifier.
	// @param affinityModifier The modifier whose description should be returned.
	eastl::string16 GetAffinityModifierText(AffinityModifier affinityModifier);

	// Finds an unused affinity modifier window to display new data.
	// @return A pointer to an available modifier window.
	IWindow* GetUnusedAffinityModifier();

	// Resets the affinity tooltip, clearing all displayed modifiers.
	void ResetAffinityRollover();

	// Populates the affinity tooltip with the current empire’s modifiers.
	void SetAffinityRollover();

private:

	// Pointer to the loaded empireRelationsAnalyzer.
	cEmpireRelationsAnalyzerPtr empireRelationsAnalyzer;

	// Pointer to the main UI window of the affinity tooltip.
	IWindowPtr affinityTooltipMainWindow;

	// Pointer to the window with the current affinity's number in the tooltip.
	IWindowPtr affinityTooltipNumberWindow;

	uint32_t affinityRolloverLayoutId;

	UTFWin::UILayout* affinityRolloverLayout;

	// Pointer to the main UI window of the affinity rollover.
	IWindowPtr affinityRolloverMainWindow;

	// Pointer to the window that lists all modifiers in the rollover.
	IWindowPtr affinityRolloverModifiersWindow;

	// Pointer to the window with the current affinity's number in the rollover.
	IWindowPtr affinityRolloverNumberWindow;

	// The empire currently in conversation with the player.
	cEmpirePtr currentEmpire;

	int currentAffinity;

	Color currentAffinityColor;

	eastl::vector<AffinityModifierData> currentAffinityModifierData;

	// Stores the text for the affinity modifiers.
	eastl::vector<string16> affinityTexts;

	// Color used for negative affinity (strong).
	Color red;

	// Color used for negative affinity (mild).
	Color orange;

	// Color used for neutral/mixed affinity.
	Color yellow;

	// Color used for positive affinity (mild).
	Color cyan;

	// Color used for positive affinity (strong).
	Color green;

	Color gray;

	Color white;

	string16 arrowUp;

	string16 arrowDown;
};
