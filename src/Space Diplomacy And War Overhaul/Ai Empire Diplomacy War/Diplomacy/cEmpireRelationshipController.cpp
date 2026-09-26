#include "stdafx.h"
#include "cEmpireRelationshipController.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>

using namespace SporeModUtils;
using namespace Simulator;

cEmpireRelationshipController::cEmpireRelationshipController(ResourceKey archetypesRelationshipEffects, cDiplomacyEffectAnalyzer* diplomacyEffectAnalyzer)
{
	eastl::vector<ResourceKey> archetypesRelationshipEffectsKeys;
	PropertyListPtr archetypesRelationshipEffectsProp;
	PropManager.GetPropertyList(archetypesRelationshipEffects.instanceID, archetypesRelationshipEffects.groupID, archetypesRelationshipEffectsProp);
	ResourceKey dst;
	App::Property::GetKey(archetypesRelationshipEffectsProp.get(), 0xD6C2DA93, dst); // Warrior.
	archetypesRelationshipEffectsKeys.push_back(dst);
	App::Property::GetKey(archetypesRelationshipEffectsProp.get(), 0x7146861B, dst); // Trader.
	archetypesRelationshipEffectsKeys.push_back(dst);
	App::Property::GetKey(archetypesRelationshipEffectsProp.get(), 0x2EA5110F, dst); // Scientist.
	archetypesRelationshipEffectsKeys.push_back(dst);
	App::Property::GetKey(archetypesRelationshipEffectsProp.get(), 0x2AB2A949, dst); // Shaman.
	archetypesRelationshipEffectsKeys.push_back(dst);
	App::Property::GetKey(archetypesRelationshipEffectsProp.get(), 0x33F628FC, dst); // Bard.
	archetypesRelationshipEffectsKeys.push_back(dst);
	App::Property::GetKey(archetypesRelationshipEffectsProp.get(), 0x4869D400, dst); // Zealot.
	archetypesRelationshipEffectsKeys.push_back(dst);
	App::Property::GetKey(archetypesRelationshipEffectsProp.get(), 0xB17EE241, dst); // Diplomat.
	archetypesRelationshipEffectsKeys.push_back(dst);
	App::Property::GetKey(archetypesRelationshipEffectsProp.get(), 0xBFAEFF76, dst); // Ecologist.
	archetypesRelationshipEffectsKeys.push_back(dst);
	for (int i = 0; i < int(archetypesRelationshipEffectsKeys.size()); i++)
	{
		ResourceKey archetypeRelationshipEffectKey = archetypesRelationshipEffectsKeys[i];
		PropertyListPtr archetypeRelationshipEffectProp;
		PropManager.GetPropertyList(archetypeRelationshipEffectKey.instanceID, archetypeRelationshipEffectKey.groupID, archetypeRelationshipEffectProp);

		// Personality effect.
		eastl::vector<float> relationshipPersonalityNiceArray;
		App::Property::GetArrayFloat(archetypeRelationshipEffectProp.get(), 0x05B6FCC9, relationshipPersonalityNiceArray);
		float relationshipPersonalityNice = relationshipPersonalityNiceArray[0];
		eastl::vector<float> relationshipPersonalityMeanArray;
		App::Property::GetArrayFloat(archetypeRelationshipEffectProp.get(), 0x05B6FCD4, relationshipPersonalityMeanArray);
		float relationshipPersonalityMean = relationshipPersonalityMeanArray[0];

		if (relationshipPersonalityNice != 0)
		{
			archetypePersonalityEffects.push_back(relationshipPersonalityNice);
		}
		else if (relationshipPersonalityMean != 0)
		{
			archetypePersonalityEffects.push_back(relationshipPersonalityMean);
		}
		else
		{
			archetypePersonalityEffects.push_back(0);
		}
		// Archetype modifier.
		eastl::vector<float> relationshipArchetypeModifier;
		App::Property::GetArrayFloat(archetypeRelationshipEffectProp.get(), 0x05FF85B4, relationshipArchetypeModifier); // RelationshipEventSpaceArchetypeMods.
		relationshipArchetypeModifiers.push_back(relationshipArchetypeModifier);
	}
	this->diplomacyEffectAnalyzer = diplomacyEffectAnalyzer;
}


cEmpireRelationshipController::~cEmpireRelationshipController()
{
}

// For internal use, do not modify.
int cEmpireRelationshipController::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cEmpireRelationshipController::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cEmpireRelationshipController::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cEmpireRelationshipController);
	return nullptr;
}

float cEmpireRelationshipController::GetEmpiresRelationship(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2)
{
	return RelationshipManager.CalculateRelationshipAbsolute(empire1->GetEmpireID(), empire2->GetEmpireID(), false) * 10.0f;
}

float cEmpireRelationshipController::ApplyRelationshipEffect(uint32_t politicalID, uint32_t causePoliticalID, uint32_t effectID) {
	float currentValue = RelationshipManager.GetRelationshipEventValue(politicalID, causePoliticalID, effectID);
	float maxScale = diplomacyEffectAnalyzer->GetMaxScale(effectID, currentValue, true);
	if (maxScale != 0.0f) {
		RelationshipManager.ApplyRelationship(politicalID, causePoliticalID, effectID, maxScale);
	}
	return maxScale;
}

float cEmpireRelationshipController::DecayRelationshipEffect(uint32_t politicalID, uint32_t causePoliticalID, uint32_t effectID) {
	float currentValue = RelationshipManager.GetRelationshipEventValue(politicalID, causePoliticalID, effectID);
	float maxDecay = diplomacyEffectAnalyzer->GetMaxDecay(effectID, currentValue);
	if (maxDecay != 0.0f) {
		RelationshipManager.ApplyRelationship(politicalID, causePoliticalID, effectID, maxDecay);
	}
	return maxDecay;
}

float cEmpireRelationshipController::SetRelationshipEffectMax(uint32_t politicalID, uint32_t causePoliticalID, uint32_t effectID) {
	float currentValue = RelationshipManager.GetRelationshipEventValue(politicalID, causePoliticalID, effectID);
	float scaleToMax = diplomacyEffectAnalyzer->GetScaleToMax(effectID, currentValue);
	if (scaleToMax != 0.0f) {
		RelationshipManager.ApplyRelationship(politicalID, causePoliticalID, effectID, scaleToMax);
	}
	return scaleToMax;
}

float cEmpireRelationshipController::SetRelationshipEffectZero(uint32_t politicalID, uint32_t causePoliticalID, uint32_t effectID) {
	float currentValue = RelationshipManager.GetRelationshipEventValue(politicalID, causePoliticalID, effectID);
	float scaleToZero = diplomacyEffectAnalyzer->GetScaleToZero(effectID, currentValue);
	if (scaleToZero != 0.0f) {
		RelationshipManager.ApplyRelationship(politicalID, causePoliticalID, effectID, scaleToZero);
	}
	return scaleToZero;
}

float cEmpireRelationshipController::GetPersonalityEffect(Simulator::cEmpire* empire)
{
	Archetypes archetype = ArchetypeUtils::GetBaseArchetype(empire->mArchetype, true);
	if (int(archetype) > int(archetypePersonalityEffects.size()))
	{
		return 0.0f;
	}
	return archetypePersonalityEffects[archetype];
}

float cEmpireRelationshipController::GetRelationshipArchetypeModifier(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2)
{
	Archetypes archetype1 = ArchetypeUtils::GetBaseArchetype(empire1->mArchetype, true);
	if (int(archetype1) > int(relationshipArchetypeModifiers.size()))
	{
		return 0.0f;
	}
	Archetypes archetype2 = ArchetypeUtils::GetBaseArchetype(empire2->mArchetype, true);
	if (int(archetype2) > int(relationshipArchetypeModifiers[archetype1].size()))
	{
		return 0.0f;
	}
	return relationshipArchetypeModifiers[archetype1][archetype2];
}

void cEmpireRelationshipController::ApplyPersonalityEffect(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2)
{
	float personalityModifier = GetPersonalityEffect(empire2);
	RelationshipEvents relationshipEvent;
	if (personalityModifier > 0.0f)
	{
		relationshipEvent = RelationshipEvents::kRelationshipEventSpacePersonalityNice;
	}
	else
	{
		relationshipEvent = RelationshipEvents::kRelationshipEventSpacePersonalityMean;
	}
	ApplyRelationshipEffect(empire1->GetEmpireID(), empire2->GetEmpireID(), relationshipEvent);
}

void cEmpireRelationshipController::ApplyArchetypeModifier(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2)
{
	float archetypeModifier = GetRelationshipArchetypeModifier(empire1, empire2);
	RelationshipEvents relationshipEvent;
	if (archetypeModifier > 0.0f)
	{
		relationshipEvent = RelationshipEvents::kRelationshipEventSpaceArchetypeNice;
	}
	else
	{
		relationshipEvent = RelationshipEvents::kRelationshipEventSpaceArchetypeMean;
		// To apply a negative modifier, the scale must be positive.
		archetypeModifier *= -1.0f;
	}
	RelationshipManager.ApplyRelationship(empire1->GetEmpireID(), empire2->GetEmpireID(), relationshipEvent, archetypeModifier);
}

void cEmpireRelationshipController::ResetRelationship(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2)
{
	RelationshipManager.ResetRelationship(empire1->GetEmpireID(), empire2->GetEmpireID());
	ApplyPersonalityEffect(empire1, empire2);
	ApplyArchetypeModifier(empire1, empire2);
}

