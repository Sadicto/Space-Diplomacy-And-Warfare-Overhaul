#include "stdafx.h"
#include "AllianceEnemyButtonProc.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"

AllianceEnemyButtonProc::AllianceEnemyButtonProc()
{
	selectedEmpire = nullptr;
	lastCurrentStar = nullptr;
	buttonEnabled = false;
}


AllianceEnemyButtonProc::~AllianceEnemyButtonProc()
{
	DeleteAlliesAndEnemiesEffects();
}

// For internal use, do not modify.
int AllianceEnemyButtonProc::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int AllianceEnemyButtonProc::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* AllianceEnemyButtonProc::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(AllianceEnemyButtonProc);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int AllianceEnemyButtonProc::GetEventFlags() const
{
	return kEventFlagAdvanced | kEventFlagUpdate;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool AllianceEnemyButtonProc::HandleUIMessage(IWindow* window, const Message& message)
{
	if (message.IsType(678582774)) {
		if (message.ButtonSelect.isSelected) {
			buttonEnabled = true;
		}
		else if (!message.ButtonSelect.isSelected) {
			DeleteAlliesAndEnemiesEffects();
			buttonEnabled = false;
			lastCurrentStar.reset();
			selectedEmpire.reset();
		}
	}
	else if (message.IsType(UTFWin::MessageType::kMsgUpdate) && buttonEnabled) {
		Simulator::cStarRecord* currentStar = Simulator::GetActiveStarRecord();

		if (lastCurrentStar.get() != currentStar && currentStar != nullptr) {
			lastCurrentStar = currentStar;
			Simulator::cEmpire* empireInCurrentStar = StarManager.GetEmpire(currentStar->mEmpireID);

			if (selectedEmpire.get() != empireInCurrentStar) {
				DeleteAlliesAndEnemiesEffects();

				if (SporeModUtils::EmpireUtils::ValidNpcEmpire(empireInCurrentStar)) {
					selectedEmpire = empireInCurrentStar;
					if (SporeModUtils::DiplomacyUtils::PlayerContactedEmpire(empireInCurrentStar)) {
						CreateAlliesAndEnemiesEffecsForSelectedEmpire();
					}
				}
				else {
					selectedEmpire.reset();
				}
			}
		}
	}
	
	return false;
}

void AllianceEnemyButtonProc::CreateAlliesAndEnemiesEffecsForSelectedEmpire() {
	for (cEmpirePtr empireEnemy : selectedEmpire->mEnemies) {
		if (SporeModUtils::EmpireUtils::ValidNpcEmpire(empireEnemy.get(), true) /* && SporeModUtils::DiplomacyUtils::PlayerContactedEmpire(empireEnemy.get())*/) {
			for (cStarRecordPtr star : empireEnemy->mStars) {
				IVisualEffectPtr visualEffect;
				EffectsManager.CreateVisualEffect(0x38627EA3, 0, visualEffect);
				visualEffect->Start();
				Transform transform = Transform();
				transform.SetOffset(star->mPosition);
				visualEffect->SetSourceTransform(transform);
				alliesAndEnemiesEffects.push_back(visualEffect);
			}
		}
	}
	for (cEmpirePtr empireAlly : selectedEmpire->mAllies) {
		if (SporeModUtils::EmpireUtils::ValidNpcEmpire(empireAlly.get(), true) /* && SporeModUtils::DiplomacyUtils::PlayerContactedEmpire(empireAlly.get())*/) {
			for (cStarRecordPtr star : empireAlly->mStars) {
				IVisualEffectPtr visualEffect;
				EffectsManager.CreateVisualEffect(0xB7BB2907, 0, visualEffect);
				visualEffect->Start();
				Transform transform = Transform();
				transform.SetOffset(star->mPosition);
				visualEffect->SetSourceTransform(transform);
				alliesAndEnemiesEffects.push_back(visualEffect);
			}
		}
	}

}

void AllianceEnemyButtonProc::DeleteAlliesAndEnemiesEffects() {
	for (IVisualEffectPtr visualEffect : alliesAndEnemiesEffects) {
		if (visualEffect != nullptr) {
			visualEffect->Stop();
		}
	}
	alliesAndEnemiesEffects.clear();
}
