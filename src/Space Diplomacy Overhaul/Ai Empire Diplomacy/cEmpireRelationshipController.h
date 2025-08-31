#pragma once

#include <Spore\BasicIncludes.h>
#include <cDiplomacyEffectAnalyzer.h>

#define cEmpireRelationshipControllerPtr intrusive_ptr<cEmpireRelationshipController>

/// Provides controlled access to the RelationshipManager, 
/// applying increases decreases or decays from diplomacy effects while respecting 
/// per–effects caps.
class cEmpireRelationshipController 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cEmpireRelationshipController");
	
	cEmpireRelationshipController(cDiplomacyEffectAnalyzer* diplomacyEffectAnalyzer);
	~cEmpireRelationshipController();

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



	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	// Pointer to the loaded diplomacy effect analyzer.
	cDiplomacyEffectAnalyzerPtr diplomacyEffectAnalyzer;
};
