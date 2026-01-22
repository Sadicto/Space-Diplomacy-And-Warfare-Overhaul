#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedEvent.h"
#include "Diplomacy/PersistedEvent/cPersistedDiplomacyEvent.h"

#define cPersistedEventSystemPtr intrusive_ptr<cPersistedEventSystem>


/// Manages the persistent events and tracks those with time limits.
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

	uint32_t CurrentTime();

	void AddExpirableEvent(cPersistedEvent* expirableEvent);

private:

	uint32_t currentTime;

	uint32_t nextExpirationTime;

	// Time passed (in miliseconds) since the cycle has started.
	int elapsedTime;

	// Miliseconds of gameTime between expansion cycles.
	int cycleInterval;

	eastl::vector<cPersistedEventPtr>::iterator nextEventToExpire;

	eastl::vector<cPersistedEventPtr> expirableEvents;

	static cPersistedEventSystem* instance;

};