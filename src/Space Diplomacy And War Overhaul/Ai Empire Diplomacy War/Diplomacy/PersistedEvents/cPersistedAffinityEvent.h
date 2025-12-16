#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedDiplomacyEvent.h"

#define cPersistedAffinityEventPtr intrusive_ptr<cPersistedAffinityEvent>

///
/// In your dllmain Initialize method, add the factory like this:
/// ClassManager.AddFactory(new cPersistedAffinityEventFactory());
///
/// Then you will be able to create instances of this class by doing:
/// auto obj = simulator_new<cPersistedAffinityEvent>();

class cPersistedAffinityEvent
	: public cPersistedDiplomacyEvent
{
public:
	static const uint32_t TYPE = id("Ai_Empire_Diplomacy::cPersistedAffinityEvent");
	static const uint32_t NOUN_ID = TYPE;

	void* Cast(uint32_t type) const override;
	uint32_t GetCastID() const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;

	//
	// You can add more methods here
	//

	static Simulator::Attribute ATTRIBUTES[];

private:
	//
	// You can add members here
	//
};

class cPersistedAffinityEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

