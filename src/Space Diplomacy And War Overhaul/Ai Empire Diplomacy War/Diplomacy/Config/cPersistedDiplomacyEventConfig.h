#pragma once

#include <Spore\BasicIncludes.h>

enum class PersistedDiplomacyEventType {
	NeighborsInPeace = 0,
	FormedAlliance = 1,
	DefeatedEnemyTogether = 2,
	UpliftedByMonolith = 3,
	MadePeace = 4
};

#define cPersistedDiplomacyEventConfigPtr intrusive_ptr<cPersistedDiplomacyEventConfig>

class cPersistedDiplomacyEventConfig 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cPersistedDiplomacyEventConfig");
	
	cPersistedDiplomacyEventConfig(ResourceKey persistedDiplomacyEventConfig);
	~cPersistedDiplomacyEventConfig();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	bool DiplomacyEventExpires(PersistedDiplomacyEventType eventType);

	uint32_t GetDiplomacyEventExpireTime(PersistedDiplomacyEventType eventType);

	bool DiplomacyEventRefreshedOnRepeat(PersistedDiplomacyEventType eventType);

private:
	eastl::vector<bool> expires;

	eastl::vector<uint32_t> expireTime;

	eastl::vector<bool> refreshedOnRepeat;
};
