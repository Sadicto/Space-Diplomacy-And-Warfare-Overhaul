#pragma once

#include <Spore\BasicIncludes.h>

#define cDiplomaticProfilePtr intrusive_ptr<cDiplomaticProfile>

class cDiplomaticProfile 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cDiplomaticProfile");
	
	cDiplomaticProfile(Simulator::cEmpire* empire);
	~cDiplomaticProfile();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	int CalculateStrenghtOfEmpire();

	int CalculateStrenghtOfAlliance();

	void OnAllianceBroken(Simulator::cEmpire* otherEmpire);

	void OnWarStarted(Simulator::cEmpire* otherEmpire);

	void OnAllianceFormed(Simulator::cEmpire* otherEmpire);

	Simulator::cEmpire* GetBreakAllianceTarget();

	Simulator::cEmpire* GetWarTarget();

	Simulator::cEmpire* GetAllianceTarget();

	cEmpirePtr empire;
	int strenght;
	int srenghtOfAlliance;
	eastl::vector<cEmpirePtr> allies;
	eastl::vector<cEmpirePtr> enemies;
	eastl::vector<cEmpirePtr> neutrals;
};
