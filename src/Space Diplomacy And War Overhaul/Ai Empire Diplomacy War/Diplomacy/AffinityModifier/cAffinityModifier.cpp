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

cPersistedDiplomacyEvent* cAffinityModifier::GetAsociatedPersistedDiplomacyEvent(const AffinityModifierContext& context){
	cPersistedDiplomacyEventPtr  asociatedPersistedEvent = nullptr;
	for (cPersistedDiplomacyEventPtr diplomacyEvent : context.diplomacyEvents) {
		if (diplomacyEvent->GetDiplomacyEventData()->GetAffinityModifier() == GetAffinityModifier()) {
			asociatedPersistedEvent = diplomacyEvent;
			break;
		}
	}
	return asociatedPersistedEvent.get();
}

int cAffinityModifier::GetAffinityGain(const AffinityModifierContext& context) {
	if (context.affinityConfig->AffinityUpgrades(GetAffinityModifier()) || context.affinityConfig->AffinityExpires(GetAffinityModifier())) {
		cPersistedDiplomacyEvent* assoaciatedDiplomacyEvent = GetAsociatedPersistedDiplomacyEvent(context);
		if (assoaciatedDiplomacyEvent != nullptr) {
			return assoaciatedDiplomacyEvent->GetDiplomacyEventData()->GetAffinityGain();
		}
		else {
			return 0;
		}
	}
	else {
		return context.affinityConfig->GetAffinityGain(GetAffinityModifier());
	}
}

bool cAffinityModifier::Upgrading(const AffinityModifierContext& context) {
	return context.affinityConfig->AffinityUpgrades(GetAffinityModifier()) && GetAffinityGain(context) != context.affinityConfig->GetAffinityGain(GetAffinityModifier());
}

uint32_t cAffinityModifier::GetUpgradeTime(const AffinityModifierContext& context) {
	if (Upgrading(context)) {
		cPersistedDiplomacyEvent* assoaciatedDiplomacyEvent = GetAsociatedPersistedDiplomacyEvent(context);
		if (assoaciatedDiplomacyEvent != nullptr) {
			return assoaciatedDiplomacyEvent->GetExpirationTime() - context.currentTime;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

bool cAffinityModifier::Expiring(const AffinityModifierContext& context) {
	return context.affinityConfig->AffinityExpires(GetAffinityModifier());
}

uint32_t cAffinityModifier::GetExpireTime(const AffinityModifierContext& context){
	if (Expiring(context)) {
		cPersistedDiplomacyEvent* assoaciatedDiplomacyEvent = GetAsociatedPersistedDiplomacyEvent(context);
		if (assoaciatedDiplomacyEvent != nullptr) {
			return assoaciatedDiplomacyEvent->GetExpirationTime() - context.currentTime;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
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
	data.expiring = Expiring(context);
	data.expireTime = GetExpireTime(context);
	data.priority = GetPriority(context);
	data.stableRelationsMutuallyExclusive = StableRelationsMutuallyExclusive(context);
	data.warTogetherMutuallyExclusive = WarTogetherMutuallyExclusive(context);
	return data;
}
