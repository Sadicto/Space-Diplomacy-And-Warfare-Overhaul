#pragma once

#include <Spore\BasicIncludes.h>
using namespace Simulator;

#define cEmpireDiplomacyManagerPtr intrusive_ptr<cEmpireDiplomacyManager>

///
/// In your dllmain Initialize method, add the system like this:
/// ModAPI::AddSimulatorStrategy(new cEmpireDiplomacyManager(), cEmpireDiplomacyManager::NOUN_ID);
///

class cEmpireDiplomacyManager
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("HarderSpaceDiplomacy::cEmpireDiplomacyManager");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;
	int Release() override;
	void Initialize() override;
	void Dispose() override;
	const char* GetName() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;
	void Update(int deltaTime, int deltaGameTime) override;
	bool WriteToXML(XmlSerializer* xml) override;
	void OnModeEntered(uint32_t previousModeID, uint32_t newModeID) override;
	static Simulator::Attribute ATTRIBUTES[];

	//
	// You can add more methods here
	//

	/**
	* @brief Gets the current star system the player is in.
	* Preconditions: none.
	* @return Pointer to the current star system (cStarRecord*).
	*/
	cStarRecord* GetCurrentStar();

	/**
	 * @brief Retrieves all empires within a given radius of coordinates,
	 * except for the Grox.
	 * Preconditions: none.
	 * @param coords Vector3 representing the center coordinates.
	 * @param radius Float representing the search radius in parsecs.
	 * @param empires Vector to store the list of empires (eastl::vector<cEmpirePtr>&)
	 * @param includePlayer if is true includes the player in the search.
	 * found within the radius.
	 */
	void GetEmpiresInRadius(const Vector3& coords, float radius, eastl::vector<cEmpirePtr>& empires, bool includePlayer = false);


	/**
	 * @brief Calculates the distance between two star systems.
	 * Preconditions: none.
	 * @param star1 Pointer to the first star system (cStarRecord*).
	 * @param star2 Pointer to the second star system (cStarRecord*).
	 * @return Float representing the distance between the two stars in parsecs.
	 */
	float GetDistanceBetweenStars(cStarRecord* star1, cStarRecord* star2);

	//new

	static cEmpireDiplomacyManagerPtr Get();

	eastl::string16 ArchetypeToString(Archetypes archetype);

	/**
	 * @brief Returns true if the player has encountered the given empire.
	 * This check is based on the player's visited stars and the empire's colonies,
	 * so it may not be 100% accurate.
	 * Preconditions: none.
	 * @param empire Pointer to the empire to check.
	 * @return true if the player has likely encountered the empire, false otherwise.
	 */
	bool EmpireEncountered(cEmpire* empire);

	int GetEmpireLevel(cEmpire* empire);

	int GetEmpireRange(cEmpire* empire);

	void GetEmpiresInRange(cEmpire* empire, eastl::vector<cEmpirePtr>& empires);

	bool Allied(cEmpire* empire1, cEmpire* empire2);

	bool AllianceWithEnemyOfEmpire(cEmpire* empire, cEmpire* target);

	int CommonEnemiesCount(cEmpire* empire1, cEmpire* empire2);

	int EmpireAgressivity(cEmpire* empire, cEmpire* target);

	int ArchetypeCompatibility(Archetypes archetype1, Archetypes archetype2);

	float BoundedSigmoid(float x);

	float AllianceProbability(cEmpire* empire1, cEmpire* empire2);

	float BreakAllianceProbability(cEmpire* empire1, cEmpire* empire2);

	float DeclareWarProbability(cEmpire* empire, cEmpire* target);

	void  CreateTributeComm(cEmpire* empire);

	void ManageEmpireDiplomacy(cEmpire* empire);

	void EmpireDiplomacyCycle();

	bool declareWar;




	

private:
	//
	// You can add members here.
	//

	static cEmpireDiplomacyManagerPtr instance;

	int elapsedTime;

	// Miliseconds of gameTime between expansion cycles.
	static const int cycleInterval;

	// Radius (in parsecs) in which the manager is active.
	static const float activeRadius;

	// Matrix of 8x8 that stores the archetypesCompatibilities.
	eastl::vector<eastl::vector<int>> archetypesCompatibilities;

	int cycle;
};