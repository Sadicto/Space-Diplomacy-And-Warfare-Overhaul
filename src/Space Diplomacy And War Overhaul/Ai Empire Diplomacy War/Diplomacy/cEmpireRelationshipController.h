#pragma once

#include <Spore\BasicIncludes.h>
#include "cDiplomacyEffectAnalyzer.h"

#define cEmpireRelationshipControllerPtr intrusive_ptr<cEmpireRelationshipController>

/// Provides controlled access to the RelationshipManager, 
/// applying increases decreases or decays from diplomacy effects while respecting 
/// per–effects caps.
class cEmpireRelationshipController 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cEmpireRelationshipController");
	
	cEmpireRelationshipController(ResourceKey archetypesRelationshipEffects, cDiplomacyEffectAnalyzer* diplomacyEffectAnalyzer);
	~cEmpireRelationshipController();

	/// @brief Calculates the relationship value between two empires.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire.
	/// @return A float representing the relationship value between the two empires.
	float GetEmpiresRelationship(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Applies the specified relationship effect between two political entities.
	/// @param politicalID.
	/// @param causePoliticalID.
	/// @param effectID The ID of the relationship effect.
	/// @return The scale value that was actually applied.
	float ApplyRelationshipEffect(uint32_t politicalID, uint32_t causePoliticalID, uint32_t effectID);

	/// @brief Applies the decay for the specified relationship effect between two political entities.
	/// @param politicalID.
	/// @param causePoliticalID.
	/// @param effectID.
	/// @return The decay value that was actually applied.
	float DecayRelationshipEffect(uint32_t politicalID, uint32_t causePoliticalID, uint32_t effectID);

	/// @brief Sets the specified relationship effect between two political entities to its maximum value.
	/// @param politicalID.
	/// @param causePoliticalID.
	/// @param effectID.
	/// @return The scale value that was applied to reach the maximum.
	float SetRelationshipEffectMax(uint32_t politicalID, uint32_t causePoliticalID, uint32_t effectID);

	/// @brief Sets the specified relationship effect between two political entities to zero.
	/// @param politicalID.
	/// @param causePoliticalID.
	/// @param effectID.
	/// @return The scale value that was applied to reset the effect to zero.
	float SetRelationshipEffectZero(uint32_t politicalID, uint32_t causePoliticalID, uint32_t effectID);

	/// @brief Gets the modifier applied to an empire's relationship based on its archetype.
	/// @param empire
	/// @return A float representing the relationship modifier of the empire.
	float GetPersonalityEffect(Simulator::cEmpire* empire);

	/// @brief Gets the modifier applied to the relationship between two empires based on their archetypes.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire.
	/// @return A float representing the relationship modifier between the two empires.
	float GetRelationshipArchetypeModifier(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Applies the modifier applied to the relationship between two empires based on the archetype of the second empire.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire, its archetype will be used for the personality effect.
	void ApplyPersonalityEffect(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Applies the modifier the relationship between two empires based on their archetypes.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire.
	void ApplyArchetypeModifier(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Reset the relationship between the empires (eliminating all relationship effects) then reapplies the archetype and personality effects.
	/// @param empire1 The first empire.
	/// @param empire2 The second empire, its archetype will be utilized for the personality effect.
	void ResetRelationship(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);



	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	// Pointer to the loaded diplomacy effect analyzer.
	cDiplomacyEffectAnalyzerPtr diplomacyEffectAnalyzer;

	// Vector containing the archetype personality effects.
	eastl::vector<float> archetypePersonalityEffects;

	// Vector of vectors containing the archetype modifiers between archetypes.
	eastl::vector<eastl::vector<float>> relationshipArchetypeModifiers;

};
