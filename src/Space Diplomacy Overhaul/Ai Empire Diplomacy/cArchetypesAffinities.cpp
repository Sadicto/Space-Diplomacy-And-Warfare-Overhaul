#include "stdafx.h"
#include "cArchetypesAffinities.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"

using namespace SporeModUtils;

cArchetypesAffinities::cArchetypesAffinities(ResourceKey archetypesAffinitiesKey)
{
	PropertyListPtr archetypesAffinitiesProp;
	PropManager.GetPropertyList(archetypesAffinitiesKey.instanceID, archetypesAffinitiesKey.groupID, archetypesAffinitiesProp);
	uint32_t compatibilitesHash[8] = { 0x36F42279, 0x70C32FF1, 0x9526B215, 0xC225B633, 0xDD439F1C, 0x51DEC400, 0xCC6B3EFB, 0xD2A58F42 };

	for (int i = 0; i < 8; i++) {
		eastl::vector<int> affinity;
		App::Property::GetArrayInt32(archetypesAffinitiesProp.get(), compatibilitesHash[i], affinity);
		archetypesAffinities.push_back(affinity);
	}
}


cArchetypesAffinities::~cArchetypesAffinities()
{
}

// For internal use, do not modify.
int cArchetypesAffinities::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cArchetypesAffinities::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cArchetypesAffinities::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cArchetypesAffinities);
	return nullptr;
}

int cArchetypesAffinities::GetArchetypesAffinity(Simulator::Archetypes archetype1, Simulator::Archetypes archetype2) {
	return archetypesAffinities[ArchetypeUtils::GetBaseArchetype(archetype1)][ArchetypeUtils::GetBaseArchetype(archetype2)];
}
