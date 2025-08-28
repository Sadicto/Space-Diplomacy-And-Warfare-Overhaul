#include "stdafx.h"
#include "cDiplomacyConfig.h"

cDiplomacyConfig::cDiplomacyConfig(ResourceKey configKey)
{
	PropertyListPtr configProp;
	PropManager.GetPropertyList(configKey.instanceID, configKey.groupID, configProp);

	App::Property::GetArrayFloat(configProp.get(), 0xE9F5A508, diplomacyRange);

	App::Property::GetInt32(configProp.get(), 0x74F51B4B, maxAffinitySoftCap);

	App::Property::GetInt32(configProp.get(), 0x63812B01, minAffinitySoftCap);

	App::Property::GetFloat(configProp.get(), 0x38D9CD5F, maxAllianceProbability);

	App::Property::GetFloat(configProp.get(), 0xDCFF754A, maxWarProbability);

	App::Property::GetInt32(configProp.get(), 0xDB025009, affinityThresholdForStableAlliance);

	App::Property::GetInt32(configProp.get(), 0x07DB4B1C, affinityThresholdForUnstableAlliance);

	App::Property::GetInt32(configProp.get(), 0x22472FA3, affinityThresholdForWar);

	App::Property::GetArrayInt32(configProp.get(), 0x8DA36269, baseAggressivityByArchetype);

	App::Property::GetArrayInt32(configProp.get(), 0x51A5A369, aggressivityGrowthByPowerLevel);

	App::Property::GetBool(configProp.get(), 0xC1C6CE47, autoDeclareWarOnAllyEnemies);

	App::Property::GetBool(configProp.get(), 0xCC04E149, startsWarsWhileAtWar);

	App::Property::GetFloat(configProp.get(), 0x4BB58401, powerThresholdToAvoidWar);
}


cDiplomacyConfig::~cDiplomacyConfig()
{
}

// For internal use, do not modify.
int cDiplomacyConfig::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cDiplomacyConfig::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cDiplomacyConfig::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cDiplomacyConfig);
	return nullptr;
}
