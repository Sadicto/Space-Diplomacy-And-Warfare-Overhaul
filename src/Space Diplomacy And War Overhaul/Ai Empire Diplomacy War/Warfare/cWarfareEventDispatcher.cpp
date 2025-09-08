#include "stdafx.h"
#include "cWarfareEventDispatcher.h"
#include "cPlanetAttackedEvent.h"

cWarfareEventDispatcher::cWarfareEventDispatcher()
{
}


cWarfareEventDispatcher::~cWarfareEventDispatcher()
{
}

// For internal use, do not modify.
int cWarfareEventDispatcher::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cWarfareEventDispatcher::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cWarfareEventDispatcher::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cWarfareEventDispatcher);
	return nullptr;
}

void cWarfareEventDispatcher::DispatchPlanetAttackedEvent(Simulator::cEmpire* aggressorEmpire, Simulator::cPlanetRecord* target, int bombers) {
	cPlanetAttackedEvent* messageBody = new cPlanetAttackedEvent(aggressorEmpire, target, bombers);
	MessageManager.MessageSend(cPlanetAttackedEvent::ID, messageBody);
	delete messageBody;
}
