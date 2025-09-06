#include "stdafx.h"
#include "cEmpireRelationshipController.h"

cEmpireRelationshipController::cEmpireRelationshipController(cDiplomacyEffectAnalyzer* diplomacyEffectAnalyzer)
{
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

