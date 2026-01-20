#include "stdafx.h"
#include "cAffinityConfig.h"

cAffinityConfig::cAffinityConfig(ResourceKey affinityConfigKey)
{
	PropertyListPtr affinityConfig;

	PropManager.GetPropertyList(affinityConfigKey.instanceID, affinityConfigKey.groupID, affinityConfig);

	App::Property::GetInt32(affinityConfig.get(), 0x57748147, numAffinityModifiers);

	App::Property::GetArrayBool(affinityConfig.get(), 0xE48692DE, upgrades);

	App::Property::GetArrayUInt32(affinityConfig.get(), 0x18E7CB64, upgradeTime);

	App::Property::GetArrayBool(affinityConfig.get(), 0xA025DC24, decays);

	App::Property::GetArrayUInt32(affinityConfig.get(), 0x7D4D7362, decayTime);

	App::Property::GetArrayBool(affinityConfig.get(), 0xD92A2F80, preventWars);

	App::Property::GetArrayInt32(affinityConfig.get(), 0x04E9E9F8, affinityGain);

	App::Property::GetArrayInt32(affinityConfig.get(), 0x393F7F7D, priority);

	App::Property::GetArrayBool(affinityConfig.get(), 0xA90AAD3E, stableRelationsMutuallyExclusive);

	App::Property::GetArrayBool(affinityConfig.get(), 0x7DC46842, warTogetherMutuallyExclusive);
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

int cAffinityConfig::getNumAffinityModifiers(){
	return numAffinityModifiers;
}

bool cAffinityConfig::AffinityUpgrades(AffinityModifier affinityModifier){
	return upgrades[int(affinityModifier)];
}

uint32_t cAffinityConfig::GetUpgradeTime(AffinityModifier affinityModifier){
	return upgradeTime[int(affinityModifier)];
}

bool cAffinityConfig::AffinityDecays(AffinityModifier affinityModifier){
	return decays[int(affinityModifier)];
}

uint32_t cAffinityConfig::GetDecayTime(AffinityModifier affinityModifier){
	return decayTime[int(affinityModifier)];
}

bool cAffinityConfig::AffinityPreventsWars(AffinityModifier affinityModifier){
	return preventWars[int(affinityModifier)];
}

int cAffinityConfig::GetAffinityGain(AffinityModifier affinityModifier){
	return affinityGain[int(affinityModifier)];
}

int cAffinityConfig::GetAffinityPriority(AffinityModifier affinityModifier){
	return priority[int(affinityModifier)];
}

bool cAffinityConfig::StableRelationsMutuallyExclusive(AffinityModifier affinityModifier){
	return stableRelationsMutuallyExclusive[int(affinityModifier)];
}

bool cAffinityConfig::WarTogetherMutuallyExclusive(AffinityModifier affinityModifier){
	return warTogetherMutuallyExclusive[int(affinityModifier)];
}
