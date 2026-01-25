#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedDiplomacyEvent.h"

#define cNeighborsInPeaceEventPtr intrusive_ptr<cNeighborsInPeaceEvent>

// Persisted diplomacy event representing two neighboring empires
// that are not currently at war.
class cNeighborsInPeaceEvent
	: public cPersistedDiplomacyEvent
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cNeighborsInPeaceEvent");
	static const uint32_t NOUN_ID = TYPE;

	void* Cast(uint32_t type) const override;
	uint32_t GetCastID() const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;


	/// @brief Returns whether the event is currently active.
	/// The event is active as long as both empires are valid
	/// and they are not at war with each other.
	bool Active() override;


	static Simulator::Attribute ATTRIBUTES[];

private:

};

class cNeighborsInPeaceEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

