#pragma once

#include <Spore\BasicIncludes.h>
#include "../cPersistedEvent.h"
#include "cPersistedDiplomacyEventData.h"

#define cPersistedDiplomacyEventPtr intrusive_ptr<cPersistedDiplomacyEvent>

///
/// In your dllmain Initialize method, add the factory like this:
/// ClassManager.AddFactory(new cPersistedDiplomacyEventFactory());
///
/// Then you will be able to create instances of this class by doing:
/// auto obj = simulator_new<cPersistedDiplomacyEvent>();

class cPersistedDiplomacyEvent
	: public cPersistedEvent
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cPersistedDiplomacyEvent");
	static const uint32_t NOUN_ID = TYPE;

	void* Cast(uint32_t type) const override;
	uint32_t GetCastID() const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;

	bool Valid() override;

	Simulator::cEmpire* GetEmpire1();

	void SetEmpire1(Simulator::cEmpire* empire1);

	Simulator::cEmpire* GetEmpire2();

	void SetEmpire2(Simulator::cEmpire* empire2);

	cPersistedDiplomacyEventData* GetDiplomacyEventData();

	void SetDiplomacyEventData(cPersistedDiplomacyEventData* diplomacyEventData);

	static Simulator::Attribute ATTRIBUTES[];

private:
	// Used for serialization.
	uint32_t empire1ID;
	uint32_t empire2ID;

	cEmpirePtr empire1;

	cEmpirePtr empire2;

	cDiplomacyEventDataPtr diplomacyEventData;
};

class cPersistedDiplomacyEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

