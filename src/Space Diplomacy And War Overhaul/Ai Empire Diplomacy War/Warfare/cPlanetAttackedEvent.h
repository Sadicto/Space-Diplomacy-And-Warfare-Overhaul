#pragma once

#include <Spore\BasicIncludes.h>


/// Represents a single warfare event where an empire launches an attack 
/// against a specific planet with a bomber fleet.
/// This class is used as the body of warfare messages.
class cPlanetAttackedEvent 
{
public:
	static const uint32_t ID = id("WarfareOverhaul::cPlanetAttackedEvent");
	
	cPlanetAttackedEvent(Simulator::cEmpire* aggressorEmpire, Simulator::cPlanetRecord* target, int bombers);
	~cPlanetAttackedEvent();

	/// The empire initiating the attack.
	Simulator::cEmpire* aggressorEmpire;

	/// The planet being targeted by the attack.
	Simulator::cPlanetRecord* target;

	/// The number of bombers assigned to the attack.
	int bombers;
};
