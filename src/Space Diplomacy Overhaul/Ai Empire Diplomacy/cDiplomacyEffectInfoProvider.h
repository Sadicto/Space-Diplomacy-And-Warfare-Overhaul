#pragma once

#include <Spore\BasicIncludes.h>

#define cDiplomacyEffectInfoProviderPtr intrusive_ptr<cDiplomacyEffectInfoProvider>

/// Provides access to diplomacy effect info
/// and exposes helper functions to query individual parameters.
class cDiplomacyEffectInfoProvider 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cDiplomacyEffectInfoProvider");
	
	cDiplomacyEffectInfoProvider(App::PropertyList* effectsProp);
	~cDiplomacyEffectInfoProvider();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Retrieves the effectInfo vector for a given diplomacy effect.
	/// @param effect The ID of the diplomacy effect to query.
	/// @param effectInfo dst.
	/// @return true if the effectInfo was retrieved successfully, false otherwise.
	bool GetEffectInfo(uint32_t effectID, eastl::vector<float>& effectInfo);

	/// @brief Returns the bonus applied when the effect value is zero.
	/// @param effectInfo The effectInfo vector of the diplomacy effect.
	/// @return The zero-value bonus.
	float GetEffectZeroBonus(const eastl::vector<float>& effectInfo);

	/// @brief Returns the main magnitude of the diplomacy effect.
	/// @param effectInfo The effectInfo vector of the diplomacy effect.
	/// @return The magnitude value.
	float GetEffectMagnitude(const eastl::vector<float>& effectInfo);

	/// @brief Returns the minimum allowed value for the effect.
	/// @param effectInfo The effectInfo vector of the diplomacy effect.
	/// @return The minimum value.
	float GetEffectMin(const eastl::vector<float>& effectInfo);

	/// @brief Returns the maximum allowed value for the effect.
	/// @param effectInfo The effectInfo vector of the diplomacy effect.
	/// @return The maximum value.
	float GetEffectMax(const eastl::vector<float>& effectInfo);

	/// @brief Returns the decay rate applied to the effect when invoked.
	/// @param effectInfo The effectInfo vector of the diplomacy effect.
	/// @return The decay value.
	float GetEffectDecay(const eastl::vector<float>& effectInfo);


	// Pointer to property list in which the effects info are stored.
	PropertyListPtr effectsProp;
};
