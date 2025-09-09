#pragma once

#include <Spore\BasicIncludes.h>
#include "cEmpireDiplomacy.h"
#include "cDiplomacyConfig.h"
#include "cArchetypesConfig.h"
#include "cEmpireRelationsAnalyzer.h"
#include <Spore\Simulator\SubSystem\SimulatorSystem.h>
#include "cDiplomacyEventDispatcher.h"
#include "cDiplomacyEventListener.h"
#include "cDiplomacyPopupManager.h"
#include "cDiplomacyEffectAnalyzer.h"
#include "cEmpireRelationshipController.h"


#define cDiplomacySystemPtr intrusive_ptr<cDiplomacySystem>

///
/// In your dllmain Initialize method, add the system like this:
/// ModAPI::AddSimulatorStrategy(new cDiplomacySystem(), cDiplomacySystem::NOUN_ID);
///

/// Main class of the diplomatic system.
/// Instantiate the class of the system when
/// entering space stage and controls
/// the diplomatic cycles, creating cEmpireDiplomacy 
/// objects acording to the number of empires to
/// manage.
class cDiplomacySystem
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("HarderSpaceDiplomacy::cDiplomacySystem");
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

	//
	// You can add more methods here
	//

	static cDiplomacySystem* Get();

	void ManageEmpireDiplomacy(Simulator::cEmpire* empire);

	void EmpireDiplomacyCycle();

private:
	//
	// You can add members here.
	//

	static cDiplomacySystem* instance;

	// Key used to load the diplomacy configuration prop.
	ResourceKey diplomacyConfigKey;

	// Pointer to the loaded diplomacy configuration object.
	cDiplomacyConfigPtr diplomacyConfig;

	// Key used to load the archetypes affinities prop.
	ResourceKey archetypesAffinitiesKey;

	// Key used to load the archetypes agressivities prop.
	ResourceKey archetypesAgressivitiesKey;

	// Pointer to the loaded archetypes affinities object.
	cArchetypesConfigPtr archetypesConfig;

	// Pointer to the loaded empire relations analyzer object.
	cEmpireRelationsAnalyzerPtr empireRelationsAnalyzer;

	// Pointer to the loaded diplomacy event dispatcher object.
	cDiplomacyEventDispatcherPtr diplomacyEventDispatcher;

	// Pointer to the loaded diplomacy event listener object.
	cDiplomacyEventListenerPtr diplomacyEventListener;

	// Pointer to the loaded diplomacy popup manager.
	cDiplomacyPopupManagerPtr diplomacyPopUpManager;

	// Key used to load the popups texts prop.
	ResourceKey spacePopUpsTextsKey;

	// Pointer to the loaded empire relationship controller.
	cEmpireRelationshipControllerPtr empireRelationshipController;

	// Pointer to the loaded diplomacy effect analyzer.
	cDiplomacyEffectAnalyzerPtr diplomacyEffectAnalyzer;

	// Key used to load the relationship effects prop.
	ResourceKey relationshipEffectsKey;

	// Pointer to the loaded diplomacy effect info provider.
	cDiplomacyEffectInfoProviderPtr diplomacyEffectInfoProvider;

	// Actives empireDiplomacy;
	eastl::vector<cEmpireDiplomacyPtr> empiresDiplomacy;

	// Time passed (in miliseconds) since the cycle has started.
	int elapsedTime;

	// Miliseconds of gameTime between expansion cycles.
	int cycleInterval;

	// Radius (in parsecs) in which the manager is active.
	float activeRadius;

	int cycle;
};