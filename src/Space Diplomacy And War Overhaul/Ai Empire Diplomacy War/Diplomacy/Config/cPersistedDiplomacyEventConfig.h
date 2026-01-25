#pragma once

#include <Spore\BasicIncludes.h>

/// Identifies the different types of persisted diplomacy events.
enum class PersistedDiplomacyEventType {
	NeighborsInPeace = 0,
	FormedAlliance = 1,
	DefeatedEnemyTogether = 2,
	UpliftedByMonolith = 3,
	MadePeace = 4
};

#define cPersistedDiplomacyEventConfigPtr intrusive_ptr<cPersistedDiplomacyEventConfig>

/// Stores and provides access to configuration values
/// for persisted diplomacy events.
class cPersistedDiplomacyEventConfig 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cPersistedDiplomacyEventConfig");
	
	cPersistedDiplomacyEventConfig(ResourceKey persistedDiplomacyEventConfig);
	~cPersistedDiplomacyEventConfig();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	// Gets whether a persisted diplomacy event type expires.
	bool DiplomacyEventExpires(PersistedDiplomacyEventType eventType);

	// Gets the base expiration time for a persisted diplomacy event type.
	uint32_t GetDiplomacyEventExpireTime(PersistedDiplomacyEventType eventType);

	// Gets whether the expiration time is refreshed
	// when the same event is triggered again.
	bool DiplomacyEventRefreshedOnRepeat(PersistedDiplomacyEventType eventType);

private:

	// Stores whether each diplomacy event type expires.
	eastl::vector<bool> expires;

	// Stores the expiration time for each diplomacy event type.
	eastl::vector<uint32_t> expireTime;

	// Stores whether each diplomacy event type refreshes on repeat.
	eastl::vector<bool> refreshedOnRepeat;
};
