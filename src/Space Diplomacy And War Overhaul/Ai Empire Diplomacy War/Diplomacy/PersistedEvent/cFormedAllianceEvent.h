#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedDiplomacyEvent.h"

#define cFormedAllianceEventPtr intrusive_ptr<cFormedAllianceEvent>

// Persisted diplomacy event representing two empires
// that have formed an alliance.
class cFormedAllianceEvent
	: public cPersistedDiplomacyEvent
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cFormedAllianceEvent");
	static const uint32_t NOUN_ID = TYPE;

	void* Cast(uint32_t type) const override;
	uint32_t GetCastID() const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;

	/// @brief Returns whether the event is currently active.
	/// The event remains active while both empires are valid
	/// and the alliance between them still exists.
	bool Active() override;

	static Simulator::Attribute ATTRIBUTES[];

private:
};

class cFormedAllianceEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

