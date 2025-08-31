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

	void ApplyRelationshipEffect(uint32_t politicalID, uint32_t causePoliticalID, uint32_t effectID);

	void DecayRelationshipEffect(uint32_t politicalID, uint32_t causePoliticalID, uint32_t effectID);

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	// Pointer to the loaded diplomacy effect analyzer.
	cDiplomacyEffectAnalyzerPtr diplomacyEffectAnalyzer;
};
