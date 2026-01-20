#pragma once

#include <Spore\BasicIncludes.h>
#include "../Config/cAffinityConfig.h"
#include "cPersistedDiplomacyEvent.h"
#include <cPersistedEventSystem.h>

#define cPersistedDiplomacyEventManagerPtr intrusive_ptr<cPersistedDiplomacyEventManager>

// Class responsible for creating and managing the different PersistedDiplomacyEvents.
class cPersistedDiplomacyEventManager 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cPersistedDiplomacyEventManager");
	
	cPersistedDiplomacyEventManager(cPersistedEventSystem* persistedEventSystem, cAffinityConfig* affinityConfig);
	~cPersistedDiplomacyEventManager();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	uint32_t CurrentTime();

	void GetPersistedDiplomaticEventsFromEmpire(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, Simulator::cEmpire* empire);

	void GetPersistedDiplomaticEventsBetweenEmpires(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);


	cPersistedDiplomacyEvent* CreatePersistedDiplomacyEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType, uint32_t duration = 0);

	// Pointer to the loaded persistedEventSystem.
	cPersistedEventSystemPtr persistedEventSystem;

	// Pointer to the loaded affinity config object.
	cAffinityConfigPtr affinityConfig;
};
