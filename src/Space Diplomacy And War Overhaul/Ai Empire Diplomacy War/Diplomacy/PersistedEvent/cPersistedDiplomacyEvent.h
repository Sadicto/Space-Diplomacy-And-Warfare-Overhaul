#pragma once

#include <Spore\BasicIncludes.h>
#include "../cPersistedEvent.h"
#include "../cSimulationValidator.h"

#define cPersistedDiplomacyEventPtr intrusive_ptr<cPersistedDiplomacyEvent>

// Base class for persisted diplomacy-related events.
class cPersistedDiplomacyEvent
	: public cPersistedEvent
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cPersistedDiplomacyEvent");
	static const uint32_t NOUN_ID = TYPE;

	void* Cast(uint32_t type) const override;
	virtual uint32_t GetNounID() const override;
	virtual bool Write(Simulator::ISerializerStream* stream) override;
	virtual bool Read(Simulator::ISerializerStream* stream) override;

	static Simulator::Attribute ATTRIBUTES[];

	virtual bool Valid() override;

	/// @brief Returns the first empire associated with the event.
	Simulator::cEmpire* GetEmpire1() const;

	/// @brief Sets the first empire associated with the event.
	/// @param empire1.
	void SetEmpire1(Simulator::cEmpire* empire1);

	/// @brief Returns the second empire associated with the event.
	Simulator::cEmpire* GetEmpire2() const;

	/// @brief Sets the second empire associated with the event.
	/// @param empire2.
	void SetEmpire2(Simulator::cEmpire* empire2);

	/// @brief Returns whether the order of the empires involved in the event matters.
	bool IsEmpireOrderImportant() const;

	/// @brief Sets whether the order of the empires involved in the event matters.
	/// @param value.
	void SetEmpireOrderImportant(bool value);

	void InjectDiplomacyEventDependencies(cSimulationValidator* simulationValidator);

protected:

	uint32_t empire1ID = 0;

	uint32_t empire2ID = 0;

	// Needed because bool values can't be serialized.
	uint32_t empireOrderImportantSerialization = 0;

	// First empire involved in the diplomacy event.
	cEmpirePtr empire1 = nullptr;

	// Second empire involved in the diplomacy event.
	cEmpirePtr empire2 = nullptr;

	// Whether the order of the empires involved in the diplomacy event matters.
	bool empireOrderImportant = false;

	cSimulationValidatorPtr simulationValidator = nullptr;

};

class cPersistedDiplomacyEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

