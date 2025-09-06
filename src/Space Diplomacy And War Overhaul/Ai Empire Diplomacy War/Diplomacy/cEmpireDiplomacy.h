#pragma once

#include <Spore\BasicIncludes.h>
#include "cDiplomacyConfig.h"
#include "cEmpireRelationsAnalyzer.h"
#include "cDiplomacyEventDispatcher.h"

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

	/// @brief Returns an ally's enemy empire, if any.
	/// @return A pointer to the enemy empire, or nullptr if none exists.
	Simulator::cEmpire* GetAllyEnemy();

	/// @brief Calculates the probability of forming an alliance with the target empire.
	/// @param target.
	/// @return Probability value in the range [0,1].
	float AllianceProbability(Simulator::cEmpire* target);

	/// @brief Calculates the probability of breaking an alliance with the target empire.
	/// @param target.
	/// @return Probability value in the range [0,1].
	float BreakAllianceProbability(Simulator::cEmpire* target);

	/// @brief Calculates the probability of declaring war on the target empire.
	/// @param target.
	/// @return Probability value in the range [0,1].
	float DeclareWarProbability(Simulator::cEmpire* target);

	/// @brief Returns an empire selected to break an alliance, based on probability checks.
	/// @return A pointer to the target empire, or nullptr if none qualified.
	Simulator::cEmpire* GetBreakAllianceTarget();

	/// @brief Returns an empire selected as a war target, based on probability checks.
	/// @return A pointer to the target empire, or nullptr if none qualified.
	Simulator::cEmpire* GetWarTarget();

	/// @brief Returns an empire selected to form an alliance, based on probability checks.
	/// @return A pointer to the target empire, or nullptr if none qualified.
	Simulator::cEmpire* GetAllianceTarget();

	/// @brief Handles all logic related to allies.
	void ManageAllies();

	/// @brief Handles all logic related to enemies.
	void ManageEnemies();

	/// @brief Handles all logic related to neutral empires.
	void ManageNeutrals();

	/// @brief Handles overall diplomacy management.
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
