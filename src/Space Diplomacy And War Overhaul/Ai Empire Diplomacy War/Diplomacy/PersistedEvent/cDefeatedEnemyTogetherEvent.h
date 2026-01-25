#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedDiplomacyEvent.h"

#define cDefeatedEnemyTogetherEventPtr intrusive_ptr<cDefeatedEnemyTogetherEvent>

// Persisted diplomacy event representing two empires
// that have cooperated in defeating a common enemy.
class cDefeatedEnemyTogetherEvent
	: public cPersistedDiplomacyEvent
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cDefeatedEnemyTogetherEvent");
	static const uint32_t NOUN_ID = TYPE;

	void* Cast(uint32_t type) const override;
	uint32_t GetCastID() const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;

	static Simulator::Attribute ATTRIBUTES[];

private:
};

class cDefeatedEnemyTogetherEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

