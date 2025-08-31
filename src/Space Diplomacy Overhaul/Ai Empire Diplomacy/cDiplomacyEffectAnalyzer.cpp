#include "stdafx.h"
#include "cDiplomacyEffectAnalyzer.h"

cDiplomacyEffectAnalyzer::cDiplomacyEffectAnalyzer(cDiplomacyEffectInfoProvider* diplomacyEfectInfoProvider)
{
	this->diplomacyEffectInfoProvider = diplomacyEfectInfoProvider;
}


cDiplomacyEffectAnalyzer::~cDiplomacyEffectAnalyzer()
{
}

// For internal use, do not modify.
int cDiplomacyEffectAnalyzer::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cDiplomacyEffectAnalyzer::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cDiplomacyEffectAnalyzer::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cDiplomacyEffectAnalyzer);
	return nullptr;
}

float cDiplomacyEffectAnalyzer::GetMaxScale(uint32_t effectID, float currentValue, bool forceNextMax) {
	eastl::vector<float> effectInfo;
	if (diplomacyEffectInfoProvider->GetEffectInfo(effectID, effectInfo)) {

		float effectMax = 0.0f;
		float effectBaseMagnitude = diplomacyEffectInfoProvider->GetEffectMagnitude(effectInfo);
		bool positiveEffect = effectBaseMagnitude > 0.0f;

		if (positiveEffect) {
			effectMax = diplomacyEffectInfoProvider->GetEffectMax(effectInfo);
		}
		else {
			effectMax = diplomacyEffectInfoProvider->GetEffectMin(effectInfo);
		}
		float normalizedEffectMax = effectMax / effectBaseMagnitude;

		if (forceNextMax) {
			float effectDecay = diplomacyEffectInfoProvider->GetEffectDecay(effectInfo);
			normalizedEffectMax += abs(effectDecay);
		}
		return min(1.0f, normalizedEffectMax - currentValue);
	}
	return 0.0f;
}

float cDiplomacyEffectAnalyzer::GetMaxDecay(uint32_t effectID, float currentValue) {
	eastl::vector<float> effectInfo;
	if (diplomacyEffectInfoProvider->GetEffectInfo(effectID, effectInfo)) {
		float effectDecay = diplomacyEffectInfoProvider->GetEffectDecay(effectInfo);
		return max(effectDecay, -abs(currentValue));
	}
	return 0.0f;
}

float cDiplomacyEffectAnalyzer::GetScaleToMax(uint32_t effectID, float currentValue) {
	eastl::vector<float> effectInfo;
	if (diplomacyEffectInfoProvider->GetEffectInfo(effectID, effectInfo)) {
		float effectMax = 0.0f;
		float effectBaseMagnitude = diplomacyEffectInfoProvider->GetEffectMagnitude(effectInfo);
		bool positiveEffect = effectBaseMagnitude > 0.0f;

		if (positiveEffect) {
			effectMax = diplomacyEffectInfoProvider->GetEffectMax(effectInfo);
		}
		else {
			effectMax = diplomacyEffectInfoProvider->GetEffectMin(effectInfo);
		}
		float normalizedEffectMax = effectMax / effectBaseMagnitude;
		return normalizedEffectMax - currentValue;

	}
	return 0.0f;
}

float cDiplomacyEffectAnalyzer::GetScaleToZero(uint32_t effectID, float currentValue) {
	return -currentValue;
}

