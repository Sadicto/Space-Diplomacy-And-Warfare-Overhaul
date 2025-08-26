#pragma once

#include <Spore\BasicIncludes.h>
#include <cDiplomaticProfile.h>
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
	void OnModeExited(uint32_t previousModeID, uint32_t newModeID) override;
	static Simulator::Attribute ATTRIBUTES[];

	//
	// You can add more methods here
	//

	static cEmpireDiplomacyManager* Get();

	eastl::string16 ArchetypeToString(Archetypes archetype);

	float GetEmpireDiplomaticRange(cEmpire* empire);

	void GetEmpiresInDiplomaticRange(cEmpire* empire, eastl::vector<cEmpirePtr>& empires);

	int GetEmpireAgressivity(cEmpire* empire);

	int ArchetypesAffinity(Archetypes archetype1, Archetypes archetype2);

	int EmpiresAffinity(cEmpire* empire1, cEmpire* empire2);

	float AllianceProbability(cEmpire* empire1, cEmpire* empire2);

	float BreakAllianceProbability(cEmpire* empire1, cEmpire* empire2);

	float DeclareWarProbability(cEmpire* empire, cEmpire* target);

	void  CreateTributeComm(cEmpire* empire);

	void DeclareWarBetweenEmpires(cEmpire* empire1, cEmpire* empire2);

	void DeclareAlianceBetweenEmpires(cEmpire* empire1, cEmpire* empire2);

	void BreakAllianceBetweenEmpires(cEmpire* empire1, cEmpire* empire2);

	void ManageEmpireDiplomacy(cEmpire* empire);

	void EmpireDiplomacyCycle();

private:
	//
	// You can add members here.
	//

	static cEmpireDiplomacyManager* instance;

	int elapsedTime;

	// Miliseconds of gameTime between expansion cycles.
	int cycleInterval;

	// Radius (in parsecs) in which the manager is active.
	float activeRadius;

	// Diplomacy range per empire level, in parsecs.
	eastl::vector<float> diplomacyRange;

	// The upper limit beyond which increased affinity has no additional effect.
	int maxAffinitySoftCap;

	// The lower limit below which decreased affinity has no additional effect.
	int minAffinitySoftCap;

	// The maximum probability of alliance between two empires during a single cycle.
	float maxAllianceProbability;

	// The maximum probability of war between two empires during a single cycle.
	float maxWarProbability;

	// The affinity value required for an alliance to be considered stable.
	int affinityThresholdForStableAlliance;

	// The affinity value in which an alliance is considered unstable.
	int affinityThresholdForUnstableAlliance;

	// The maximum affinity at which empires may still declare war.
	int affinityThresholdForWar;

	// Affinity gained between empires that are allies of the same ally.
	int affinityGainForAllyOfAlly;

	// Affinity gained between empires that share a common enemy.
	int affinityGainForEnemyOfEnemy;

	// Affinity lost between empires when one is an enemy of the other's ally (should be negative).
	int affinityGainForEnemyOfAlly;

	// Stores the base aggressiveness for each archetype.
	eastl::vector<int> baseAggressivityByArchetype;

	// Extra aggressiveness gained per power level of an empire.
	eastl::vector<int> aggressivityGrowthByPowerLevel;

	// If true, AI empires automatically declare war on enemies of their allies.
	bool autoDeclareWarOnAllyEnemies;

	// If true, AI empires may start new wars while already at war against non-enemies of allies.
	bool startsWarsWhileAtWar;

	// If (power of target + allies) / (power of empire + allies) > powerThresholdToAvoidWar, then the empire will avoid declaring war.
	float powerThresholdToAvoidWar;

	// An 8x8 matrix storing affinity values between archetypes.
	eastl::vector<eastl::vector<int>> archetypesAffinities;

	eastl::map<cEmpirePtr, cDiplomaticProfilePtr> diplomaticProfiles;


	int cycle;
};