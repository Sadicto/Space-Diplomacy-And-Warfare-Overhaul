#include "stdafx.h"
#include "AffinityTextProc.h"
#include <Spore-Mod-Utils/Include/SporeModUtils.h>
using namespace SporeModUtils;

AffinityTextProc::AffinityTextProc(IWindow* mainWindow, cEmpireRelationsAnalyzer* empireRelationsAnalyzer, ResourceKey affinityTextConfigKey)
{
	this->mainWindow = mainWindow;
	this->tooltipWindow = mainWindow->FindWindowByID(0x2A2D1FD2);
	tooltipWindow->SetVisible(false);
	this->textWindow = mainWindow->FindWindowByID(0xAE85024E);
	//textWindow->FindWindowByID(0xFA2B32AF)->SetVisible(false);
	this->empireRelationsAnalyzer = empireRelationsAnalyzer;

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
		App::ConsolePrintF("Mouse enter");
		tooltipWindow->SetVisible(true);
		return true;
	}
	else if (message.IsType(UTFWin::MessageType::kMsgMouseLeave)) {
		App::ConsolePrintF("Mouse leave");
		tooltipWindow->SetVisible(false);
		return true;
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}

void AffinityTextProc::SetAffinityText(uint32_t empireID) {
	Simulator::cEmpire* empire = StarManager.GetEmpire(empireID);
	if (EmpireUtils::ValidNpcEmpire(empire)) {
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

		Color verde(0, 255, 0, 255);
		textWindow->SetCaption(str16.c_str());
		if (affinity <= -2)
			textWindow->SetShadeColor(red);
		else if (affinity == -1)
			textWindow->SetShadeColor(orange);
		else if (affinity == 0)
			textWindow->SetShadeColor(yellow);
		else if (affinity == 1)
			textWindow->SetShadeColor(cyan);
		else if (affinity >= 2)
			textWindow->SetShadeColor(green);

		textWindow->SetVisible(true);
	}
	else {
		textWindow->SetVisible(false);
	}
}
