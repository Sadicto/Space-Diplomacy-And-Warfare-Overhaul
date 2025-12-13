#include "stdafx.h"
#include "cEmpireDiplomacyFactory.h"

cEmpireDiplomacyFactory::cEmpireDiplomacyFactory(cDiplomacyConfig* diplomacyConfig,
	cEmpireRelationsAnalyzer* empireRelationsAnalyzer,
	cDiplomacyEventDispatcher* diplomacyEventDispatcher)
{
	this->diplomacyConfig = diplomacyConfig;
	this->empireRelationsAnalyzer = empireRelationsAnalyzer;
	this->diplomacyEventDispatcher = diplomacyEventDispatcher;
}


cEmpireDiplomacyFactory::~cEmpireDiplomacyFactory()
{
}

// For internal use, do not modify.
int cEmpireDiplomacyFactory::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cEmpireDiplomacyFactory::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cEmpireDiplomacyFactory::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cEmpireDiplomacyFactory);
	return nullptr;
}

cEmpireDiplomacy* cEmpireDiplomacyFactory::CreateEmpireDiplomacy(Simulator::cEmpire* empire)
{
	return new cEmpireDiplomacy(empire, diplomacyConfig.get(), empireRelationsAnalyzer.get(), diplomacyEventDispatcher.get());
}


