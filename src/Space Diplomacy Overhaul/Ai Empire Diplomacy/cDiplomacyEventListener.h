#pragma once

#include <Spore\BasicIncludes.h>
#include <cDiplomacyPopupManager.h>

#define cDiplomacyEventListenerPtr intrusive_ptr<cDiplomacyEventListener>

/// Handles incoming diplomacy events dispatched by the system.
class cDiplomacyEventListener 
	: public App::IMessageListener
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cDiplomacyEventListener");

	cDiplomacyEventListener(cDiplomacyPopupManager* diplomacyPopUpManager);
	~cDiplomacyEventListener();

	int AddRef() override;
	int Release() override;
	
	// This is the function you have to implement, called when a message you registered to is sent.
	bool HandleMessage(uint32_t messageID, void* message) override;

	/// @brief Called when two empires form an alliance.
	/// @param empire1
	/// @param empire2
	void OnFormAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Called when two empires break an alliance.
	/// @param empire1
	/// @param empire2
	void OnBreakAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Called when an alliance breaks due to conflict between allies.
	/// @param empire1
	/// @param empire2
	void OnConflictBreakAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Called when two empires have a stable alliance.
	/// @param empire1
	/// @param empire2
	void OnStableAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Called when two empires have an unstable alliance.
	/// @param empire1
	/// @param empire2
	void OnUnstableAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Called when two empires have an hostile alliance.
	/// @param empire1
	/// @param empire2
	void OnHostileAlliance(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Called when an empire declares war on another.
	/// @param empire1
	/// @param empire2
	void OnDeclareWar(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Called when an empire decides to continue an ongoing war.
	/// @param empire1
	/// @param empire2
	void OnContinueWar(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	// Pointer to the loaded diplomacy popup manager.
	cDiplomacyPopupManagerPtr diplomacyPopUpManager;

};
