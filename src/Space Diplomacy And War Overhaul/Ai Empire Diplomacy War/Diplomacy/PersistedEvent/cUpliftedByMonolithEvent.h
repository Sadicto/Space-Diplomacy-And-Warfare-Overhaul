#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedDiplomacyEvent.h"

#define cUpliftedByMonolithEventPtr intrusive_ptr<cUpliftedByMonolithEvent>

// Persisted diplomacy event representing an uplift performed via a monolith.
class cUpliftedByMonolithEvent
	: public cPersistedDiplomacyEvent
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cUpliftedByMonolithEvent");
	static const uint32_t NOUN_ID = TYPE;

	void* Cast(uint32_t type) const override;
	uint32_t GetCastID() const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;

	static Simulator::Attribute ATTRIBUTES[];

private:
};

class cUpliftedByMonolithEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

