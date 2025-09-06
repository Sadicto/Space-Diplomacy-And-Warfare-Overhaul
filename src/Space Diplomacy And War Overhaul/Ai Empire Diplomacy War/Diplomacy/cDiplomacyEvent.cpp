
#include "stdafx.h"
#include "cDiplomacyEvent.h"


cDiplomacyEvent::cDiplomacyEvent(DiplomacyEventType eventType, Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	this->eventType = eventType;
	this->empire1 = empire1;
	this->empire2 = empire2;
}

cDiplomacyEvent::~cDiplomacyEvent() {

}
