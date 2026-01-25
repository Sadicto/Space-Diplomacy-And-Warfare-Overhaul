#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedDiplomacyEvent.h"
#include <cPersistedEventSystem.h>
#include "../Config/cPersistedDiplomacyEventConfig.h"


#define cPersistedDiplomacyEventManagerPtr intrusive_ptr<cPersistedDiplomacyEventManager>

// Class responsible for creating and managing the different PersistedDiplomacyEvents.
class cPersistedDiplomacyEventManager 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cPersistedDiplomacyEventManager");
	
	cPersistedDiplomacyEventManager(cPersistedDiplomacyEventConfig* persistedDiplomacyEventConfig, cPersistedEventSystem* persistedEventSystem);
	~cPersistedDiplomacyEventManager();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Returns the current persisted-event system time.
	uint32_t CurrentTime();

	/// @brief Retrieves all active persisted diplomacy events involving an empire.
	/// @param diplomacyEvents Output vector populated with active events.
	/// @param empire.
	void GetPersistedDiplomaticEventsOfEmpire(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, Simulator::cEmpire* empire);

	/// @brief Retrieves all active persisted diplomacy events between two empires.
	/// @param diplomacyEvents Output vector populated with active events.
	/// @param empire1.
	/// @param empire2.
	void GetPersistedDiplomaticEventsBetweenEmpires(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Returns a specific persisted diplomacy event between two empires, if present.
	/// @param empire1.
	/// @param empire2.
	/// @param eventType.
	/// @return The persisted diplomacy event, or nullptr if not found.
	cPersistedDiplomacyEvent* GetPersistedDiplomacyEventBetweenEmpires(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType);

	/// @brief Creates or updates a persisted diplomacy event between two empires.
	/// If the event already exists, its expiration may be refreshed depending on configuration.
	/// @param empire1.
	/// @param empire2.
	/// @param eventType.
	void CreatePersistedDiplomacyEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType);

private:

	/// @brief Returns the noun ID corresponding to a persisted diplomacy event type.
	uint32_t GetNounIdOfEvent(PersistedDiplomacyEventType eventType);

	/// @brief Creates an empty persisted diplomacy event instance of the given type.
	cPersistedDiplomacyEvent* CreateEmptyPersistedDiplomacyEventOfType(PersistedDiplomacyEventType eventType);

	// Pointed to the loaded persisted diplomacy event config.
	cPersistedDiplomacyEventConfigPtr persistedDiplomacyEventConfig;

	// Pointer to the loaded persistedEventSystem.
	cPersistedEventSystemPtr persistedEventSystem;

	// Maps empires to the persisted diplomacy events they are involved in.
	eastl::map<cEmpirePtr, eastl::vector<cPersistedDiplomacyEventPtr>> diplomacyEventsByEmpire;
};
