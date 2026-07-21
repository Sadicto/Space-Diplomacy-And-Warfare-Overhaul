#include "stdafx.h"
#include "cPersistedDiplomacyEventManager.h"
#include "../Diplomacy/PersistedEvent/cNeighborsInPeaceEvent.h"
#include "../Diplomacy/PersistedEvent/cMadePeaceEvent.h"
#include "../Diplomacy/PersistedEvent/cUpliftedByMonolithEvent.h"
#include "../Diplomacy/PersistedEvent/cFormedAllianceEvent.h"
#include "../Diplomacy/PersistedEvent/cDefeatedEnemyTogetherEvent.h"
using namespace Simulator;

cPersistedDiplomacyEventManager::cPersistedDiplomacyEventManager(cSimulationValidator* simulationValidator, 
	cPersistedDiplomacyEventConfig* persistedDiplomacyEventConfig, 
	cDatabaseManager* databaseManager,
	ISpaceTimeProvider* spaceTimeProvider)
{
	this->simulationValidator = simulationValidator;
	this->persistedDiplomacyEventConfig = persistedDiplomacyEventConfig;
	this->databaseManager = databaseManager;
	this->spaceTimeProvider = spaceTimeProvider;

	eastl::vector<cPersistedObjectPtr> persistedObjects;
	databaseManager->GetPersistedObjectsOfType(persistedObjects, cPersistedDiplomacyEvent::NOUN_ID, true);

	for (cPersistedObjectPtr persistedObject : persistedObjects)
	{
		cPersistedDiplomacyEventPtr persistedDiplomacyEvent = object_cast<cPersistedDiplomacyEvent>(persistedObject);
		if (persistedDiplomacyEvent == nullptr)
		{
			continue;
		}
		if (!persistedDiplomacyEvent->Valid())
		{
			persistedDiplomacyEvent->Destroy();
			continue;
		}
		cEmpirePtr empire = persistedDiplomacyEvent->GetEmpire1();
		diplomacyEventsByEmpire[empire].push_back(persistedDiplomacyEvent);
		empire = persistedDiplomacyEvent->GetEmpire2();
		diplomacyEventsByEmpire[empire].push_back(persistedDiplomacyEvent);
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

void cPersistedDiplomacyEventManager::GetPersistedDiplomaticEventsOfEmpire(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, cEmpire* empire) {
	auto mapIt = diplomacyEventsByEmpire.find(empire);
	if (mapIt != diplomacyEventsByEmpire.end()){

		eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEventsOfEmpire = mapIt->second;
		auto diplomacyEventIt = diplomacyEventsOfEmpire.begin();
		while (diplomacyEventIt != diplomacyEventsOfEmpire.end()) {

			cPersistedDiplomacyEvent* diplomacyEvent = diplomacyEventIt->get();
			if (diplomacyEvent != nullptr){
				if (diplomacyEvent->Valid()) {
					diplomacyEvents.push_back(diplomacyEvent);
					diplomacyEventIt++;
				}
				else {
					diplomacyEvent->Destroy();
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
	if (simulationValidator->ValidEmpire(empire1, true) && (simulationValidator->ValidEmpire(empire2, true))) {
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

// TODO: Put this in a factory.
void cPersistedDiplomacyEventManager::CreatePersistedDiplomacyEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType)
{
	cPersistedDiplomacyEvent* existingEvent = GetPersistedDiplomacyEventBetweenEmpires(empire1, empire2, eventType);

	if (existingEvent){
		if (!persistedDiplomacyEventConfig->DiplomacyEventReplacedOnRepeat(eventType)){
			return;
		}
		DeletePersistedDiplomacyEvent(empire1, empire2, eventType);
	}
	uint32_t now = spaceTimeProvider->GetCurrentSpaceTime();
	cPersistedDiplomacyEvent* persistedDiplomacyEvent = CreateEmptyPersistedDiplomacyEventOfType(eventType);
	if (persistedDiplomacyEvent == nullptr)
	{
		return;
	}
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
}

void cPersistedDiplomacyEventManager::DeletePersistedDiplomacyEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType){
	cPersistedDiplomacyEvent* existingEvent = GetPersistedDiplomacyEventBetweenEmpires(empire1, empire2, eventType);
	if (existingEvent != nullptr) {
		existingEvent->Destroy();
	}
}

void cPersistedDiplomacyEventManager::DeleteAllPersistedDiplomacyEventsOfType(Simulator::cEmpire* empire, PersistedDiplomacyEventType eventType){
	eastl::vector<cPersistedDiplomacyEventPtr> diplomacyEvents;
	uint32_t nounIdOfEvent = GetNounIdOfEvent(eventType);
	GetPersistedDiplomaticEventsOfEmpire(diplomacyEvents, empire);
	for (cPersistedDiplomacyEventPtr diplomacyEvent : diplomacyEvents) {
		if (diplomacyEvent->GetNounID() == nounIdOfEvent) {
			diplomacyEvent->Destroy();
		}
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
	cPersistedDiplomacyEvent* diplomacyEvent = object_cast<cPersistedDiplomacyEvent>(databaseManager->CreatePersistedObjectOfType(GetNounIdOfEvent(eventType)));
	return diplomacyEvent;
}
