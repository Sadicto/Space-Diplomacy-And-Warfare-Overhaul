#pragma once

#include <Spore\BasicIncludes.h>

#define cSimulationValidatorPtr intrusive_ptr<cSimulationValidator>

/// 0 = Ignore only other players' empires
/// 1 = Ignore other players' empires + their direct allies and enemies
/// 2 = Ignore other players' empires + their direct relations + relations of those relations
enum class EmpireInvalidationDepth {
	othersPlayerEmpiresOnly = 0,
	directRelations = 1,
	indirectRelations = 2
};

/// Validates Simulation objects like empires, stars,
/// and planets.
class cSimulationValidator 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cSimulationValidator");
	
	cSimulationValidator(ResourceKey validatorConfigKey);
	~cSimulationValidator();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	void InvalidateAlliesAndEnemies(Simulator::cEmpire* empire, bool includeIndirectRelations = false);

	/**
	 * An empire is considered valid if it is not null, owns at least one system,
	 * it is not present in the 'invalidEmpires' set, which expands based on the 
	 * current EmpireValidationDepthand and passes the optional filter conditions.
	 * @param empire.
	 * @param includePlayer If set to `false`, the player empire will be excluded.
	 * @param includeGrox If set to `false`, the Grox empire will be excluded.
	 * @return true if the empire is a valid empire; false otherwise.
	 */
	bool ValidEmpire(Simulator::cEmpire* empire, bool includePlayer = false, bool includeGrox = false);

	/**
	 * @brief Checks whether a star is valid.
	 * A star is considered valid if it is not null, represents a normal star 
	 * (i.e., not a black hole, protoplanetary disk, etc.), is not a savegame and has at least one rocky planet.
	 * @param star.
	 * @return true if the star is valid; false otherwise.
	 */
	bool ValidStar(Simulator::cStarRecord* star);

	/**
	 * Returns true if the given planet valid.
	 * A planet is considered valid if it:
	 * - Not null,
	 * - Is a rocky planet (i.e., not a gas giant),
	 * - Is not assigned to a adventure,
	 * - Is not destroyed.
	 *
	 * @param planet
	 * @return True if the player can interact with this planet; false otherwise.
	 */
	bool ValidPlanet(Simulator::cPlanetRecord* planet);

	EmpireInvalidationDepth empireInvalidationDepth;

	eastl::set<cEmpirePtr> invalidEmpires;
};
