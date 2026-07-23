#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedDiplomacyEvent.h"
#include "../Config/cPersistedDiplomacyEventConfig.h"
#include "../../cSimulationValidator.h"
#include "cDatabaseManager.h"


#define cPersistedDiplomacyEventManagerPtr intrusive_ptr<cPersistedDiplomacyEventManager>

/// Specifies which empire role to match for order-sensitive diplomacy events.
enum class EmpireRole
{
	Any,
	Empire1,
	Empire2
};

// Class responsible for creating and managing the different PersistedDiplomacyEvents.
class cPersistedDiplomacyEventManager 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cPersistedDiplomacyEventManager");
	
	cPersistedDiplomacyEventManager(cSimulationValidator* simulationValidator, 
		cPersistedDiplomacyEventConfig* persistedDiplomacyEventConfig, 
		cDatabaseManager* databaseManager,
		ISpaceTimeProvider* spaceTimeProvider);
	~cPersistedDiplomacyEventManager();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Retrieves all active persisted diplomacy events involving an empire.
	/// @param diplomacyEvents Output vector populated with active events.
	/// @param empire.
	void GetPersistedDiplomaticEventsOfEmpire(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, Simulator::cEmpire* empire);

	/// @brief Returns the first persisted diplomacy event of the specified type involving an empire.
	/// @param empire.
	/// @param eventType.
	/// @param role Specifies whether the empire must match empire1, empire2, or either.
	/// @return The persisted diplomacy event, or nullptr if not found.
	cPersistedDiplomacyEvent* GetPersistedDiplomacyEventOfType(Simulator::cEmpire* empire, PersistedDiplomacyEventType eventType, EmpireRole role = EmpireRole::Any);

	/// @brief Retrieves all active persisted diplomacy events between two empires.
	/// @param diplomacyEvents Output vector populated with active events.
	/// @param empire1.
	/// @param empire2.
	/// For event types where empire order is significant, the order of
    ///	empire1 and empire2 must match the order stored by the event.
	void GetPersistedDiplomaticEventsBetweenEmpires(eastl::vector<cPersistedDiplomacyEventPtr>& diplomacyEvents, Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Returns a specific persisted diplomacy event between two empires, if present.
	/// @param empire1.
	/// @param empire2.
	/// @param eventType.
	/// @return The persisted diplomacy event, or nullptr if not found.
	/// For event types where empire order is significant, the order of
    ///	empire1 and empire2 must match the order stored by the event.
	cPersistedDiplomacyEvent* GetPersistedDiplomacyEventBetweenEmpires(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType);

	/// @brief Creates or updates a persisted diplomacy event between two empires.
	/// If the event already exists, its expiration may be refreshed depending on configuration.
	/// @param empire1.
	/// @param empire2.
	/// @param eventType.
	void CreatePersistedDiplomacyEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType);

	/// @brief Deletes a persisted diplomacy event between two empires, if it exists.
	/// @param empire1.
	/// @param empire2.
	/// @param eventType.
	void DeletePersistedDiplomacyEvent(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, PersistedDiplomacyEventType eventType);

	/// @brief Deletes all persisted diplomacy events of a specific type involving the given empire.
	/// @param empire.
	/// @param eventType.
	void DeleteAllPersistedDiplomacyEventsOfType(Simulator::cEmpire* empire, PersistedDiplomacyEventType eventType);

private:

	/// @brief Returns the noun ID corresponding to a persisted diplomacy event type.
	uint32_t GetNounIdOfEvent(PersistedDiplomacyEventType eventType);

	/// @brief Creates an empty persisted diplomacy event instance of the given type.
	cPersistedDiplomacyEvent* CreateEmptyPersistedDiplomacyEventOfType(PersistedDiplomacyEventType eventType);

	// Pointer to the loaded simulation validator object.
	cSimulationValidatorPtr simulationValidator;

	// Pointed to the loaded persisted diplomacy event config.
	cPersistedDiplomacyEventConfigPtr persistedDiplomacyEventConfig;

	// Pointer to the loaded databaseManager.
	cDatabaseManagerPtr databaseManager;

	// Pointer to the loaded spaceTimeProvider.
	ISpaceTimeProviderPtr spaceTimeProvider;


	// Maps empires to the persisted diplomacy events they are involved in.
	eastl::map<cEmpirePtr, eastl::vector<cPersistedDiplomacyEventPtr>> diplomacyEventsByEmpire;
};
