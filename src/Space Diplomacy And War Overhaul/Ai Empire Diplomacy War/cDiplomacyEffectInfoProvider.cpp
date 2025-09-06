#include "stdafx.h"
#include "cDiplomacyEffectInfoProvider.h"

cDiplomacyEffectInfoProvider::cDiplomacyEffectInfoProvider(ResourceKey effectsKey)
{
	PropManager.GetPropertyList(effectsKey.instanceID, effectsKey.groupID, effectsProp);
}


cDiplomacyEffectInfoProvider::~cDiplomacyEffectInfoProvider()
{
}

// For internal use, do not modify.
int cDiplomacyEffectInfoProvider::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cDiplomacyEffectInfoProvider::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cDiplomacyEffectInfoProvider::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cDiplomacyEffectInfoProvider);
	return nullptr;
}

bool cDiplomacyEffectInfoProvider::GetEffectInfo(uint32_t effectID, eastl::vector<float>& effectInfo) {
	return App::Property::GetArrayFloat(effectsProp.get(), effectID, effectInfo);
}

float cDiplomacyEffectInfoProvider::GetEffectZeroBonus(const eastl::vector<float>& effectInfo)
{
	return effectInfo.size() > 0 ? effectInfo[0] : 0.0f;
}

float cDiplomacyEffectInfoProvider::GetEffectMagnitude(const eastl::vector<float>& effectInfo)
{
	return effectInfo.size() > 1 ? effectInfo[1] : 0.0f;
}

float cDiplomacyEffectInfoProvider::GetEffectMin(const eastl::vector<float>& effectInfo)
{
	return effectInfo.size() > 2 ? effectInfo[2] : 0.0f;
}

float cDiplomacyEffectInfoProvider::GetEffectMax(const eastl::vector<float>& effectInfo)
{
	return effectInfo.size() > 3 ? effectInfo[3] : 0.0f;
}

float cDiplomacyEffectInfoProvider::GetEffectDecay(const eastl::vector<float>& effectInfo)
{
	return effectInfo.size() > 4 ? effectInfo[4] : 0.0f;
}