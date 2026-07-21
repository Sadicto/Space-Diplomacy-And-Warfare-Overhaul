#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedObject.h"
#include "ISpaceTimeProvider.h"

#define cPersistenceStatePtr intrusive_ptr<cPersistenceState>

/// Stores metadata for the mod, such as the mod's version of the last save,
/// the instanceID of the next cPersistedObject to be created, and the current
/// space stage time.
class cPersistenceState
	: public cPersistedObject
	, public ISpaceTimeProvider
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyWarfareOverhaul::cPersistenceState");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	uint32_t GetNounID() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;

	static Simulator::Attribute ATTRIBUTES[];

	eastl::string16 GetModVersion() const;

	void SetModVersion(eastl::string16 modVersion);

	/// Returns nextInstanceID and increments it by one.
	uint32_t GetNextInstanceID();

	uint32_t GetCurrentSpaceTime() override;

	/// Increments 'spaceStageTime' by 'deltaTime'.
	void IncrementSpaceTime(uint32_t deltaTime);

private:
	eastl::string16 modVersion;
	uint32_t nextInstanceID = 1;
	uint32_t spaceStageTime = 0;
};

class cPersistenceStateFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

