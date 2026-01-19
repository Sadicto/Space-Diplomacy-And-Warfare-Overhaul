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

cPersistedDiplomacyEvent* cPersistedDiplomacyEventManager::CreateAffinityEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, AffinityModifier affinityModifier, int affinityGain){
	if (affinityGain == 0) {
		affinityGain = affinityConfig->GetAffinityGain(affinityModifier);
	}
	uint32_t creationTime = persistedEventSystem->CurrentTime();
	uint32_t expirationTime = 0;
	ActionOnExpiry actionOnExpiry;
	bool expires = affinityConfig->AffinityExpires(affinityModifier);

	if (expires) {
		expirationTime = affinityConfig->GetExpireTime(affinityModifier);
		actionOnExpiry = ActionOnExpiry::DecayAffinity;
	}
	else {
		uint32_t expirationTime = 0;
		ActionOnExpiry actionOnExpiry = ActionOnExpiry::Nothing;
	}
	cPersistedDiplomacyEvent* persistedDiplomacyEvent = simulator_new<cPersistedDiplomacyEvent>();
	persistedDiplomacyEvent->SetCreationTime(creationTime);
	persistedDiplomacyEvent->SetExpirationTime(creationTime + expirationTime);
	persistedDiplomacyEvent->SetExpireAction(ActionOnExpiry::Nothing);
	persistedDiplomacyEvent->SetEmpire1(empire1);
	persistedDiplomacyEvent->SetEmpire2(empire2);

	cPersistedDiplomacyEventData* persistedDiplomacyEventData = simulator_new<cPersistedDiplomacyEventData>();
	persistedDiplomacyEventData->SetAffinityModifier(affinityModifier);
	if (affinityGain == 0) {
		affinityGain = affinityConfig->GetAffinityGain(affinityModifier);
	}
	persistedDiplomacyEventData->SetAffinityGain(affinityGain);
	persistedDiplomacyEventData->SetPreventsWars(affinityConfig->AffinityPreventsWars(affinityModifier));

	persistedDiplomacyEvent->SetDiplomacyEventData(persistedDiplomacyEventData);
	if (expires) {
		persistedEventSystem->AddExpirableEvent(persistedDiplomacyEvent);
	}
	return persistedDiplomacyEvent;
}
