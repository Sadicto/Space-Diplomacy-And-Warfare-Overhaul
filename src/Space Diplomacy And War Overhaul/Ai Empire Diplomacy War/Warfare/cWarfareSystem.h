#pragma once

#include <Spore\BasicIncludes.h>
#include <Spore\Simulator\SubSystem\SimulatorSystem.h>
#include "cWarfareConfig.h"
#include "cSpaceCombatMetrics.h"
#include "cWarfareStrengthAnalyzer.h"
#include "cWarfareEventDispatcher.h"
#include "cEmpireWarfare.h"
#include "cWarfareEventListener.h"
#include "cArchetypeStrengthConfig.h"

#define cWarfareSystemPtr intrusive_ptr<cWarfareSystem>

///
/// In your dllmain Initialize method, add the system like this:
/// ModAPI::AddSimulatorStrategy(new cWarfareSystem(), cWarfareSystem::NOUN_ID);
///

/// Main class of the warfare system.
/// Coordinates warfare cycles and subcycles, instantiates and owns
/// the other warfare-related subsystem classes, and manages
/// cEmpireWarfare objects for all empires in range.
class cWarfareSystem
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("Ai_Empire_Diplomacy::cWarfareSystem");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;
	int Release() override;
	void Initialize() override;
	void Dispose() override;
	const char* GetName() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;
	void Update(int deltaTime, int deltaGameTime) override;
	bool WriteToXML(Simulator::XmlSerializer* xml) override;
	void OnModeEntered(uint32_t previousModeID, uint32_t newModeID) override;
	void OnModeExited(uint32_t previousModeID, uint32_t newModeID) override;
	static Simulator::Attribute ATTRIBUTES[];

	/// @brief Executes one subcycle of the warfare system.
	/// Processes the next empire in the queue and manages its warfare actions.
	void WarfareSubCycle();

	/// @brief Starts a new warfare cycle.
	/// Initializes the list of empires, resets timers, and distributes empires across subcycles.
	void StartWarfareCycle();



private:
	
	// Key used to load the warfare config prop.
	ResourceKey warfareConfigKey;

	// Pointer to the loaded warfareConfig.
	cWarfareConfigPtr warfareConfig;

	// Key used to load the space combat prop.
	ResourceKey spaceCombatKey;

	// Pointer to the loaded spaceCombatMetrics.
	cSpaceCombatMetricsPtr spaceCombatMetrics;

	// Pointer to the loaded warfareStrengthAnalyzer.
	cWarfareStrengthAnalyzerPtr warfareStrengthAnalyzer;

	// Pointer to the loaded warfareEventDispatcher.
	cWarfareEventDispatcherPtr warfareEventDispatcher;

	// Pointer to the loaded warfareEventListener.
	cWarfareEventListenerPtr warfareEventListener;

	// Key used to load the archetypeStrengthConfig prop.
	ResourceKey archetypeStrengthConfigKey;

	// Pointer to the loaded ArchetypeStrengthConfig
	cArchetypeStrengthConfigPtr archetypeStrengthConfig;

	// Actives empireWarfare;
	eastl::vector<cEmpireWarfarePtr> empiresWarfare;

	// Iterator to the next empire whose warfare will be managed..
	eastl::vector<cEmpireWarfarePtr>::iterator empireToManage;

	// Time passed (in miliseconds) since the cycle has started.
	int elapsedTime;

	// Miliseconds of gameTime between expansion cycles.
	int cycleInterval;

	// Number of empires evaluated per subcycle.
	int empiresPerSubCycle;

	// Timestamp (in milliseconds) of the next subcycle execution.
	int nextSubcycleTime;

	// Minimum time (in milliseconds) that must pass between subcycles.
	int minSubcycleStep;

	// Time (in milliseconds) that will pass between subcycles.
	int subcycleStep;

	// Radius (in parsecs) in which the manager is active.
	float activeRadius;

	int cycle;

};