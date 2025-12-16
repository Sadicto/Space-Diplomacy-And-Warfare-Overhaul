#include "stdafx.h"
#include "cArchetypesConfig.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>

using namespace SporeModUtils;

cArchetypesConfig::cArchetypesConfig(ResourceKey archetypesAffinitiesKey, ResourceKey archetypesAgressivitiesKey)
{
	PropertyListPtr archetypesAffinitiesProp;
	PropManager.GetPropertyList(archetypesAffinitiesKey.instanceID, archetypesAffinitiesKey.groupID, archetypesAffinitiesProp);
	uint32_t compatibilitesHash[8] = { 0x36F42279, 0x70C32FF1, 0x9526B215, 0xC225B633, 0xDD439F1C, 0x51DEC400, 0xCC6B3EFB, 0xD2A58F42 };

	for (int i = 0; i < 8; i++) {
		eastl::vector<int> affinity;
		App::Property::GetArrayInt32(archetypesAffinitiesProp.get(), compatibilitesHash[i], affinity);
		archetypesAffinities.push_back(affinity);
	}

	PropertyListPtr archetypesAgressivitiesProp;
	PropManager.GetPropertyList(archetypesAgressivitiesKey.instanceID, archetypesAgressivitiesKey.groupID, archetypesAgressivitiesProp);
	uint32_t agressivitiesHash[8] = { 0x0E1AF0CB, 0xFA5B88E3, 0x57CF3387, 0x37352675, 0xA9C8F8A0, 0xC4877964, 0x85B4B9DD, 0x22514096 };

	for (int i = 0; i < 8; i++) {
		eastl::vector<int> agressivity;
		App::Property::GetArrayInt32(archetypesAgressivitiesProp.get(), agressivitiesHash[i], agressivity);
		archetypesAgressivities.push_back(agressivity);
	}
}


cArchetypesConfig::~cArchetypesConfig()
{
}

// For internal use, do not modify.
int cArchetypesConfig::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cArchetypesConfig::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cArchetypesConfig::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cArchetypesConfig);
	return nullptr;
}

int cArchetypesConfig::GetArchetypesAffinity(Simulator::Archetypes archetype1, Simulator::Archetypes archetype2) {
	return archetypesAffinities[ArchetypeUtils::GetBaseArchetype(archetype1)][ArchetypeUtils::GetBaseArchetype(archetype2)];
}

int cArchetypesConfig::GetArchetypeAgressivtyByPowerLevel(Simulator::Archetypes archetype, int powerLevel) {
	return archetypesAgressivities[ArchetypeUtils::GetBaseArchetype(archetype)][powerLevel];
}
