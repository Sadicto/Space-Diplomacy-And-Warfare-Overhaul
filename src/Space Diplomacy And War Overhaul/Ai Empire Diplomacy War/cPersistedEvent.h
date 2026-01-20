#pragma once

#include <Spore\BasicIncludes.h>

#define cPersistedEventPtr intrusive_ptr<cPersistedEvent>


///
/// In your dllmain Initialize method, add the factory like this:
/// ClassManager.AddFactory(new cPersistedEventFactory());
///
/// Then you will be able to create instances of this class by doing:
/// auto obj = simulator_new<cPersistedEvent>();

class cPersistedEvent
	: public Simulator::cGameData
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cPersistedEvent");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	uint32_t GetCastID() const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;

	bool virtual Valid();

	bool Expires();

	void SetExpires(bool decays);

	uint32_t GetCreationTime();

	void SetCreationTime(uint32_t creationTime);

	uint32_t GetExpirationTime();

	void SetExpirationTime(uint32_t expirationTime);

	static Simulator::Attribute ATTRIBUTES[];


private:

	bool expires;

	uint32_t creationTime;

	uint32_t expirationTime;
};

class cPersistedEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

