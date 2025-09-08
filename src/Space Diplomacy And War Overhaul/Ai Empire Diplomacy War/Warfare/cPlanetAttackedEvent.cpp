#include "stdafx.h"
#include "cPlanetAttackedEvent.h"

cPlanetAttackedEvent::cPlanetAttackedEvent(Simulator::cEmpire* aggressorEmpire, Simulator::cPlanetRecord* target, int bombers)
{
	this->aggressorEmpire = aggressorEmpire;
	this->target = target;
	this->bombers = bombers;
}


cPlanetAttackedEvent::~cPlanetAttackedEvent()
{
}
