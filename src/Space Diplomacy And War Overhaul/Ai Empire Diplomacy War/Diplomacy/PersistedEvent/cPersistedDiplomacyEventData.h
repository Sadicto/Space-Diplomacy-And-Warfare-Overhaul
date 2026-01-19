#pragma once

#include <Spore\BasicIncludes.h>
#include <Diplomacy/Config/cAffinityConfig.h>

#define cPersistedDiplomacyEventDataPtr intrusive_ptr<cPersistedDiplomacyEventData>

///
/// In your dllmain Initialize method, add the factory like this:
/// ClassManager.AddFactory(new cDiplomaticEventDataFactory());
///
/// Then you will be able to create instances of this class by doing:
/// auto obj = simulator_new<cDiplomaticEventData>();

class cPersistedDiplomacyEventData
	: public Simulator::cGameData
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cPersistedDiplomacyEventData");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	uint32_t GetCastID() const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;

	AffinityModifier GetAffinityModifier();

	void SetAffinityModifier(AffinityModifier affinityModifier);

	int GetAffinityGain();

	void SetAffinityGain(int affinityGain);

	bool PreventsWars();

	void SetPreventsWars(bool preventsWars);

	static Simulator::Attribute ATTRIBUTES[];

private:
	
	uint32_t affinityModifierSerialization;
	AffinityModifier affinityModifier;
	int affinityGain;
	bool preventsWars;
};

class cPersistedDiplomacyEventDataFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

