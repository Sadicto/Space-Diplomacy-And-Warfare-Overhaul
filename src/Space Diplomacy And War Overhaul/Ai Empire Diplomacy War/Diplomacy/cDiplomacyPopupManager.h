#pragma once

#include <Spore\BasicIncludes.h>

#define cDiplomacyPopupManagerPtr intrusive_ptr<cDiplomacyPopupManager>

/// Manages the creation and display of diplomacy popups.
class cDiplomacyPopupManager 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cDiplomacyPopupManager");
	
	cDiplomacyPopupManager(ResourceKey spacePopUpsConfigKey);
	~cDiplomacyPopupManager();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	/// @brief Formats a popup message by replacing a specific placeholder with information from a given empire.
	/// @param empire
	/// @param tokenIndex Index of the placeholder token to replace (e.g., 2 replaces [empire2] and [archetype2])
	/// @param messageText the text to format
	void FormatDiplomaticActionMessage(Simulator::cEmpire* empire, int tokenIndex, eastl::string16& messageText);


	/// @brief Formats a popup message about the diplomatic action between two empires.
	/// @param empire1
	/// @param empire2
	/// @param messageText the text to format
	void FormatDiplomaticActionMessage(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2, eastl::string16& messageText);

	/// @brief Shows a popup notification to the player.
	/// @param eventKey.
	/// @param formattedText.
	void ShowPopup(ResourceKey eventKey, eastl::string16 popupText);

	/// @brief Notification when two AI empires form an alliance.
	/// @param empire1
	/// @param empire2
	void ShowFormAllianceAI(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Notification when an AI empire improves its relationship with the player.
	/// @param empire
	void ShowAIRelationImproved(Simulator::cEmpire* empire);

	/// @brief Notification when two AI empires break an alliance.
	/// @param empire1
	/// @param empire2
	void ShowBreakAllianceAI(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Notification when two AI empires break an alliance due to conflicted alliances.
	/// @param empire1
	/// @param empire2
	void ShowConflictBreakAllianceAI(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Notification when an alliance breaks due to conflicted alliances with the player.
	/// @param empire
	void ShowConflictBreakAlliancePlayer(Simulator::cEmpire* empire);

	/// @brief Shows a notification when two AI empires declare war on each other.
	/// @param empire1
	/// @param empire2
	void ShowDeclareWarAI(Simulator::cEmpire* empire1, Simulator::cEmpire* empire2);

	/// @brief Shows a notification when an AI declares war to the player.
	/// @param empire
	void ShowDeclareWarPlayer(Simulator::cEmpire* empire);

	/// @brief Shows a notification when the player has an unstable alliance.
	/// @param empire
	void ShowUnstableAlliance(Simulator::cEmpire* empire);

	/// @brief Shows a notification when the player has a hostile alliance.
	/// @param empire
	void ShowHostileAlliance(Simulator::cEmpire* empire);

	// Popup text: AI vs AI alliance ended due to conflict.
	eastl::string16 AllianceConflictAiAiText;

	// Popup text: AI ended alliance with player due to conflict.
	eastl::string16 AllianceConflictAiPlayerText;

	// Popup text: AI formed an alliance with another AI.
	eastl::string16 AllianceDeclaredAiAiText;

	// Popup text: AI ended alliance with another AI.
	eastl::string16 AllianceEndedAiAiText;

	// Popup text: AI is hostile towards the player due to low affinity.
	eastl::string16 HostileAllianceAiPlayerText;

	// Popup text: AI improved relations with the player.
	eastl::string16 RelationImprovedAiPlayer;

	// Popup text: AI declared war on another AI.
	eastl::string16 WarDeclaredAiAiText;

	// Popup text: AI declared war on the player.
	eastl::string16 WarDeclaredAiPlayerText;

	// Popup text: AI weakened alliance with the player due to low affinity.
	eastl::string16 WeakAllianceAiPlayerText;


};
