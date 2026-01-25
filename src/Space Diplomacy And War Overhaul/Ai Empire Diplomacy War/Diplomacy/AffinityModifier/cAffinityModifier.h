#pragma once

#include <Spore\BasicIncludes.h>
#include "..\IAffinityModifier.h"

#define cAffinityModifierPtr intrusive_ptr<cAffinityModifier>

// Abstract base class for affinity modifiers.
// Provides standard implementations for most IAffinityModifier methods.
class cAffinityModifier 
	: public IAffinityModifier
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyOverhaul::cAffinityModifier");
	
	cAffinityModifier();
	~cAffinityModifier();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	virtual bool Active(const AffinityModifierContext& context) override;

	virtual cPersistedDiplomacyEvent* GetPersistedDiplomacyEventByType(const AffinityModifierContext& context, uint32_t eventNounId);
	
	// Calculates the affinity gain based on elapsed time since event creation.
	virtual int CalculateAffinityGain(const AffinityModifierContext& context, uint32_t eventCreationTime);

	virtual int GetAffinityGain(const AffinityModifierContext& context) override;

	virtual bool Upgrading(const AffinityModifierContext& context) override;

	// Calculates the remaining time until the next upgrade step.
	virtual uint32_t CalculateUpgradeTime(const AffinityModifierContext& context, uint32_t eventCreationTime);

	virtual uint32_t GetUpgradeTime(const AffinityModifierContext& context) override;

	virtual bool Decaying(const AffinityModifierContext& context) override;

	// Calculates the remaining time until the next decay step.
	virtual uint32_t CalculateDecayTime(const AffinityModifierContext& context, uint32_t eventCreationTime);

	virtual uint32_t GetDecayTime(const AffinityModifierContext& context) override;

	virtual bool PreventsWars(const AffinityModifierContext& context) override;

	virtual int GetPriority(const AffinityModifierContext& context) override;

	virtual bool StableRelationsMutuallyExclusive(const AffinityModifierContext& context) override;

	virtual bool WarTogetherMutuallyExclusive(const AffinityModifierContext& context) override;

	virtual AffinityModifierData GetAffinityModifierData(const AffinityModifierContext& context) override;
};
