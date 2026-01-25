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

cPersistedDiplomacyEventManager::cPersistedDiplomacyEventManager(cPersistedDiplomacyEventConfig* persistedDiplomacyEventConfig, cPersistedEventSystem* persistedEventSystem)
{
	this->persistedDiplomacyEventConfig = persistedDiplomacyEventConfig;
	this->persistedEventSystem = persistedEventSystem;
	eastl::vector<cPersistedEventPtr> persistedEvents;
	persistedEventSystem->GetPersistedEvents(persistedEvents);
	eastl::vector<cPersistedEventPtr> persistedDiplomacyEvents;
	for (cPersistedEventPtr persistedEvent : persistedEvents) {
		cPersistedDiplomacyEventPtr persistedDiplomacyEvent = object_cast<cPersistedDiplomacyEvent>(persistedEvent);
		if (persistedDiplomacyEvent != nullptr) {
			if (persistedDiplomacyEvent->Active()) {
				cEmpirePtr empire = persistedDiplomacyEvent->GetEmpire1();
				diplomacyEventsByEmpire[empire].push_back(persistedDiplomacyEvent);
				empire = persistedDiplomacyEvent->GetEmpire2();
				diplomacyEventsByEmpire[empire].push_back(persistedDiplomacyEvent);
			}
			else {
				persistedEventSystem->DeleteEvent(persistedDiplomacyEvent.get());
			}
		}
	}
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

void cPersistedDiplomacyEventManager::GetPersistedDiplomaticEventsOfEmpire(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, cEmpire* empire) {
	auto mapIt = diplomacyEventsByEmpire.find(empire);
	if (mapIt != diplomacyEventsByEmpire.end()){

		eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEventsOfEmpire = mapIt->second;
		auto diplomacyEventIt = diplomacyEventsOfEmpire.begin();
		while (diplomacyEventIt != diplomacyEventsOfEmpire.end()) {

			cPersistedDiplomacyEvent* diplomacyEvent = diplomacyEventIt->get();
			if (diplomacyEvent != nullptr){
				if (diplomacyEvent->Active()) {
					diplomacyEvents.push_back(diplomacyEvent);
					diplomacyEventIt++;
				}
				else {
					persistedEventSystem->DeleteEvent(diplomacyEvent);
					diplomacyEventIt = diplomacyEventsOfEmpire.erase(diplomacyEventIt);
				}
			}
			else {
				diplomacyEventIt = diplomacyEventsOfEmpire.erase(diplomacyEventIt);
			}
		}
		if (diplomacyEventsOfEmpire.empty()) {
			diplomacyEventsByEmpire.erase(mapIt);
		}
	}
}

void cPersistedDiplomacyEventManager::GetPersistedDiplomaticEventsBetweenEmpires(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, cEmpire* empire1, cEmpire* empire2) {
	eastl::vector<cPersistedDiplomacyEventPtr> diplomacyEventOfEmpire1;
	GetPersistedDiplomaticEventsOfEmpire(diplomacyEventOfEmpire1, empire1);
	for (cPersistedDiplomacyEventPtr diplomacyEvent : diplomacyEventOfEmpire1) {
		if (diplomacyEvent->GetEmpire1() == empire2 || diplomacyEvent->GetEmpire2() == empire2) {
			diplomacyEvents.push_back(diplomacyEvent);
		}
	}
}
cPersistedDiplomacyEvent* cPersistedDiplomacyEventManager::GetPersistedDiplomacyEventBetweenEmpires(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType){
	if (EmpireUtils::ValidNpcEmpire(empire1, true) && (EmpireUtils::ValidNpcEmpire(empire2, true))) {
		eastl::vector <cPersistedDiplomacyEventPtr> diplomacyEventsOfEmpire;
		GetPersistedDiplomaticEventsBetweenEmpires(diplomacyEventsOfEmpire, empire1, empire2);
		uint32_t nounIdOfEvent = GetNounIdOfEvent(eventType);
		for (const cPersistedDiplomacyEventPtr& diplomacyEvent : diplomacyEventsOfEmpire) {
			if (diplomacyEvent->GetNounID() == nounIdOfEvent) {
				return diplomacyEvent.get();
			}
		}
	}
	return nullptr;
}

void cPersistedDiplomacyEventManager::CreatePersistedDiplomacyEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType)
{
	cPersistedDiplomacyEvent* existingEvent = GetPersistedDiplomacyEventBetweenEmpires(empire1, empire2, eventType);
	if (existingEvent != nullptr) {
		if (persistedDiplomacyEventConfig->DiplomacyEventRefreshedOnRepeat(eventType)){
			persistedEventSystem->UpdateExpirationTimeOfExpirableEvent(existingEvent, CurrentTime() + persistedDiplomacyEventConfig->GetDiplomacyEventExpireTime(eventType));
		}
	}
	else {
		uint32_t now = CurrentTime();
		cPersistedDiplomacyEvent* persistedDiplomacyEvent = CreateEmptyPersistedDiplomacyEventOfType(eventType);
		persistedDiplomacyEvent->SetCreationTime(now);
		persistedDiplomacyEvent->SetExpires(persistedDiplomacyEventConfig->DiplomacyEventExpires(eventType));
		if (persistedDiplomacyEvent->Expires()) {
			persistedDiplomacyEvent->SetExpirationTime(now + persistedDiplomacyEventConfig->GetDiplomacyEventExpireTime(eventType));
		}
		else {
			persistedDiplomacyEvent->SetExpirationTime(0);
		}
		persistedDiplomacyEvent->SetEmpire1(empire1);
		persistedDiplomacyEvent->SetEmpire2(empire2);
		diplomacyEventsByEmpire[empire1].push_back(persistedDiplomacyEvent);
		diplomacyEventsByEmpire[empire2].push_back(persistedDiplomacyEvent);
		persistedEventSystem->AddPersistedEvent(persistedDiplomacyEvent);
	}
}

uint32_t cPersistedDiplomacyEventManager::GetNounIdOfEvent(PersistedDiplomacyEventType eventType){
	uint32_t nounId = 0;
	switch (eventType) {
	case(PersistedDiplomacyEventType::NeighborsInPeace): {
		nounId = cNeighborsInPeaceEvent::NOUN_ID;
		break;
	}
	case(PersistedDiplomacyEventType::FormedAlliance): {
		nounId = cFormedAllianceEvent::NOUN_ID;
		break;
	}
	case(PersistedDiplomacyEventType::DefeatedEnemyTogether): {
		nounId = cDefeatedEnemyTogetherEvent::NOUN_ID;
		break;
	}
	case(PersistedDiplomacyEventType::UpliftedByMonolith): {
		nounId = cUpliftedByMonolithEvent::NOUN_ID;
		break;
	}
	case(PersistedDiplomacyEventType::MadePeace): {
		nounId = cMadePeaceEvent::NOUN_ID;
		break;
	}
	}
	return nounId;
}

cPersistedDiplomacyEvent* cPersistedDiplomacyEventManager::CreateEmptyPersistedDiplomacyEventOfType(PersistedDiplomacyEventType eventType){
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
