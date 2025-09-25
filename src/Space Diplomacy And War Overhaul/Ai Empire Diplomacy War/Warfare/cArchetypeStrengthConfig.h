#pragma once

#include <Spore\BasicIncludes.h>

#define cArchetypeStrengthConfigPtr intrusive_ptr<cArchetypeStrengthConfig>

// Loads archetype strength configuration files and provides methods
// to retrieve base and bonus strength values for each archetype.
class cArchetypeStrengthConfig 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cArchetypeStrengthConfig");
	
	cArchetypeStrengthConfig(ResourceKey archetypeStrengthConfiggKey);
	~cArchetypeStrengthConfig();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Returns the base strength multiplier for the specified archetype.
	/// @param archetype.
	/// @return The base strength multiplier.
	float GetArchetypeBaseStrength(Simulator::Archetypes archetype);

	/// @brief Returns the bonus strength value for the specified archetype.
	/// @param archetype.
	/// @return The bonus strength value.
	float GetArchetypeBonusStrength(Simulator::Archetypes archetype);


	// Base strength multipliers for each archetype.
	eastl::vector<float> archetypeBaseStrengths;

	// Additional strength bonuses for each archetype.
	eastl::vector<float> archetypeBonusStrengths;
};
