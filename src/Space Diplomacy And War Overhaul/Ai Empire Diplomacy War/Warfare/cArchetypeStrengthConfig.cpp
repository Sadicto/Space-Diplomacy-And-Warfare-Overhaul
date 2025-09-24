#include "stdafx.h"
#include "cArchetypeStrengthConfig.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include "../Diplomacy/cDiplomacySystem.h"
#include "../Diplomacy/cArchetypesConfig.h"

using namespace SporeModUtils;

cArchetypeStrengthConfig::cArchetypeStrengthConfig(ResourceKey archetypeBaseStrengthKey, ResourceKey archetypeBonusStrengthKey, ResourceKey archetypeHostilityMultiplierKey)
{
	PropertyListPtr archetypeBaseStrengthProp;
	PropManager.GetPropertyList(archetypeBaseStrengthKey.instanceID, archetypeBaseStrengthKey.groupID, archetypeBaseStrengthProp);
	App::Property::GetArrayFloat(archetypeBaseStrengthProp.get(), 0xC6FD9391, archetypeBaseStrengths);

	PropertyListPtr archetypeBonusStrengthProp;
	PropManager.GetPropertyList(archetypeBonusStrengthKey.instanceID, archetypeBonusStrengthKey.groupID, archetypeBonusStrengthProp);
	App::Property::GetArrayFloat(archetypeBonusStrengthProp.get(), 0xA3608DC1, archetypeBonusStrengths);

	float hostilityMultiplier;
	PropertyListPtr archetypeHostilityMultiplierProp;
	PropManager.GetPropertyList(archetypeHostilityMultiplierKey.instanceID, archetypeHostilityMultiplierKey.groupID, archetypeHostilityMultiplierProp);
	App::Property::GetFloat(archetypeHostilityMultiplierProp.get(), 0x0FAE7B8D, hostilityMultiplier);

	// This is extremely nasty and i should change it asap.
	cArchetypesConfig* archetypesConfig = cDiplomacySystem::Get()->archetypesConfig.get();
	Simulator::Archetypes playerArchetype = Simulator::GetPlayerEmpire()->mArchetype;
	if (archetypesConfig != nullptr) {
		for (int i = 0; i < 8; i++) {
			Simulator::Archetypes archetype = static_cast<Simulator::Archetypes>(i);
			int archetypeAffinity = archetypesConfig->GetArchetypesAffinity(playerArchetype, archetype);
			if (archetypeAffinity < 0) {
				archetypeBaseStrengths[archetype] = archetypeBaseStrengths[archetype] + abs(archetypeAffinity) * hostilityMultiplier;
			}
		}
	}
}


cArchetypeStrengthConfig::~cArchetypeStrengthConfig()
{
}

// For internal use, do not modify.
int cArchetypeStrengthConfig::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cArchetypeStrengthConfig::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cArchetypeStrengthConfig::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cArchetypeStrengthConfig);
	return nullptr;
}

float cArchetypeStrengthConfig::GetArchetypeBaseStrength(Simulator::Archetypes archetype) {
	return archetypeBaseStrengths[ArchetypeUtils::GetBaseArchetype(archetype)];
}

float cArchetypeStrengthConfig::GetArchetypeBonusStrength(Simulator::Archetypes archetype) {
	return archetypeBonusStrengths[ArchetypeUtils::GetBaseArchetype(archetype)];
}
