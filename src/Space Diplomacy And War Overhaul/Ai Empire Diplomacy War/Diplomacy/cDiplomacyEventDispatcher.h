#pragma once

#include <Spore\BasicIncludes.h>
#include "cDiplomacyEvent.h"

#define cDiplomacyEventDispatcherPtr intrusive_ptr<cDiplomacyEventDispatcher>

/// Responsible for dispatching diplomacy events.
class cDiplomacyEventDispatcher 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cDiplomacyEventDispatcher");
	
	cDiplomacyEventDispatcher();
	~cDiplomacyEventDispatcher();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Dispatches a diplomacy event using the messaging system.
	/// @param eventType The type of diplomacy event to trigger.
	/// @param empire1 The first empire involved in the event.
	/// @param empire2 The second empire involved in the event.
	void DispatchDiplomacyEvent(DiplomacyEventType eventType, Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);
};
