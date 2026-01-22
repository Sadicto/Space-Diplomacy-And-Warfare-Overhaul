#include "stdafx.h"
#include "cPersistedDiplomacyEventConfig.h"

cPersistedDiplomacyEventConfig::cPersistedDiplomacyEventConfig(ResourceKey persistedDiplomacyEventConfig){
	PropertyListPtr persistedDiplomacyEventConfigProp;
	PropManager.GetPropertyList(persistedDiplomacyEventConfig.instanceID, persistedDiplomacyEventConfig.groupID, persistedDiplomacyEventConfigProp);

	App::Property::GetArrayBool(persistedDiplomacyEventConfigProp.get(), 0x44F772EB, expires);
	App::Property::GetArrayUInt32(persistedDiplomacyEventConfigProp.get(), 0x9BE6806B, expireTime);
	App::Property::GetArrayBool(persistedDiplomacyEventConfigProp.get(), 0xD8CECF0B, refreshedOnRepeat);
}


cPersistedDiplomacyEventConfig::~cPersistedDiplomacyEventConfig()
{
}

// For internal use, do not modify.
int cPersistedDiplomacyEventConfig::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cPersistedDiplomacyEventConfig::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cPersistedDiplomacyEventConfig::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cPersistedDiplomacyEventConfig);
	return nullptr;
}

bool cPersistedDiplomacyEventConfig::DiplomacyEventExpires(PersistedDiplomacyEventType eventType){
	return expires[int(eventType)];
}

uint32_t cPersistedDiplomacyEventConfig::GetDiplomacyEventExpireTime(PersistedDiplomacyEventType eventType){
	return expireTime[int(eventType)];
}

bool cPersistedDiplomacyEventConfig::DiplomacyEventRefreshedOnRepeat(PersistedDiplomacyEventType eventType){
	return refreshedOnRepeat[int(eventType)];
}


