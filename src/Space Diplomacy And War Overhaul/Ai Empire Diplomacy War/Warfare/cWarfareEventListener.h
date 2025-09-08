#pragma once

#include <Spore\BasicIncludes.h>

#define cWarfareEventListenerPtr intrusive_ptr<cWarfareEventListener>

/// Handles incoming warfare events dispatched by the system.
class cWarfareEventListener 
	: public App::IMessageListener
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cWarfareEventListener");

	cWarfareEventListener();
	~cWarfareEventListener();

	int AddRef() override;
	int Release() override;
	
	// This is the function you have to implement, called when a message you registered to is sent.
	bool HandleMessage(uint32_t messageID, void* message) override;

	/// @brief Handles a planet being attacked by an empire
	/// creating the corresponding raidEvent.
	/// @param aggressorEmpire The empire initiating the attack.
	/// @param target The planet being attacked.
	/// @param bombers The number of bombers involved in the attack.
	void OnPlanetAttacked(Simulator::cEmpire* aggressorEmpire, Simulator::cPlanetRecord* target, int bombers);
};
