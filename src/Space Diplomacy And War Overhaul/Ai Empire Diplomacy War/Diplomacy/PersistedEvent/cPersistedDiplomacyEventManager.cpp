#include "stdafx.h"
#include "cPersistedDiplomacyEventManager.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include "../Diplomacy/PersistedEvent/cNeighborsInPeaceEvent.h"
#include "../Diplomacy/PersistedEvent/cMadePeaceEvent.h"
#include "../Diplomacy/PersistedEvent/cUpliftedByMonolithEvent.h"
#include "../Diplomacy/PersistedEvent/cFormedAllianceEvent.h"
#include "../Diplomacy/PersistedEvent/cDefeatedEnemyTogetherEvent.h"
using namespace Simulator;
using namespace SporeModUtils;

cPersistedDiplomacyEventManager::cPersistedDiplomacyEventManager(cPersistedDiplomacyEventConfig* persistedDiplomacyEventConfig, cAffinityConfig* affinityConfig, cPersistedEventSystem* persistedEventSystem)
{
	this->persistedDiplomacyEventConfig = persistedDiplomacyEventConfig;
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
	auto persistedDiplomacyEvent = GetDataByCast<cPersistedDiplomacyEvent>();
	for (cPersistedDiplomacyEventPtr diplomacyEvent : persistedDiplomacyEvent) {
		if (diplomacyEvent->Valid() && (diplomacyEvent->GetEmpire1() == empire || diplomacyEvent->GetEmpire2() == empire)) {
			diplomacyEvents.push_back(diplomacyEvent);
		}
	}
}

void cPersistedDiplomacyEventManager::GetPersistedDiplomaticEventsBetweenEmpires(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, cEmpire* empire1, cEmpire* empire2) {
	auto persistedDiplomacyEvent = GetDataByCast<cPersistedDiplomacyEvent>();
	for (cPersistedDiplomacyEventPtr diplomacyEvent : persistedDiplomacyEvent) {
		if (diplomacyEvent->Active()){
			if((diplomacyEvent->GetEmpire1() == empire1 && diplomacyEvent->GetEmpire2() == empire2) ||
				(diplomacyEvent->GetEmpire1() == empire2 && diplomacyEvent->GetEmpire2() == empire1)) {
					diplomacyEvents.push_back(diplomacyEvent);
}
		}
		else {
			diplomacyEvent->SetActive(false);
		}
	}
}

cPersistedDiplomacyEvent* cPersistedDiplomacyEventManager::CreatePersistedDiplomacyEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType)
{
	// TODO check an event of the same type does not already exists between the two empires.
	cPersistedDiplomacyEvent* persistedDiplomacyEvent = createdEmptyPersistedDiplomacyEventOfType(eventType);
	persistedDiplomacyEvent->SetCreationTime(CurrentTime());
	persistedDiplomacyEvent->SetExpires(persistedDiplomacyEventConfig->DiplomacyEventExpires(eventType));
	persistedDiplomacyEvent->SetExpirationTime(CurrentTime() + persistedDiplomacyEventConfig->GetDiplomacyEventExpireTime(eventType));
	persistedDiplomacyEvent->SetEmpire1(empire1);
	persistedDiplomacyEvent->SetEmpire2(empire2);
	persistedDiplomacyEvent->SetActive(true);
	return persistedDiplomacyEvent;
}

cPersistedDiplomacyEvent* cPersistedDiplomacyEventManager::createdEmptyPersistedDiplomacyEventOfType(PersistedDiplomacyEventType eventType){
	cPersistedDiplomacyEvent* diplomacyEvent = nullptr;
	switch (eventType) {
	case(PersistedDiplomacyEventType::NeighborsInPeace): {
		diplomacyEvent = simulator_new<cNeighborsInPeaceEvent>();
		break;
	}
	case(PersistedDiplomacyEventType::FormedAlliance): {
		diplomacyEvent = simulator_new<cFormedAllianceEvent>();
		break;
	}
	case(PersistedDiplomacyEventType::DefeatedEnemyTogether): {
		diplomacyEvent = simulator_new<cDefeatedEnemyTogetherEvent>();
		break;
	}
	case(PersistedDiplomacyEventType::UpliftedByMonolith): {
		diplomacyEvent = simulator_new<cUpliftedByMonolithEvent>();
		break;
	}
	case(PersistedDiplomacyEventType::MadePeace): {
		diplomacyEvent = simulator_new<cMadePeaceEvent>();
		break;
	}
	}
	return diplomacyEvent;
}
