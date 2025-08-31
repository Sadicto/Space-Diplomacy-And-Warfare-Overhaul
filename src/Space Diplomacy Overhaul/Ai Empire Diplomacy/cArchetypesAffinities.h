#pragma once

#include <Spore\BasicIncludes.h>

#define cArchetypesAffinitiesPtr intrusive_ptr<cArchetypesAffinities>

/// Stores and provides access to affinity values between different archetypes.
class cArchetypesAffinities 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cArchetypesAffinities");
	
	cArchetypesAffinities(ResourceKey archetypesAffinitiesKey);
	~cArchetypesAffinities();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Gets the affinity value between two archetypes.
	/// @param archetype1.
	/// @param archetype2.
	/// @return The affinity value between the two archetypes.
	int GetArchetypesAffinity(Simulator::Archetypes archetype1, Simulator::Archetypes archetype2);




	// An 8x8 matrix storing affinity values between archetypes.
	eastl::vector<eastl::vector<int>> archetypesAffinities;
};
