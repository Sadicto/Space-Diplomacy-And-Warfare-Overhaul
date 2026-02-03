#include "stdafx.h"
#include "AffinityTextProc.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace SporeModUtils;

AffinityTextProc::AffinityTextProc(IWindow* affinityTooltipMainWindow, cEmpireRelationsAnalyzer* empireRelationsAnalyzer, ResourceKey affinityTextConfigKey)
{
	this->affinityTooltipMainWindow = affinityTooltipMainWindow;
	this->affinityTooltipNumberWindow = affinityTooltipMainWindow->FindWindowByID(0xAE85024E);
	this->affinityRolloverMainWindow = nullptr;
	this->affinityRolloverNumberWindow = nullptr;
	this->affinityRolloverModifiersWindow = nullptr;
	this->affinityRolloverLayout = nullptr;

	this->empireRelationsAnalyzer = empireRelationsAnalyzer;
	currentEmpire = nullptr;
	currentAffinity = 0;

	PropertyListPtr affinityTextConfig;
	PropManager.GetPropertyList(affinityTextConfigKey.instanceID, affinityTextConfigKey.groupID, affinityTextConfig);

	App::Property::GetUInt32(affinityTextConfig.get(), 0xFE7137B3, affinityRolloverLayoutId);

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

	App::Property::GetColorRGBA(affinityTextConfig.get(), 0xBD77BB98, aux);
	white = aux.ToIntColor();

	App::Property::GetColorRGBA(affinityTextConfig.get(), 0x360868E4, aux);
	gray = aux.ToIntColor();

	currentAffinityColor = yellow;

	App::Property::GetArrayString16(affinityTextConfig.get(), 0x557AFFAB, affinityTexts);
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
		if (EmpireUtils::ValidNpcEmpire(currentEmpire.get())){
			// Call SetAffinityTooltip first to update it in case the player did something to change the affinity during their conversation with currentEmpire.
			SetAffinityTooltip(currentEmpire->GetEmpireID());
			SetAffinityRollover();
		}
		return true;
	}
	else if (message.IsType(UTFWin::MessageType::kMsgMouseLeave)) {
		if (affinityRolloverLayout != nullptr) {
			WindowManager.GetMainWindow()->RemoveWindow(affinityRolloverMainWindow.get());
			affinityRolloverMainWindow.reset();
			affinityRolloverModifiersWindow.reset();
			affinityRolloverNumberWindow.reset();
			delete affinityRolloverLayout;
			affinityRolloverLayout = nullptr;
		}
		return true;
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}

void AffinityTextProc::SetAffinityTooltip(uint32_t empireID) {
	Simulator::cEmpire* empire = StarManager.GetEmpire(empireID);
	currentEmpire = empire;
	if (EmpireUtils::ValidNpcEmpire(empire)) {
		currentAffinity = empireRelationsAnalyzer->EmpiresAffinity(Simulator::GetPlayerEmpire(), empire);
		currentAffinityModifierData.clear();
		empireRelationsAnalyzer->GetEmpiresAffinityModifiersData(currentEmpire.get(), Simulator::GetPlayerEmpire(), currentAffinityModifierData);
		eastl::string tmp;
		if (currentAffinity > 0)
			tmp = "+" + eastl::to_string(currentAffinity);
		else
			tmp = eastl::to_string(currentAffinity);
		eastl::string16 s16;
		s16.assign_convert(tmp);
		affinityTooltipNumberWindow->SetCaption(s16.c_str());
		if (currentAffinity <= -2)
		{
			currentAffinityColor = red;
		}
		else if (currentAffinity == -1)
		{
			currentAffinityColor = orange;
		}
		else if (currentAffinity == 0)
		{
			currentAffinityColor = yellow;
		}
		else if (currentAffinity == 1)
		{
			currentAffinityColor = cyan;
		}
		else if (currentAffinity >= 2)
		{
			currentAffinityColor = green;
		}
		affinityTooltipNumberWindow->SetShadeColor(currentAffinityColor);
		affinityTooltipNumberWindow->SetVisible(true);
	}
	else {
		currentAffinity = 0;
		currentAffinityModifierData.clear();
		currentAffinityColor = yellow;
		affinityTooltipNumberWindow->SetVisible(false);
	}
}

eastl::string16 AffinityTextProc::GetAffinityModifierText(AffinityModifier affinityModifier) {
	return affinityTexts[int(affinityModifier)];
}

IWindow* AffinityTextProc::GetUnusedAffinityModifier() {
	for (IWindow* children : affinityRolloverModifiersWindow->children()) {
		if (children->GetControlID() == 0xE2D40949 && !children->IsVisible()) {
			return children;
		}
	}
	return nullptr;
}

void AffinityTextProc::ResetAffinityRollover() {
	for (IWindow* children : affinityRolloverModifiersWindow->children()) {
		if (children->GetControlID() == 0xE2D40949) {
			Math::Rectangle area = children->GetArea();
			area.y1 = 0;
			area.y2 = area.y1 + 21;
			children->SetArea(area);
			children->SetVisible(false);
		}
	}
}

void AffinityTextProc::SetAffinityRollover() {
	if (EmpireUtils::ValidNpcEmpire(currentEmpire.get())) {
		affinityRolloverLayout = new UTFWin::UILayout();
		affinityRolloverLayout->LoadByID(affinityRolloverLayoutId);

		affinityRolloverMainWindow = affinityRolloverLayout->FindWindowByID(0x2A2D1FD2);
		affinityRolloverNumberWindow = affinityRolloverLayout->FindWindowByID(0xB64FAF85);
		affinityRolloverModifiersWindow = affinityRolloverLayout->FindWindowByID(0xFB0CF1A7);

		Math::Rectangle affinityTooltipArea = affinityTooltipMainWindow->GetArea();
		Math::Point tooltipBottomRight = affinityTooltipMainWindow->ToGlobalCoordinates(affinityTooltipArea.GetBottomRight());
		// Align the rollover top with the relations rollover top.
		float rolloverY = tooltipBottomRight.y -6.5f;
		// Align the rollover center with the tooltip center.
		float rolloverX = tooltipBottomRight.x - affinityRolloverMainWindow->GetArea().GetWidth() / 2.0f + affinityTooltipArea.GetWidth() / 2.0f;
		affinityRolloverMainWindow->SetLocation(rolloverX, rolloverY);
		ResetAffinityRollover();

		eastl::string tmp = eastl::to_string(currentAffinity);
		eastl::string16 s16;
		s16.assign_convert(tmp);

		affinityRolloverNumberWindow->SetCaption(s16.c_str());
		affinityRolloverNumberWindow->SetShadeColor(currentAffinityColor);

		int modifiersCount = 0;
		for (const AffinityModifierData& affinityModifierData : currentAffinityModifierData) {
			if (affinityModifierData.active) {
				// decay and upgrade.
				IWindow* modifierUI = GetUnusedAffinityModifier();
				Math::Rectangle area = modifierUI->GetArea();
				area.y1 = 21.0f * modifiersCount;
				area.y2 = area.y1 + 21;
				modifierUI->SetArea(area);
				modifierUI->SetCaption(GetAffinityModifierText(affinityModifierData.affinityModifier).c_str());

				eastl::string str;

				if (affinityModifierData.affinityGain > 0)
					str = "+" + eastl::to_string(affinityModifierData.affinityGain);
				else
					str = eastl::to_string(affinityModifierData.affinityGain);

				eastl::basic_string<char16_t> str16;
				str16.reserve(str.size());
				for (char c : str)
					str16.push_back(static_cast<char16_t>(c));

				IWindow* affinityModifierNumber = modifierUI->FindWindowByID(0xD08B205F);
				affinityModifierNumber->SetCaption(str16.c_str());
				Color affinityModifierColor = Color(0,0,0,0);
				if (affinityModifierData.effective) {
					modifierUI->SetShadeColor(white);
					if (affinityModifierData.affinityGain > 0) {
						affinityModifierColor = green;
					}
					else if (affinityModifierData.affinityGain == 0) {
						affinityModifierColor = yellow;
					}
					else if (affinityModifierData.affinityGain < 0) {
						affinityModifierColor = red;
					}
				}

				else {
					modifierUI->SetShadeColor(gray);
					affinityModifierColor = white;
				}

				affinityModifierNumber->SetShadeColor(affinityModifierColor);
				modifierUI->SetVisible(true);
				modifiersCount++;
			}
		}

		Math::Rectangle area = affinityRolloverMainWindow->GetArea();
		area.y2 = area.y1 + 6 + 50 + 21 * modifiersCount;
		affinityRolloverMainWindow->SetArea(area);
		affinityRolloverMainWindow->SetVisible(true);
	}
}
