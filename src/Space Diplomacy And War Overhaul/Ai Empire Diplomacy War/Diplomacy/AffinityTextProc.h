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

	// Pointer to the main UI window of the affinity interface.
	IWindowPtr mainWindow;

	// Pointer to the tooltip window that shows additional info.
	IWindowPtr tooltipWindow;

	// Pointer to the tooltip window that lists all modifiers.
	IWindowPtr tooltipModifiersWindow;

	// Pointer to the main affinity text element.
	IWindowPtr mainAffinityText;

	// Pointer to the secondary affinity text element that is displayed in the tooltip.
	IWindowPtr secondaryAffinityText;

	// The empire currently in conversation with the player.
	cEmpirePtr currentEmpire;

	// Pointer to the loaded empireRelationsAnalyzer.
	cEmpireRelationsAnalyzerPtr empireRelationsAnalyzer;

	// Text for the archetype compatibility effect.
	eastl::string16 archetypeAffinityText;

	// Text for the shared enemy effect.
	eastl::string16 commonEnemyText;

	// Text for the shared ally effect.
	eastl::string16 commonAllyText;

	// Text for the “at war with an ally” effect.
	eastl::string16 warWithAllyText;

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


	// Sets the affinity text for the empire with the given ID in the UI.
	// @param empireID The ID of the empire whose affinity value should be displayed.
	void SetAffinityText(uint32_t empireID);

	// Gets the localized text for a given affinity modifier.
	// @param affinityModifier The modifier whose description should be returned.
	eastl::string16 GetAffinityModifierText(AffinityModifier affinityModifier);

	// Finds an unused affinity modifier window to display new data.
	// @return A pointer to an available modifier window.
	IWindow* GetUnusedAffinityModifier();

	// Resets the affinity tooltip, clearing all displayed modifiers.
	void ResetAffinityToltip();

	// Populates the affinity tooltip with the current empire’s modifiers.
	void SetAffinityTooltip();

	
};
