#pragma once

#include <Spore\BasicIncludes.h>
#include <cDiplomacyConfig.h>
#include <cEmpireRelationsAnalyzer.h>
#include <cDiplomacyEventDispatcher.h>

#define cEmpireDiplomacyPtr intrusive_ptr<cEmpireDiplomacy>


/// Handles diplomatic decisions for an empire.
/// This class decides alliances, wars, and alliance breakups, 
/// for a given empire.
class cEmpireDiplomacy 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cEmpireDiplomacy");
	
	cEmpireDiplomacy(Simulator::cEmpire* empire, 
		cDiplomacyConfig* diplomacyConfig, 
		cEmpireRelationsAnalyzer* empireRelationsAnalyzer, 
		cDiplomacyEventDispatcher* diplomacyEventDispatcher);
	~cEmpireDiplomacy();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/**
	 * @brief Resolves conflicts between allied empires.
	 * This function checks for cases where two allies are at war with each other.
	 * When such a conflict is detected, it breaks the alliance with the ally
	 * that has the lower affinity.
	 */
	void ResolveAlliesWar();

	float AllianceProbability(Simulator::cEmpire* target);

	float BreakAllianceProbability(Simulator::cEmpire* target);

	float DeclareWarProbability(Simulator::cEmpire* target);

	Simulator::cEmpire* GetBreakAllianceTarget();

	Simulator::cEmpire* GetWarTarget();

	Simulator::cEmpire* GetAllianceTarget();

	void ManageAllies();

	void ManageEnemies();

	void ManageNeutrals();

	void ManageDiplomacy();

	// Pointer to the empire this object is managing.
	cEmpirePtr empire;

	// Pointer to the loaded diplomacy configuration object.
	cDiplomacyConfigPtr diplomacyConfig;

	// Pointer to the loaded empire relations analyzer object.
	cEmpireRelationsAnalyzerPtr empireRelationsAnalyzer;

	cDiplomacyEventDispatcherPtr diplomacyEventDispatcher;

	eastl::vector<cEmpirePtr> neutrals;

	int strenght;
	int srenghtOfAlliance;
};
