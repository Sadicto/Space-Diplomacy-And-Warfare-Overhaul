#include "stdafx.h"
#include "cEmpireWarfareFactory.h"

cEmpireWarfareFactory::cEmpireWarfareFactory(cWarfareConfig* warfareConfig,
	cWarfareStrengthAnalyzer* warfareStrengthAnalyzer,
	cWarfareEventDispatcher* warfareEventDispatcher)
{
	this->warfareConfig = warfareConfig;
	this->warfareStrengthAnalyzer = warfareStrengthAnalyzer;
	this->warfareEventDispatcher = warfareEventDispatcher;
}


cEmpireWarfareFactory::~cEmpireWarfareFactory()
{
}

// For internal use, do not modify.
int cEmpireWarfareFactory::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cEmpireWarfareFactory::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cEmpireWarfareFactory::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cEmpireWarfareFactory);
	return nullptr;
}

cEmpireWarfare* cEmpireWarfareFactory::CreateEmpireWarfare(Simulator::cEmpire* empire)
{
	return new cEmpireWarfare(empire, warfareConfig.get(), warfareStrengthAnalyzer.get(), warfareEventDispatcher.get());
}
