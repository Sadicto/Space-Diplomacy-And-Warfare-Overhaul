#pragma once

#include <Spore\BasicIncludes.h>

#define cPersistedEventPtr intrusive_ptr<cPersistedEvent>

// Base class for all persisted events.
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

	/// @brief Returns whether the event is still valid.
	/// Extends base validation by ensuring both empires are valid.
	bool virtual Valid();

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

	static Simulator::Attribute ATTRIBUTES[];


private:

	// Whether the event expires after a given time.
	bool expires;

	// Time at which the event was created.
	uint32_t creationTime;

	// Time at which the event expires, if applicable.
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

