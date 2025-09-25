#include "stdafx.h"
#include "cArchetypeStrengthConfig.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
#include "../Diplomacy/cDiplomacySystem.h"
#include "../Diplomacy/cArchetypesConfig.h"

using namespace SporeModUtils;

cArchetypeStrengthConfig::cArchetypeStrengthConfig(ResourceKey archetypeStrengthConfigKey)
{
	PropertyListPtr archetypeStrengthConfigProp;
	PropManager.GetPropertyList(archetypeStrengthConfigKey.instanceID, archetypeStrengthConfigKey.groupID, archetypeStrengthConfigProp);
	App::Property::GetArrayFloat(archetypeStrengthConfigProp.get(), 0xC6FD9391, archetypeBaseStrengths);

	App::Property::GetArrayFloat(archetypeStrengthConfigProp.get(), 0xA3608DC1, archetypeBonusStrengths);

	float globalMultiplier;
	App::Property::GetFloat(archetypeStrengthConfigProp.get(), 0x9935D339, globalMultiplier);

	float hostilityMultiplier;
	App::Property::GetFloat(archetypeStrengthConfigProp.get(), 0x0FAE7B8D, hostilityMultiplier);

	// This is extremely nasty and i should change it asap.
	cArchetypesConfig* archetypesConfig = cDiplomacySystem::Get()->archetypesConfig.get();
	Simulator::Archetypes playerArchetype = Simulator::GetPlayerEmpire()->mArchetype;
	if (archetypesConfig != nullptr) {
		for (int i = 0; i < 8; i++) {
			Simulator::Archetypes archetype = static_cast<Simulator::Archetypes>(i);
			archetypeBaseStrengths[archetype] = archetypeBaseStrengths[archetype] * globalMultiplier;
			int archetypeAffinity = archetypesConfig->GetArchetypesAffinity(playerArchetype, archetype);
			if (archetypeAffinity < 0) {
				archetypeBaseStrengths[archetype] *= (1.0f + abs(archetypeAffinity) * hostilityMultiplier);

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
