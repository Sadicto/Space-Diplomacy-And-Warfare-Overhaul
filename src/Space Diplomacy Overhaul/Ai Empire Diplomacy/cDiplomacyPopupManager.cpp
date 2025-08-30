#include "stdafx.h"
#include "cDiplomacyPopupManager.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"

cDiplomacyPopupManager::cDiplomacyPopupManager(ResourceKey spacePopUpsConfigKey){
	PropertyListPtr spacePopUpsConfigProp;
	PropManager.GetPropertyList(spacePopUpsConfigKey.instanceID, spacePopUpsConfigKey.groupID, spacePopUpsConfigProp);
	App::Property::GetString16(spacePopUpsConfigProp.get(), 0xBCF79B3B, AllianceConflictAiAiText);
	App::Property::GetString16(spacePopUpsConfigProp.get(), 0x07335A90, AllianceConflictAiPlayerText);
	App::Property::GetString16(spacePopUpsConfigProp.get(), 0x56F71779, AllianceDeclaredAiAiText);
	App::Property::GetString16(spacePopUpsConfigProp.get(), 0x8CEF63AB, AllianceEndedAiAiText);
	App::Property::GetString16(spacePopUpsConfigProp.get(), 0xA1003F93, HostileAllianceAiPlayerText);
	App::Property::GetString16(spacePopUpsConfigProp.get(), 0x5A02F668, RelationImprovedAiPlayer);
	App::Property::GetString16(spacePopUpsConfigProp.get(), 0x3AEEDDBA, WarDeclaredAiAiText);
	App::Property::GetString16(spacePopUpsConfigProp.get(), 0x1AAB0A46, WeakAllianceAiPlayerText);
}


cDiplomacyPopupManager::~cDiplomacyPopupManager()
{
}

// For internal use, do not modify.
int cDiplomacyPopupManager::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cDiplomacyPopupManager::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cDiplomacyPopupManager::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cDiplomacyPopupManager);
	return nullptr;
}

void cDiplomacyPopupManager::FormatDiplomaticActionMessage(Simulator::cEmpire* empire, int tokenIndex, eastl::string16& messageText) {

	eastl::string16 tokenString(eastl::string16::CtorConvert{}, eastl::to_string(tokenIndex).c_str());

	eastl::string16 relationToken = eastl::string16(u"[relation") + tokenString + eastl::string16(u"]");

	eastl::string16 empireToken = eastl::string16(u"[empire") + tokenString + eastl::string16(u"]");

	eastl::string16 archetypeToken = eastl::string16(u"[archetype") + tokenString + eastl::string16(u"]");

	eastl::string16 relationName = SporeModUtils::DiplomacyUtils::GetPlayerRelationString(empire);
	eastl::string16 empireName = empire->mEmpireName;
	eastl::string16 archetypeName = SporeModUtils::ArchetypeUtils::ArchetypeToString(empire->mArchetype) ;

	size_t pos = messageText.find(relationToken);
	if (pos != eastl::string16::npos) {
		messageText.replace(pos, relationToken.size(), relationName);
	}

	pos = messageText.find(empireToken);
	if (pos != eastl::string16::npos) {
		messageText.replace(pos, empireToken.size(), empireName);
	}

	pos = messageText.find(archetypeToken);
	if (pos != eastl::string16::npos) {
		messageText.replace(pos, archetypeToken.size(), archetypeName);
	}

}

void cDiplomacyPopupManager::FormatDiplomaticActionMessage(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, eastl::string16& messageText) {
	FormatDiplomaticActionMessage(empire1, 1, messageText);
	FormatDiplomaticActionMessage(empire2, 2, messageText);
}

void cDiplomacyPopupManager::ShowPopup(ResourceKey eventKey, eastl::string16 popupText) {
	uint32_t eventID = UIEventLog.ShowEvent(eventKey.instanceID, eventKey.groupID);
	size_t start = 0;
	while (start < popupText.size() && popupText[start] == u' ') {
		++start;
	}

	if (start > 0) {
		popupText = popupText.substr(start);
	}

	if (!popupText.empty() && popupText[0] >= u'a' && popupText[0] <= u'z') {
		popupText[0] = popupText[0] - u'a' + u'A';
	}
	UIEventLog.SetDescription(eventID, popupText.c_str());
}

void cDiplomacyPopupManager::ShowFormAllianceAI(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	eastl::string16 popupText = AllianceDeclaredAiAiText;
	FormatDiplomaticActionMessage(empire1, empire2, popupText);
	ResourceKey eventKey = ResourceKey(id("AllianceDeclaredAiAi"), 0, id("SdoSpacePopUps"));
	ShowPopup(eventKey, popupText);
}

void cDiplomacyPopupManager::ShowAIRelationImproved(Simulator::cEmpire* empire) {
	eastl::string16 popupText = RelationImprovedAiPlayer;
	FormatDiplomaticActionMessage(empire, 1, popupText);
	ResourceKey eventKey = ResourceKey(id("RelationImprovedAiPlayer"), 0, id("SdoSpacePopUps"));
	ShowPopup(eventKey, popupText);
}

void cDiplomacyPopupManager::ShowBreakAllianceAI(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	eastl::string16 popupText = AllianceEndedAiAiText;
	FormatDiplomaticActionMessage(empire1, empire2, popupText);
	ResourceKey eventKey = ResourceKey(id("AllianceEndedAiAi"), 0, id("SdoSpacePopUps"));
	ShowPopup(eventKey, popupText);
}

void cDiplomacyPopupManager::ShowConflictBreakAllianceAI(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	eastl::string16 popupText = AllianceConflictAiAiText;
	FormatDiplomaticActionMessage(empire1, empire2, popupText);
	ResourceKey eventKey = ResourceKey(id("AllianceConflictAiAi"), 0, id("SdoSpacePopUps"));
	ShowPopup(eventKey, popupText);
}

void cDiplomacyPopupManager::ShowConflictBreakAlliancePlayer(Simulator::cEmpire* empire) {
	eastl::string16 popupText = AllianceConflictAiPlayerText;
	FormatDiplomaticActionMessage(empire, 1, popupText);
	ResourceKey eventKey = ResourceKey(id("AllianceConflictAiPlayer"), 0, id("SdoSpacePopUps"));
	ShowPopup(eventKey, popupText);
}

void cDiplomacyPopupManager::ShowDeclareWarAI(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2) {
	eastl::string16 popupText = WarDeclaredAiAiText;
	FormatDiplomaticActionMessage(empire1, empire2, popupText);
	ResourceKey eventKey = ResourceKey(id("WarDeclaredAiAi"), 0, id("SdoSpacePopUps"));
	ShowPopup(eventKey, popupText);
}

void cDiplomacyPopupManager::ShowUnstableAlliance(Simulator::cEmpire* empire) {
	eastl::string16 popupText = WeakAllianceAiPlayerText;
	FormatDiplomaticActionMessage(empire, 1, popupText);
	ResourceKey eventKey = ResourceKey(id("WeakAllianceAiPlayer"), 0, id("SdoSpacePopUps"));
	ShowPopup(eventKey, popupText);
}

void cDiplomacyPopupManager::ShowHostileAlliance(Simulator::cEmpire* empire) {
	eastl::string16 popupText = HostileAllianceAiPlayerText;
	FormatDiplomaticActionMessage(empire, 1, popupText);
	ResourceKey eventKey = ResourceKey(id("HostileAllianceAiPlayer"), 0, id("SdoSpacePopUps"));
	ShowPopup(eventKey, popupText);
}

