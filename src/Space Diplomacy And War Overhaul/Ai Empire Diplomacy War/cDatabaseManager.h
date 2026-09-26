#pragma once

#include <Spore\BasicIncludes.h>
#include "cPersistedObject.h"
#include "cPersistenceState.h"
#include "cPersistenceInjector.h"

#define cDatabaseManagerPtr intrusive_ptr<cDatabaseManager>

/// Manager for the mod's database.
/// Reads the database on entering space stage, writes it when saving,
/// and has methods to access and create new cPersistedObjects.
class cDatabaseManager
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyWarfareOverhaul::cDatabaseManager");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;
	int Release() override;
	void Initialize() override;
	void Dispose() override;
	const char* GetName() const override;

	/// Called when the player saves the game. If in the Space Stage, 
	/// writes all cPersistedObjects for which 'Valid()' returns true to the database.	
	bool Write(Simulator::ISerializerStream* stream) override;

	bool Read(Simulator::ISerializerStream* stream) override;
	void Update(int deltaTime, int deltaGameTime) override;
	bool WriteToXML(Simulator::XmlSerializer* xml) override;
	void OnModeEntered(uint32_t previousModeID, uint32_t newModeID) override;
	void OnModeExited(uint32_t previousModeID, uint32_t newModeID) override;

	static Simulator::Attribute ATTRIBUTES[];
	static cDatabaseManager* Get();

	/**
	* @brief Injects the required dependencies into the manager.
	* @param persistenceInjector.
	*/
	void InjectDependencies(cPersistenceInjector* persistenceInjector);

	cPersistenceState* GetPersistenceState();

	/**
	* @brief Retrieves and inserts into out all the cPersistedObjects of type 'type'.
	* @param out output vector.
	* @param type
	* @param includeSubtypes if true, also insert all the instances that belong to subclasess of 'type'.
	*/
	void GetPersistedObjectsOfType(eastl::vector<cPersistedObjectPtr>& out, uint32_t type, bool includeSubtypes = false);

	cPersistedObject* CreatePersistedObjectOfType(uint32_t type);


private:
	/**
	* @brief Returns the uint used to identify the current savegame in the database.
	* Currently using the ID of the player's empire
	*/
	uint32_t GetSavegameIdentifier();

	ResourceKey GetResourceKey(cPersistedObject* persistedObject);

	/// @brief Initializes the database.
	/// @return true if initialization succeeded; otherwise, false.
	bool InitializeDB();

	/// @brief Reads the database and populates the persistedObjects vector.
	/// @return true if the database was read successfully, false otherwise
	bool ReadDB();
	
	static cDatabaseManager* instance;

	cPersistenceInjectorPtr persistenceInjector;

	// Indicates whether the manager’s dependencies have been injected.
	bool dependenciesInjected;

	eastl::string16 databaseName;

	eastl::string16 currentModVersion;

	cPersistenceStatePtr persistenceState;

	// TODO: investigate if a vector is the best solution.
	eastl::vector<cPersistedObjectPtr> persistedObjects;

	// Database used to serialize cPersistedObject.
	SerializerDatabasePtr persistedObjectDB;

	// Whether the database has already been read.
	bool readDB;

	bool readFailedPermanently;

};