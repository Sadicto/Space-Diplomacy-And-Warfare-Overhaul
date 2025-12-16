#pragma once

#include <Spore\BasicIncludes.h>

#define cPersistedEventSystemPtr intrusive_ptr<cPersistedEventSystem>

///
/// In your dllmain Initialize method, add the system like this:
/// ModAPI::AddSimulatorStrategy(new cPersistedEventSystem(), cPersistedEventSystem::NOUN_ID);
///

/// Manages a persistent collection of events and tracks those with time limits.
class cPersistedEventSystem
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("Ai_Empire_Diplomacy::cPersistedEventSystem");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;
	int Release() override;
	void Initialize() override;
	void Dispose() override;
	const char* GetName() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;
	void Update(int deltaTime, int deltaGameTime) override;
	bool WriteToXML(Simulator::XmlSerializer* xml) override;
	void OnModeEntered(uint32_t previousModeID, uint32_t newModeID) override;
	void OnModeExited(uint32_t previousModeID, uint32_t newModeID) override;

	static cPersistedEventSystem* Get();

	static Simulator::Attribute ATTRIBUTES[];

private:

	static cPersistedEventSystem* instance;

};