#include "stdafx.h"
#include "cAffinityConfig.h"

cAffinityConfig::cAffinityConfig(ResourceKey affinityConfigKey)
{
	PropertyListPtr affinityConfig;

	PropManager.GetPropertyList(affinityConfigKey.instanceID, affinityConfigKey.groupID, affinityConfig);

	App::Property::GetInt32(affinityConfig.get(), 0x57748147, numAffinityModifiers);

	App::Property::GetArrayBool(affinityConfig.get(), 0x44F772EB, expires);

	App::Property::GetArrayUInt32(affinityConfig.get(), 0x9BE6806B, expireTime);

	App::Property::GetArrayBool(affinityConfig.get(), 0xD92A2F80, preventWars);

	App::Property::GetArrayInt32(affinityConfig.get(), 0x04E9E9F8, affinityGain);

	App::Property::GetArrayBool(affinityConfig.get(), 0x4AE9BBD4, mutuallyExclusive);
}


cAffinityConfig::~cAffinityConfig()
{
}

// For internal use, do not modify.
int cAffinityConfig::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cAffinityConfig::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cAffinityConfig::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cAffinityConfig);
	return nullptr;
}

int cAffinityConfig::getNumAffinityModifiers()
{
	return numAffinityModifiers;
}

bool cAffinityConfig::AffinityExpires(AffinityModifier affinityModifier)
{
	return expires[int(affinityModifier)];
}

uint32_t cAffinityConfig::GetExpireTime(AffinityModifier affinityModifier)
{
	return expireTime[int(affinityModifier)];
}

bool cAffinityConfig::AffinityPreventsWars(AffinityModifier affinityModifier)
{
	return preventWars[int(affinityModifier)];
}

int cAffinityConfig::GetAffinityGain(AffinityModifier affinityModifier)
{
	return affinityGain[int(affinityModifier)];
}

bool cAffinityConfig::MutuallyExclusive(AffinityModifier affinityModifier)
{
	return mutuallyExclusive[int(affinityModifier)];
}
