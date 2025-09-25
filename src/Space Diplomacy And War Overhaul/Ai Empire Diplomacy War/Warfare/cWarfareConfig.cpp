#include "stdafx.h"
#include "cWarfareConfig.h"

cWarfareConfig::cWarfareConfig(ResourceKey warfareConfigKey)
{
	PropertyListPtr warfareConfigProp;
	PropManager.GetPropertyList(warfareConfigKey.instanceID, warfareConfigKey.groupID, warfareConfigProp);

	App::Property::GetArrayFloat(warfareConfigProp.get(), 0x4BC711F0, warfareRange);

	App::Property::GetFloat(warfareConfigProp.get(), 0xD07B1F69, bombersPerRedSpiceFactory);

	App::Property::GetInt32(warfareConfigProp.get(), 0x140FD2AA, buildingsPerFactoryRatio);

	App::Property::GetInt32(warfareConfigProp.get(), 0xC30095A4, citiesInHomeworld);

	App::Property::GetInt32(warfareConfigProp.get(), 0x9A2EE896, maxBombersInRaid);

	PropertyListPtr terraformTuningProp;
	PropManager.GetPropertyList(0xa6d1bc70, 0x02ae0c7e, terraformTuningProp);

	App::Property::GetInt32(terraformTuningProp.get(), 0x04A5D473, buildingsInT1Colony);
	App::Property::GetInt32(terraformTuningProp.get(), 0x04A5D46D, buildingsInT2Colony);
	App::Property::GetInt32(terraformTuningProp.get(), 0x052F046E, buildingsInT3Colony);

}


cWarfareConfig::~cWarfareConfig()
{
}

// For internal use, do not modify.
int cWarfareConfig::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cWarfareConfig::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cWarfareConfig::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cWarfareConfig);
	return nullptr;
}

float cWarfareConfig::GetWarfareRange(int empireLevel)
{
	return warfareRange[empireLevel];
}

float cWarfareConfig::GetBombersPerRedSpiceFactory()
{
	return bombersPerRedSpiceFactory;
}

int cWarfareConfig::GetBuildingsPerFactoryRatio()
{
	return buildingsPerFactoryRatio;
}

int cWarfareConfig::GetBuildingsInT1Colony()
{
	return buildingsInT1Colony;
}

int cWarfareConfig::GetBuildingsInT2Colony()
{
	return buildingsInT2Colony;
}

int cWarfareConfig::GetBuildingsInT3Colony()
{
	return buildingsInT3Colony;
}


int cWarfareConfig::GetCitiesInHomeworld()
{
	return citiesInHomeworld;
}

int cWarfareConfig::GetMaxBombersInRaid() {
	return maxBombersInRaid;
}

