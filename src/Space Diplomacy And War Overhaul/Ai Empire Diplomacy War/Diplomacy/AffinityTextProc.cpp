#include "stdafx.h"
#include "AffinityTextProc.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace SporeModUtils;

AffinityTextProc::AffinityTextProc(IWindow* mainWindow, cEmpireRelationsAnalyzer* empireRelationsAnalyzer, ResourceKey affinityTextConfigKey)
{
	this->mainWindow = mainWindow;
	this->mainAffinityText = mainWindow->FindWindowByID(0xAE85024E);
	this->tooltipWindow = mainWindow->FindWindowByID(0x2A2D1FD2);
	this->secondaryAffinityText = tooltipWindow->FindWindowByID(0xB64FAF85);
	tooltipWindow->SetVisible(false);
	this->tooltipModifiersWindow = tooltipWindow->FindWindowByID(0xFB0CF1A7);
	ResetAffinityToltip();


	//textWindow->FindWindowByID(0xFA2B32AF)->SetVisible(false);
	this->empireRelationsAnalyzer = empireRelationsAnalyzer;
	currentEmpire = nullptr;

	PropertyListPtr affinityTextConfig;
	PropManager.GetPropertyList(affinityTextConfigKey.instanceID, affinityTextConfigKey.groupID, affinityTextConfig);
	ColorRGBA aux;
	App::Property::GetColorRGBA(affinityTextConfig.get(), 0x3A72D292, aux);
	red = aux.ToIntColor();

	App::Property::GetColorRGBA(affinityTextConfig.get(), 0x513AA127, aux);
	orange = aux.ToIntColor();

	App::Property::GetColorRGBA(affinityTextConfig.get(), 0xA55A8819, aux);
	yellow = aux.ToIntColor();

	App::Property::GetColorRGBA(affinityTextConfig.get(), 0xFAA33C0C, aux);
	cyan = aux.ToIntColor();

	App::Property::GetColorRGBA(affinityTextConfig.get(), 0xF73297B2, aux);
	green = aux.ToIntColor();

	App::Property::GetString16(affinityTextConfig.get(), 0x4DB402BB, archetypeAffinityText);
	App::Property::GetString16(affinityTextConfig.get(), 0x529A946B, commonEnemyText);
	App::Property::GetString16(affinityTextConfig.get(), 0x58BA9D91, commonAllyText);
	App::Property::GetString16(affinityTextConfig.get(), 0x01FFE8D2, warWithAllyText);
}


AffinityTextProc::~AffinityTextProc()
{
}

// For internal use, do not modify.
int AffinityTextProc::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int AffinityTextProc::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* AffinityTextProc::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(AffinityTextProc);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int AffinityTextProc::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced | kEventRefresh;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool AffinityTextProc::HandleUIMessage(IWindow* window, const Message& message)
{
	if (message.IsType(UTFWin::MessageType::kMsgMouseEnter)) {
		SetAffinityTooltip();
		return true;
	}
	else if (message.IsType(UTFWin::MessageType::kMsgMouseLeave)) {
		tooltipWindow->SetVisible(false);
		return true;
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}

void AffinityTextProc::SetAffinityText(uint32_t empireID) {
	Simulator::cEmpire* empire = StarManager.GetEmpire(empireID);
	if (EmpireUtils::ValidNpcEmpire(empire)) {
		currentEmpire = empire;
		int affinity = empireRelationsAnalyzer->EmpiresAffinity(Simulator::GetPlayerEmpire(), empire);
		eastl::string str;
		if (affinity > 0)
			str = "+" + eastl::to_string(affinity);
		else
			str = eastl::to_string(affinity);

		eastl::basic_string<char16_t> str16;
		str16.reserve(str.size());
		for (char c : str)
			str16.push_back(static_cast<char16_t>(c));

		// For the secondary text, remove the '+' sign if the number is positive
		eastl::basic_string<char16_t> str16Secondary = str16;
		if (!str16Secondary.empty() && str16Secondary[0] == u'+') {
			str16Secondary.erase(0, 1);
		}


		mainAffinityText->SetCaption(str16.c_str());
		secondaryAffinityText->SetCaption(str16Secondary.c_str());
		if (affinity <= -2) 
		{
			mainAffinityText->SetShadeColor(red);
			secondaryAffinityText->SetShadeColor(red);
		}
		else if (affinity == -1)
		{
			mainAffinityText->SetShadeColor(orange);
			secondaryAffinityText->SetShadeColor(orange);
		}
		else if (affinity == 0)
		{
			mainAffinityText->SetShadeColor(yellow);
			secondaryAffinityText->SetShadeColor(yellow);
		}
		else if (affinity == 1)
		{
			mainAffinityText->SetShadeColor(cyan);
			secondaryAffinityText->SetShadeColor(cyan);
		}
		else if (affinity >= 2)
		{
			mainAffinityText->SetShadeColor(green);
			secondaryAffinityText->SetShadeColor(green);
		}

		mainAffinityText->SetVisible(true);
	}
	else {
		currentEmpire = nullptr;
		mainAffinityText->SetVisible(false);
	}
}

eastl::string16 AffinityTextProc::GetAffinityModifierText(AffinityModifier affinityModifier) {
	switch (affinityModifier) {
	case AffinityModifier::ArchetypeAffinity:
		return archetypeAffinityText;
	case AffinityModifier::CommonEnemy:
		return commonEnemyText;
	case AffinityModifier::CommonAlly:
		return commonAllyText;
	case AffinityModifier::WarWithAlly:
		return warWithAllyText;
	default:
		return u"";
	}
}

IWindow* AffinityTextProc::GetUnusedAffinityModifier() {
	for (IWindow* children : tooltipModifiersWindow->children()) {
		if (children->GetControlID() == 0xE2D40949 && !children->IsVisible()) {
			return children;
		}
	}
	return nullptr;
}

void AffinityTextProc::ResetAffinityToltip() {
	for (IWindow* children : tooltipModifiersWindow->children()) {
		if (children->GetControlID() == 0xE2D40949) {
			Math::Rectangle area = children->GetArea();
			area.y1 = 0;
			area.y2 = area.y1 + 21;
			children->SetArea(area);
			children->SetVisible(false);
		}
	}
}

void AffinityTextProc::SetAffinityTooltip() {
	if (EmpireUtils::ValidNpcEmpire(currentEmpire.get())) {
		ResetAffinityToltip();
		eastl::vector<pair<AffinityModifier, int>> affinityModifiers = 
			empireRelationsAnalyzer->GetEmpiresAffinityModifier(currentEmpire.get(), Simulator::GetPlayerEmpire());

		int modifiersCount = 0;
		for (pair<AffinityModifier, int> modifier : affinityModifiers) {
			if (modifier.second != 0) {
				IWindow* modifierUI = GetUnusedAffinityModifier();
				Math::Rectangle area = modifierUI->GetArea();
				area.y1 = 21.0f * modifiersCount;
				area.y2 = area.y1 + 21;
				modifierUI->SetArea(area);
				modifierUI->SetCaption(GetAffinityModifierText(modifier.first).c_str());

				eastl::string str;

				if (modifier.second > 0)
					str = "+" + eastl::to_string(modifier.second);
				else
					str = eastl::to_string(modifier.second);

				eastl::basic_string<char16_t> str16;
				str16.reserve(str.size());
				for (char c : str)
					str16.push_back(static_cast<char16_t>(c));

				IWindow* affinityModifierText = modifierUI->FindWindowByID(0xD08B205F);
				affinityModifierText->SetCaption(str16.c_str());

				Color color = (modifier.second > 0) ? green
					: red;
				affinityModifierText->SetShadeColor(color);
				modifierUI->SetVisible(true);
				modifiersCount++;
			}
		}

		Math::Rectangle area = tooltipWindow->GetArea();
		area.y2 = area.y1 + 6 + 50 + 21 * modifiersCount;
		tooltipWindow->SetArea(area);
		tooltipWindow->SetVisible(true);
	}
	else {
		tooltipWindow->SetVisible(false);
	}
}
