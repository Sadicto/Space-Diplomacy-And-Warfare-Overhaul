#pragma once

#include <Spore\BasicIncludes.h>

#define cPersistedObjectPtr intrusive_ptr<cPersistedObject>


/// Base class for persisted objects.
class cPersistedObject
	: public Simulator::ISimulatorSerializable
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyWarfareOverhaul::cPersistedObject");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;
	int Release() override;
	virtual void* Cast(uint32_t type) const override;
	uint32_t virtual GetNounID() const override;
	bool virtual Write(Simulator::ISerializerStream* stream) override;
	bool virtual Read(Simulator::ISerializerStream* stream) override;
	bool ISimulatorSerializable_func18h() override;
	bool WriteToXML(Simulator::XmlSerializer*) override;

	static Simulator::Attribute ATTRIBUTES[];

	uint32_t GetInstanceID() const;

	void SetInstanceID(uint32_t instanceID);

	bool HasInjectedDependencies() const;

	void SetDependenciesInjected(bool dependenciesInjected);

	/// Sets 'destroyed' to 'true', forcing 'Valid()' to return false, ensuring that the object will be deleted in the next database write.
	void Destroy();

	/// Performs a validity check on the instance, if 'Valid()' returns false, the object won't be written to the database 
	/// and will be deleted if it was already persisted.
	bool virtual Valid();

private:
	
	uint32_t instanceID = UINT32_MAX;

	bool dependenciesInjected = false;

	bool destroyed = false;
};

class cPersistedObjectFactory
	: public App::ISPClassFactory
{
public:
	size_t GetTypes(uint32_t* pDstTypes, size_t count) const override;
	Object* Create(uint32_t type, ICoreAllocator* pAllocator) override;
	const char* GetName(uint32_t type) const override;
};

