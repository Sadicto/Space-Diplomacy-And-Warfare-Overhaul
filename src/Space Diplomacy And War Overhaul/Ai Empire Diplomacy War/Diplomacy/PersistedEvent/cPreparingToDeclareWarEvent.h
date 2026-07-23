#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedDiplomacyEvent.h"
#include <Diplomacy/Config/cDiplomacyConfig.h>

#define cPreparingToDeclareWarEventPtr intrusive_ptr<cPreparingToDeclareWarEvent>

// Persisted diplomacy event representing empire1
// preparing for a war against empire2.
class cPreparingToDeclareWarEvent
	: public cPersistedDiplomacyEvent
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cPreparingToDeclareWarEvent");
	static const uint32_t NOUN_ID = TYPE;

	void* Cast(uint32_t type) const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;

	bool Valid() override;

	void InjectPreparingToDeclareWarEventDependencies(cDiplomacyConfig* diplomacyConfig);

	static Simulator::Attribute ATTRIBUTES[];

private:

	cDiplomacyConfigPtr diplomacyConfig = nullptr;
};

class cPreparingForWarEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

