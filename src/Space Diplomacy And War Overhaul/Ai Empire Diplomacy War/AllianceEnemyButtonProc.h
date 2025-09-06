#pragma once

#include <Spore\BasicIncludes.h>

#define AllianceEnemyButtonProcPtr intrusive_ptr<AllianceEnemyButtonProc>

// To avoid repeating UTFWin:: all the time.
using namespace UTFWin;

class AllianceEnemyButtonProc 
	: public IWinProc
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("AllianceEnemyButtonProc");
	
	AllianceEnemyButtonProc();
	~AllianceEnemyButtonProc();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	
	int GetEventFlags() const override;
	// This is the function you have to implement, called when a window you added this winproc to received an event
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;

	void CreateAlliesAndEnemiesEffecsForSelectedEmpire();

	void DeleteAlliesAndEnemiesEffects();

	cStarRecordPtr lastCurrentStar;
	cEmpirePtr selectedEmpire;
	bool buttonEnabled;
	eastl::vector<IVisualEffectPtr> alliesAndEnemiesEffects;
	
};
