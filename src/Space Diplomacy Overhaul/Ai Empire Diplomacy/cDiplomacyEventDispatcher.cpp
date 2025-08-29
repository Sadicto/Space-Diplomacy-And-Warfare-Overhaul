#include "stdafx.h"
#include "cDiplomacyEventDispatcher.h"

cDiplomacyEventDispatcher::cDiplomacyEventDispatcher()
{
}


cDiplomacyEventDispatcher::~cDiplomacyEventDispatcher()
{
}

// For internal use, do not modify.
int cDiplomacyEventDispatcher::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cDiplomacyEventDispatcher::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cDiplomacyEventDispatcher::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cDiplomacyEventDispatcher);
	return nullptr;
}

void cDiplomacyEventDispatcher::DispatchDiplomacyEvent(DiplomacyEventType eventType, Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	cDiplomacyEvent* messageBody = new cDiplomacyEvent(eventType, empire1, empire2);
	MessageManager.MessageSend(cDiplomacyEvent::ID, messageBody);
	delete messageBody;
}
