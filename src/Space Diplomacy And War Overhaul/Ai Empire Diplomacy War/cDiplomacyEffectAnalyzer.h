#pragma once

#include <Spore\BasicIncludes.h>
#include <cDiplomacyEffectInfoProvider.h>

#define cDiplomacyEffectAnalyzerPtr intrusive_ptr<cDiplomacyEffectAnalyzer>

/// Provides detailed information about a diplomacy event based on its metadata.
class cDiplomacyEffectAnalyzer 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cDiplomacyEffectAnalyzer");
	
	cDiplomacyEffectAnalyzer(cDiplomacyEffectInfoProvider* diplomacyEfectInfoProvider);
	~cDiplomacyEffectAnalyzer();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Returns the maximum scale that can be applied for the given diplomacy effect
	/// based on its ID and the current event value.
	/// @param effectID.
	/// @param currentValue The current value of the event.
	/// @param forceNextMax If true, increases the returned max scale enough so that
	/// (if near the max) in the next vanilla diplomacy cycle the effect will remain at its maximum.
	/// @return Maximum scale value that can be applied without exceeding limits.
	float GetMaxScale(uint32_t effectID, float currentValue, bool forceNextMax = false);

	/// @brief Returns the maximum decay that can be applied for the given diplomacy effect
	/// based on its ID and the current event value.
	/// @param effectID.
	/// @param currentValue The current value of the event.
	/// @return Maximum decay value that can be applied without exceeding limits.
	float GetMaxDecay(uint32_t effectID, float currentValue);

	/// @brief Returns the scale required to raise the diplomacy effect to its maximum value.
	/// @param effectID.
	/// @param currentValue The current value of the event.
	/// @return Scale value needed to reach the maximum.
	float GetScaleToMax(uint32_t effectID, float currentValue);

	/// @brief Returns the scale required to reduce the diplomacy effect to zero.
	/// @param effectID.
	/// @param currentValue The current value of the event.
	/// @return Scale value needed to reach zero.
	float GetScaleToZero(uint32_t effectID, float currentValue);


	// Pointer to the loaded diplomacy effect info provider.
	cDiplomacyEffectInfoProviderPtr diplomacyEffectInfoProvider;
};
