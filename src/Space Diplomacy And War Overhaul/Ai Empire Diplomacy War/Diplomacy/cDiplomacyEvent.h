#pragma once
#include <Spore\BasicIncludes.h>


/// Defines the different types of diplomacy events that may occur between empires.
enum DiplomacyEventType {
	FormAlliance,
	BreakAlliance,
	ConflictBreakAlliance,
	StableAlliance,
	UnstableAlliance,
	HostileAlliance,
	DeclareWar,
	ContinueWar
};

/// Represents a diplomacy event exchanged between empires.
/// This class is used as the body of diplomacy messages.
class cDiplomacyEvent
{
public:
	static const uint32_t ID = id("SpaceDiplomacyOverhaul::cDiplomacyEvent");

	cDiplomacyEvent(DiplomacyEventType eventType, Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);
	~cDiplomacyEvent();
	
	/// The type of diplomacy event.
	DiplomacyEventType eventType;

	// The first empire involved in the event.
	Simulator::cEmpire* empire1;

	/// The second empire involved in the event.
	Simulator::cEmpire* empire2;



};


