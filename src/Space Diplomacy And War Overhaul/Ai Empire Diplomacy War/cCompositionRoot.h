#pragma once

#include <Spore\BasicIncludes.h>
#include <Diplomacy/cDiplomacyEffectInfoProvider.h>
#include <Diplomacy/cDiplomacyConfig.h>
#include <Diplomacy/cEmpireRelationsAnalyzer.h>
#include <Diplomacy/cDiplomacyEventDispatcher.h>
#include <Diplomacy/cDiplomacyEventListener.h>
#include <Diplomacy/cEmpireDiplomacyFactory.h>
#include <Warfare/cWarfareConfig.h>
#include <Warfare/cSpaceCombatMetrics.h>
#include <Warfare/cWarfareStrengthAnalyzer.h>
#include <Warfare/cWarfareEventDispatcher.h>
#include <Warfare/cWarfareEventListener.h>
#include <Diplomacy/cDiplomacySystem.h>
#include <Warfare/cWarfareSystem.h>
#include <Warfare/cEmpireWarfareFactory.h>

#define cCompositionRootPtr intrusive_ptr<cCompositionRoot>

///
/// In your dllmain Initialize method, add the system like this:
/// ModAPI::AddSimulatorStrategy(new cCompositionRoot(), cCompositionRoot::NOUN_ID);
///

/// Singleton class that handles dependency injection
/// for all other classes in the mod.
class cCompositionRoot
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cCompositionRoot");
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

	static cCompositionRoot* Get();

	static Simulator::Attribute ATTRIBUTES[];



private:
	
	static cCompositionRoot* instance;

	// Pointer to the loaded diplomacy system object.
	// it's not created by the composition root.
	cDiplomacySystemPtr diplomacySystem;

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

	// Pointer to the loaded empire diplomacy factory.
	cEmpireDiplomacyFactoryPtr empireDiplomacyFactory;

	// Pointer to the loaded diplomacy event listener object.
	cDiplomacyEventListenerPtr diplomacyEventListener;

	// Key used to load the popups texts prop.
	ResourceKey spacePopUpsTextsKey;

	// Key used to load the popups filter config.
	ResourceKey popupsFilterConfigKey;

	// Pointer to the loaded diplomacy popup manager.
	cDiplomacyPopupManagerPtr diplomacyPopUpManager;

	// Pointer to the loaded empire relationship controller.
	cEmpireRelationshipControllerPtr empireRelationshipController;

	// Pointer to the loaded diplomacy effect analyzer.
	cDiplomacyEffectAnalyzerPtr diplomacyEffectAnalyzer;

	// Key used to load the relationship effects prop.
	ResourceKey relationshipEffectsKey;

	// Pointer to the loaded diplomacy effect info provider.
	cDiplomacyEffectInfoProviderPtr diplomacyEffectInfoProvider;

	// Pointer to the loaded warfare system.
	// It's not created by the composition root.
	cWarfareSystemPtr warfareSystem;

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

	// Pointer to the loaded empireWarfareFactory;
	cEmpireWarfareFactoryPtr empireWarfareFactory;

	// Pointer to the loaded warfareEventListener.
	cWarfareEventListenerPtr warfareEventListener;

	// Key used to load the archetypeStrengthConfig prop.
	ResourceKey archetypeStrengthConfigKey;

	// Pointer to the loaded ArchetypeStrengthConfig
	cArchetypeStrengthConfigPtr archetypeStrengthConfig;

	// Pointer to the loaded affinityLayout.
	UTFWin::UILayout* affinityLayout;

	// Key used to load the affinityTextConfig.
	ResourceKey affinityTextConfigKey;
};