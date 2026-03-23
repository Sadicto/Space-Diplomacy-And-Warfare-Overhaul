#include "stdafx.h"
#include "cAffinityModifier.h"

cAffinityModifier::cAffinityModifier()
{
}


cAffinityModifier::~cAffinityModifier()
{
}

// For internal use, do not modify.
int cAffinityModifier::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cAffinityModifier::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cAffinityModifier::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAffinityModifier);
	CLASS_CAST(cAffinityModifier);
	return nullptr;
}

bool cAffinityModifier::Active(const AffinityModifierContext& context) {
	return GetAffinityGain(context) != 0;
}

cPersistedDiplomacyEvent* cAffinityModifier::GetPersistedDiplomacyEventByType(const AffinityModifierContext& context, uint32_t eventNounId){
	cPersistedDiplomacyEventPtr  persistedEventOfType = nullptr;
	for (cPersistedDiplomacyEventPtr diplomacyEvent : context.diplomacyEvents) {
		if (diplomacyEvent->GetNounID() == eventNounId) {
			persistedEventOfType = diplomacyEvent;
			break;
		}
	}
	return persistedEventOfType.get();
}

int cAffinityModifier::CalculateAffinityGain(const AffinityModifierContext& context, uint32_t eventCreationTime){
	uint32_t elapsedTime = max(context.currentTime - eventCreationTime, uint32_t(1));
	if (context.affinityConfig->AffinityUpgrades(GetAffinityModifier())) {
		return min(context.affinityConfig->GetAffinityGain(GetAffinityModifier()), int(elapsedTime / context.affinityConfig->GetUpgradeTime(GetAffinityModifier())));
	}
	else if (context.affinityConfig->AffinityDecays(GetAffinityModifier())) {
		return max(0, context.affinityConfig->GetAffinityGain(GetAffinityModifier()) - int(elapsedTime / context.affinityConfig->GetDecayTime(GetAffinityModifier())));
	}
	return 0;
}

int cAffinityModifier::GetAffinityGain(const AffinityModifierContext& context) {
	return context.affinityConfig->GetAffinityGain(GetAffinityModifier());
}

bool cAffinityModifier::Upgrading(const AffinityModifierContext& context) {
	return context.affinityConfig->AffinityUpgrades(GetAffinityModifier());
}

uint32_t cAffinityModifier::CalculateUpgradeTime(const AffinityModifierContext& context, uint32_t eventCreationTime){
	uint32_t elapsedTime = max(context.currentTime - eventCreationTime, uint32_t(1));
	uint32_t baseUpgradeTime = context.affinityConfig->GetUpgradeTime(GetAffinityModifier());
	return  baseUpgradeTime - (elapsedTime % baseUpgradeTime);
}

uint32_t cAffinityModifier::GetUpgradeTime(const AffinityModifierContext& context) {
	return 0;
}

bool cAffinityModifier::Decaying(const AffinityModifierContext& context) {
	return context.affinityConfig->AffinityDecays(GetAffinityModifier());
}

uint32_t cAffinityModifier::CalculateDecayTime(const AffinityModifierContext& context, uint32_t eventCreationTime){
	uint32_t elapsedTime = max(context.currentTime - eventCreationTime, uint32_t(1));
	uint32_t baseDecayTime = context.affinityConfig->GetDecayTime(GetAffinityModifier());
	return baseDecayTime - (elapsedTime % baseDecayTime);
}

uint32_t cAffinityModifier::GetDecayTime(const AffinityModifierContext& context){
	return 0;
}

bool cAffinityModifier::PreventsWars(const AffinityModifierContext& context) {
	return context.affinityConfig->AffinityPreventsWars(GetAffinityModifier());
}

int cAffinityModifier::GetPriority(const AffinityModifierContext& context) {
	return context.affinityConfig->GetAffinityPriority(GetAffinityModifier());
}

bool cAffinityModifier::StableRelationsMutuallyExclusive(const AffinityModifierContext& context) {
	return context.affinityConfig->StableRelationsMutuallyExclusive(GetAffinityModifier());
}

bool cAffinityModifier::WarTogetherMutuallyExclusive(const AffinityModifierContext& context) {
	return context.affinityConfig->WarTogetherMutuallyExclusive(GetAffinityModifier());
}

AffinityModifierData cAffinityModifier::GetAffinityModifierData(const AffinityModifierContext& context){
	AffinityModifierData data;
	data.affinityModifier = GetAffinityModifier();
	data.active = Active(context);
	data.affinityGain = GetAffinityGain(context);
	data.upgrading = Upgrading(context);
	data.upgradeTime = GetUpgradeTime(context);
	data.decaying = Decaying(context);
	data.decayTime = GetDecayTime(context);
	data.priority = GetPriority(context);
	data.stableRelationsMutuallyExclusive = StableRelationsMutuallyExclusive(context);
	data.warTogetherMutuallyExclusive = WarTogetherMutuallyExclusive(context);
	return data;
}
