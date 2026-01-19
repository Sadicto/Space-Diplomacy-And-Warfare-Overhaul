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

	void GetPersistedDiplomaticEventsFromEmpire(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, Simulator::cEmpire* empire);

	void GetPersistedDiplomaticEventsBetweenEmpires(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Creates a new diplomacy event representing an affinity change event.
	/// @param empire1 The source empire of the event.
	/// @param empire2 The target empire of the event.
	/// @param affinityModifier
	/// @param affinityGain if 0 uses the default affinityGain for the affinityModifier.
	/// @return Pointer to the created cPersistedDiplomacyEvent.
	cPersistedDiplomacyEvent* CreateAffinityEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, AffinityModifier affinityModifier, int affinityGain = 0);

	// Pointer to the loaded persistedEventSystem.
	cPersistedEventSystemPtr persistedEventSystem;

	// Pointer to the loaded affinity config object.
	cAffinityConfigPtr affinityConfig;
};
