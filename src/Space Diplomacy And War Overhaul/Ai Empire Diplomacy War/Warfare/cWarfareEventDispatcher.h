#pragma once

#include <Spore\BasicIncludes.h>

#define cWarfareEventDispatcherPtr intrusive_ptr<cWarfareEventDispatcher>

/// Responsible for dispatching warfare events.
class cWarfareEventDispatcher 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cWarfareEventDispatcher");
	
	cWarfareEventDispatcher();
	~cWarfareEventDispatcher();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Dispatches a PlanetAttackedEvent using the messaging system.
	/// @param aggressorEmpire The empire launching the attack.
	/// @param target The planet being attacked.
	/// @param bombers The number of bombers involved in the attack.
	void DispatchPlanetAttackedEvent(Simulator::cEmpire* aggressorEmpire, Simulator::cPlanetRecord* target, int bombers);
};
