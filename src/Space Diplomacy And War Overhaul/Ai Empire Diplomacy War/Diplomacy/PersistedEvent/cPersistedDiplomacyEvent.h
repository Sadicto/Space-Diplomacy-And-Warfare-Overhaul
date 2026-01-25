#pragma once

#include <Spore\BasicIncludes.h>
#include "../cPersistedEvent.h"

#define cPersistedDiplomacyEventPtr intrusive_ptr<cPersistedDiplomacyEvent>

// Base class for persisted diplomacy-related events.
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


	/// @brief Returns whether the event is currently active.
	/// This is a semantic check intended to reflect current diplomatic state.
	virtual bool Active();

	/// @brief Returns the first empire associated with the event.
	Simulator::cEmpire* GetEmpire1();

	/// @brief Sets the first empire associated with the event.
	/// @param empire1.
	void SetEmpire1(Simulator::cEmpire* empire1);

	/// @brief Returns the second empire associated with the event.
	Simulator::cEmpire* GetEmpire2();

	/// @brief Sets the second empire associated with the event.
	/// @param empire2.
	void SetEmpire2(Simulator::cEmpire* empire2);

	static Simulator::Attribute ATTRIBUTES[];

protected:

	// First empire involved in the diplomacy event.
	cEmpirePtr empire1;

	// Second empire involved in the diplomacy event.
	cEmpirePtr empire2;

};

class cPersistedDiplomacyEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

