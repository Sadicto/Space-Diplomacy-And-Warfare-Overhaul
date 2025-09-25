#pragma once

#include <Spore\BasicIncludes.h>

#define cWarfareConfigPtr intrusive_ptr<cWarfareConfig>

// Stores all global configuration data for the warfare system,
// including aggression ranges, production rates, and other tunable parameters.
class cWarfareConfig
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cWarfareConfig");

	cWarfareConfig(ResourceKey warfareConfigKey);
	~cWarfareConfig();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Gets the warfare range for the given empire level, in parsecs.
	float GetWarfareRange(int empireLevel);

	// Returns the number of bombers produced per factory on a planet with red spice.
	float GetBombersPerRedSpiceFactory();

	// Returns the ratio of buildings per factory.
	int GetBuildingsPerFactoryRatio();

	// Returns the number of buildings in a Tier 1 colony.
	int GetBuildingsInT1Colony();

	// Returns the number of buildings in a Tier 2 colony.
	int GetBuildingsInT2Colony();

	// Returns the number of buildings in a Tier 3 colony.
	int GetBuildingsInT3Colony();

	// Returns the number of cities on the empire's homeworld.
	int GetCitiesInHomeworld();

	// Returns the maximum number of bombers allowed in a raid.
	int GetMaxBombersInRaid();

	// Vector storing the warfare range for different empire levels.
	eastl::vector<float> warfareRange;

	// Number of bombers produced per factory on a planet with red spice.
	float bombersPerRedSpiceFactory;

	// Ratio of buildings per factory.
	int buildingsPerFactoryRatio;

	// Number of buildings in a T1 colony.
	int buildingsInT1Colony;

	// Number of buildings in a T1 colony.
	int buildingsInT2Colony;

	// Number of buildings in a T1 colony.
	int buildingsInT3Colony;

	// Number of cities on the empire's homeworld.
	int citiesInHomeworld;

	// Max bombers per raid.
	int maxBombersInRaid;

};
