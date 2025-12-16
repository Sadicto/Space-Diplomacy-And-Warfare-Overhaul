#pragma once

#include <Spore\BasicIncludes.h>
#include "../cPersistedEvent.h"

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
	static const uint32_t TYPE = id("Ai_Empire_Diplomacy::cPersistedDiplomacyEvent");
	static const uint32_t NOUN_ID = TYPE;

	void* Cast(uint32_t type) const override;
	uint32_t GetCastID() const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;

	uint32_t GetEmpire1ID();

	void SetEmpire1ID(uint32_t ID);

	uint32_t GetEmpire2ID();

	void SetEmpire2ID(uint32_t ID);

	static Simulator::Attribute ATTRIBUTES[];

private:
	
	uint32_t empire1ID;

	uint32_t empire2ID;
};

class cPersistedDiplomacyEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

