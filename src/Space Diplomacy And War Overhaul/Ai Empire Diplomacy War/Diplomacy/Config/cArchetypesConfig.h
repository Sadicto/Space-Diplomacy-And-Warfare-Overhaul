#pragma once

#include <Spore\BasicIncludes.h>

#define cArchetypesConfigPtr intrusive_ptr<cArchetypesConfig>

/// Stores and provides access to affinity values between different archetypes.
class cArchetypesConfig 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cArchetypesConfig");
	
	cArchetypesConfig(ResourceKey archetypesAffinitiesKey, ResourceKey archetypesAgressivitiesKey);
	~cArchetypesConfig();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Gets the affinity value between two archetypes.
	/// @param archetype1.
	/// @param archetype2.
	/// @return The affinity value between the two archetypes.
	int GetArchetypesAffinity(Simulator::Archetypes archetype1, Simulator::Archetypes archetype2);

	/// @brief Gets the agressivity value of an archetype given it's power level.
	/// @param archetype
	/// @param powerLevel.
	/// @return The agressivity of the archetype with a given powerLevel.
	int GetArchetypeAgressivtyByPowerLevel(Simulator::Archetypes archetype, int powerLevel);




	// An 8x8 matrix storing affinity values between archetypes.
	eastl::vector<eastl::vector<int>> archetypesAffinities;

	// An 8x8 matrix storing agressivity values for archetypes.
	eastl::vector<eastl::vector<int>> archetypesAgressivities;
};
