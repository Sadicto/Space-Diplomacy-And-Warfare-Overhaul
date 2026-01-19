#pragma once

#include <Spore\BasicIncludes.h>
#include "..\IAffinityModifier.h"

#define cAffinityModifierPtr intrusive_ptr<cAffinityModifier>

class cAffinityModifier 
	: public IAffinityModifier
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cAffinityModifier");
	
	cAffinityModifier();
	~cAffinityModifier();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	virtual bool Active(const AffinityModifierContext& context) override;

	virtual cPersistedDiplomacyEvent* GetAsociatedPersistedDiplomacyEvent(const AffinityModifierContext& context) override;

	virtual int GetAffinityGain(const AffinityModifierContext& context) override;

	virtual bool Upgrading(const AffinityModifierContext& context) override;

	virtual uint32_t GetUpgradeTime(const AffinityModifierContext& context) override;

	virtual bool Expiring(const AffinityModifierContext& context) override;

	virtual uint32_t GetExpireTime(const AffinityModifierContext& context) override;

	virtual bool PreventsWars(const AffinityModifierContext& context) override;

	virtual int GetPriority(const AffinityModifierContext& context) override;

	virtual bool StableRelationsMutuallyExclusive(const AffinityModifierContext& context) override;

	virtual bool WarTogetherMutuallyExclusive(const AffinityModifierContext& context) override;

	virtual AffinityModifierData GetAffinityModifierData(const AffinityModifierContext& context) override;
};
