#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedObject.h"
#include "ISpaceTimeProvider.h"

#define cPersistedEventPtr intrusive_ptr<cPersistedEvent>

/// Extends cPersistedObject to persist an object's 'creationTime' and 'expirationTime', along with the methods associated with them.
class cPersistedEvent
	: public cPersistedObject
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyWarfareOverhaul::cPersistedEvent");
	static const uint32_t NOUN_ID = TYPE;

	virtual void* Cast(uint32_t type) const override;
	virtual uint32_t GetNounID() const override;
	virtual bool Write(Simulator::ISerializerStream* stream) override;
	virtual bool Read(Simulator::ISerializerStream* stream) override;

	bool virtual Valid() override;

	/// @brief Returns whether this event has an expiration time.
	bool Expires();

	/// @brief Sets whether this event expires.
	/// @param decays.
	void SetExpires(bool decays);

	/// @brief Returns the creation time of the event.
	uint32_t GetCreationTime();

	/// @brief Sets the creation time of the event.
	/// @param creationTime.
	void SetCreationTime(uint32_t creationTime);

	/// @brief Returns the expiration time of the event.
	uint32_t GetExpirationTime();

	/// @brief Sets the expiration time of the event.
	/// @param expirationTime.
	void SetExpirationTime(uint32_t expirationTime);

	void InjectEventDependencies(ISpaceTimeProvider* spaceTimeProvider);

	static Simulator::Attribute ATTRIBUTES[];


private:

	// Needed because bool values can't be serialized.
	uint32_t expiresSerialization = 0;

	// Whether the event expires after a given time.
	bool expires = false;

	// Time at which the event was created.
	uint32_t creationTime = 0;

	// Time at which the event expires, if applicable.
	uint32_t expirationTime = 0;

	ISpaceTimeProviderPtr spaceTimeProvider = nullptr;
};

class cPersistedEventFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

