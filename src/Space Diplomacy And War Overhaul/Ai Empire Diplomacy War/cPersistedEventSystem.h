#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedEvent.h"
#include "Diplomacy/PersistedEvent/cPersistedDiplomacyEvent.h"

#define cPersistedEventSystemPtr intrusive_ptr<cPersistedEventSystem>


// Manages persisted events lifecycle, tracking expirable events and
// automatically expiring them once their expiration time is reached.
class cPersistedEventSystem
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cPersistedEventSystem");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;
	int Release() override;
	void Initialize() override;
	void Dispose() override;
	const char* GetName() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;
	void Update(int deltaTime, int deltaGameTime) override;
	bool WriteToXML(Simulator::XmlSerializer* xml) override;
	void OnModeEntered(uint32_t previousModeID, uint32_t newModeID) override;
	void OnModeExited(uint32_t previousModeID, uint32_t newModeID) override;

	static cPersistedEventSystem* Get();

	static Simulator::Attribute ATTRIBUTES[];

	/// @brief Returns the persisted‑event system time measured from when the current save entered the Space Stage.
	uint32_t CurrentTime();

	/// @brief Retrieves all valid persisted events currently managed by the system.
	/// @param persistedEvents Output vector populated with valid persisted events.
	void GetPersistedEvents(eastl::vector<cPersistedEventPtr>& persistedEvents);

	/// @brief Registers a persisted event with the system.
	/// @param persistedEvent.
	void AddPersistedEvent(cPersistedEvent* persistedEvent);

	/// @brief Removes and destroys a persisted event.
	/// @param persistedEvent.
	void DeleteEvent(cPersistedEvent* persistedEvent);

	/// @brief Updates the expiration time of an expirable persisted event.
	/// @param expirableEvent.
	/// @param newExpirationTime.
	void UpdateExpirationTimeOfExpirableEvent(cPersistedEvent* expirableEvent, uint32_t newExpirationTime);

private:

	/// @brief Recomputes which expirable event will expire next.
	void RecalculateNextEventToExpire();

	/// @brief Adds an expirable persisted event to the expiration tracking list.
	/// @param expirableEvent.
	void AddExpirablePersistedEvent(cPersistedEvent* expirableEvent);

	/// @brief Finds an expirable persisted event in the tracking list.
	/// @param expirableEvent.
	/// @return Iterator pointing to the event, or end() if not found.
	eastl::vector<cPersistedEventPtr>::iterator FindExpirableEvent(cPersistedEvent* expirableEvent);

	/// @brief Expires and destroys the next scheduled event.
	void ExpireNextEventToExpire();

	/// @brief Removes an expirable event from expiration tracking.
	/// @param expirableEvent.
	void RemoveExpirableEvent(cPersistedEvent* expirableEvent);

	// Current internal time used for persisted event expiration checks, measured from when the current save entered the Space Stage.
	uint32_t currentTime;

	// Expiration time of the next event scheduled to expire.
	uint32_t nextExpirationTime;

	// Time passed (in milliseconds) since the last validation cycle.
	int elapsedTime;

	// Interval (in milliseconds) between persisted-event validation cycles.
	int cycleInterval;

	// Pointer to the next event scheduled to expire.
	cPersistedEventPtr nextEventToExpire;

	// List of all persisted events that have expiration times.
	eastl::vector<cPersistedEventPtr> expirableEvents;

	static cPersistedEventSystem* instance;

};