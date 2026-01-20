#include "stdafx.h"
#include "cPersistedDiplomacyEventManager.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace Simulator;
using namespace SporeModUtils;

cPersistedDiplomacyEventManager::cPersistedDiplomacyEventManager(cPersistedEventSystem* persistedEventSystem, cAffinityConfig* affinityConfig)
{
	this->affinityConfig = affinityConfig;
	this->persistedEventSystem = persistedEventSystem;
}


cPersistedDiplomacyEventManager::~cPersistedDiplomacyEventManager()
{
}

// For internal use, do not modify.
int cPersistedDiplomacyEventManager::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cPersistedDiplomacyEventManager::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cPersistedDiplomacyEventManager::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cPersistedDiplomacyEventManager);
	return nullptr;
}

uint32_t cPersistedDiplomacyEventManager::CurrentTime() {
	return persistedEventSystem->CurrentTime();
}

void cPersistedDiplomacyEventManager::GetPersistedDiplomaticEventsFromEmpire(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, cEmpire* empire) {
	auto persistedDiplomacyEvent = GetData<cPersistedDiplomacyEvent>();
	for (cPersistedDiplomacyEventPtr diplomacyEvent : persistedDiplomacyEvent) {
		if (diplomacyEvent->Valid() && (diplomacyEvent->GetEmpire1() == empire || diplomacyEvent->GetEmpire2() == empire)) {
			diplomacyEvents.push_back(diplomacyEvent);
		}
	}
}

void cPersistedDiplomacyEventManager::GetPersistedDiplomaticEventsBetweenEmpires(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, cEmpire* empire1, cEmpire* empire2) {
	auto persistedDiplomacyEvent = GetData<cPersistedDiplomacyEvent>();
	for (cPersistedDiplomacyEventPtr diplomacyEvent : persistedDiplomacyEvent) {
		if (diplomacyEvent->Valid() &&
			((diplomacyEvent->GetEmpire1() == empire1 && diplomacyEvent->GetEmpire2() == empire2) ||
				(diplomacyEvent->GetEmpire1() == empire2 && diplomacyEvent->GetEmpire2() == empire1))) {
			diplomacyEvents.push_back(diplomacyEvent);
		}
	}
}

cPersistedDiplomacyEvent* cPersistedDiplomacyEventManager::CreatePersistedDiplomacyEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType, uint32_t duration)
{
	// TODO check an event of the same type does not already exists between the two empires.
	cPersistedDiplomacyEvent* persistedDiplomacyEvent = simulator_new<cPersistedDiplomacyEvent>();
	persistedDiplomacyEvent->SetCreationTime(CurrentTime());
	bool expirable = duration == 0 ? true : false;
	persistedDiplomacyEvent->SetExpires(expirable);
	persistedDiplomacyEvent->SetExpirationTime(CurrentTime() + duration);
	persistedDiplomacyEvent->SetEmpire1(empire1);
	persistedDiplomacyEvent->SetEmpire2(empire2);
	persistedDiplomacyEvent->SetEventType(eventType);
	return persistedDiplomacyEvent;
}
