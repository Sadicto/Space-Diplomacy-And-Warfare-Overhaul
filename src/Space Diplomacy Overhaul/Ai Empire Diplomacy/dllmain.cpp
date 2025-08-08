// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "DebugDiplomacy.h"
#include <Spore/Simulator/SubSystem/CommManager.h>
#include "cEmpireDiplomacyManager.h"
void Initialize()
{
	CheatManager.AddCheat("DebugDiplomacy", new DebugDiplomacy());
	cSimulatorSystem::Get()->AddStrategy(new cEmpireDiplomacyManager(), cEmpireDiplomacyManager::NOUN_ID);
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
}

void Dispose()
{
	// This method is called when the game is closing
}

member_detour(DeclareAlliance__detour, Simulator::cRelationshipManager, void(cEmpire* pEmpire1, cEmpire* pEmpire2)) {
	void detoured(cEmpire * pEmpire1, cEmpire * pEmpire2) {
		cEmpireDiplomacyManagerPtr EmpireDiplomacyManager = cEmpireDiplomacyManager::Get();
		if (pEmpire1 == GetPlayerEmpire() || pEmpire2 == GetPlayerEmpire()) {
			original_function(this, pEmpire1, pEmpire2);
			uint32_t playerEmpireId = SpacePlayerData::Get()->mPlayerEmpireID;
			bool encountered = EmpireDiplomacyManager->EmpireEncountered(pEmpire1) || EmpireDiplomacyManager->EmpireEncountered(pEmpire2);
			if (pEmpire1->GetEmpireID() != playerEmpireId && pEmpire2->GetEmpireID() != playerEmpireId && encountered) {
				uint32_t eventID = UIEventLog.ShowEvent(id("AllianceBetweenAiEmpires"), GroupIDs::SpacePopups);
				eastl::string16 empire1 = pEmpire1->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire1->mArchetype) + u")";
				eastl::string16 empire2 = pEmpire2->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire2->mArchetype) + u")";
				eastl::string16 str1 = u"The " + empire1 + u" and " + empire2 + u" have formed an alliance.";
				UIEventLog.SetDescription(eventID, str1.c_str());
			}
		}
	}
};

member_detour(BreakAlliance__detour, Simulator::cRelationshipManager, void(cEmpire* pEmpire1, cEmpire* pEmpire2)) {
	void detoured(cEmpire * pEmpire1, cEmpire * pEmpire2) {
		cEmpireDiplomacyManagerPtr EmpireDiplomacyManager = cEmpireDiplomacyManager::Get();
		if (pEmpire1 == GetPlayerEmpire() || pEmpire2 == GetPlayerEmpire()) {
			original_function(this, pEmpire1, pEmpire2);
			uint32_t playerEmpireId = SpacePlayerData::Get()->mPlayerEmpireID;
			bool encountered = EmpireDiplomacyManager->EmpireEncountered(pEmpire1) || EmpireDiplomacyManager->EmpireEncountered(pEmpire2);
			if (pEmpire1->GetEmpireID() != playerEmpireId && pEmpire2->GetEmpireID() != playerEmpireId && encountered) {
				uint32_t eventID = UIEventLog.ShowEvent(id("AllianceEndedBetweenAiEmpires"), GroupIDs::SpacePopups);
				eastl::string16 empire1 = pEmpire1->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire1->mArchetype) + u")";
				eastl::string16 empire2 = pEmpire2->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire2->mArchetype) + u")";
				eastl::string16 str1 = u"The alliance between the " + empire1 + u" and " + empire2 + u" has broken down.";
				UIEventLog.SetDescription(eventID, str1.c_str());
			}
		}
	}
};

member_detour(DeclareWar__detour, Simulator::cRelationshipManager, void(cEmpire* pEmpire1, cEmpire* pEmpire2)) {
	void detoured(cEmpire * pEmpire1, cEmpire * pEmpire2) {
		cEmpireDiplomacyManagerPtr EmpireDiplomacyManager = cEmpireDiplomacyManager::Get();
		if (pEmpire1 == GetPlayerEmpire() || pEmpire2 == GetPlayerEmpire()){
			if (!RelationshipManager.IsAtWar(pEmpire1, pEmpire2)) {
				original_function(this, pEmpire1, pEmpire2);
				uint32_t playerEmpireId = SpacePlayerData::Get()->mPlayerEmpireID;
				bool encountered = EmpireDiplomacyManager->EmpireEncountered(pEmpire1) || EmpireDiplomacyManager->EmpireEncountered(pEmpire2);
				if (pEmpire1->GetEmpireID() != playerEmpireId && pEmpire2->GetEmpireID() != playerEmpireId && encountered) {
					uint32_t eventID = UIEventLog.ShowEvent(id("WarDeclaredBetweenAiEmpires"), GroupIDs::SpacePopups);
					eastl::string16 empire1 = pEmpire1->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire1->mArchetype) + u")";
					eastl::string16 empire2 = pEmpire2->mEmpireName + u" (" + EmpireDiplomacyManager->ArchetypeToString(pEmpire2->mArchetype) + u")";
					eastl::string16 str1 = u"The " + empire1 + u" declared war on the " + empire2;
					UIEventLog.SetDescription(eventID, str1.c_str());
				}
			}
		}
	}
};

class cEmpireGrowthSim : public cGonzagoSimulator {
	eastl::vector<int> selfColorWeights; // 0x10.
	//uint32_t field_2c;
	//int field_30;
	eastl::vector<int> selfRelationshipWeights; // 0x24.
	//int field_2c; 
	int field_38;
	int field_3c;
	int field_40;
	int field_44;
	bool field_48;
	bool field_49;
	int minTimeEGS; // 0x4c.
	int maxTimeEGS; // 0x50.
	int NPCForcedTurnCount; // 0x54.
	int field_5c;
	int field_60;
	int field_64;
	int field_68;
	int field_6c;
	int field_70;
	int field_74;
	int field_78;
	int field_7c;
	int field_80;
	int field_84;
	int field_88;
	int field_8c;
	int field_90;
	int field_94;
	int field_98;
	eastl::vector<int> field_9c;

	float field_b0;
	float field_b4;
	float field_b8;
	float field_bc;
	float field_c0;
	float field_c4;
	float field_c8;
	float field_cc;
	float field_d0;
	float field_d4;
	float field_d8;
	float field_dc;
	float field_e0;
	float field_e4;
	float field_e8;
	float field_ec;
	float field_f0;
	float field_f4;
	float field_f8;
	float field_fc;
	float field_100;
	float field_104;
	float field_108;
	float field_10c;
	float field_110;
	float field_114;
	float field_118;
	float field_11c;
	float field_120;
	float field_124;
	float field_128;
	float field_12c;
	float field_130;
	float field_134;
	float field_138;
	float field_13c;
	float field_140;
	float field_144;
	float field_148;
	float field_14c;
	float field_150;
	float field_154;
	float field_158;
	float field_15c;
	float field_160;
	float field_164;
	float field_168;
	float field_16c;
	float field_170;
	float field_174;
	float field_178;
	float field_17c;
	float field_180;
	float field_184;
	float field_188;
	float field_18c;
	float field_190;
	float field_194;
	float field_198;
	float field_19c;
	float field_1a0;
	float field_1a4;
	float field_1a8;
	float field_1ac;
	float field_1b0;
	float field_1b4;
	float field_1b8;
	float field_1bc;
	float field_1c0;
	float field_1c4;
	float field_1c8;
	float field_1cc;
	float field_1d0;
	float field_1d4;
	float field_1d8;
	float field_1dc;
	float field_1e0;
	float field_1e4;
	float field_1e8;
	float field_1ec;
	float field_1f0;
	float field_1f4;
	float field_1f8;
	float field_1fc;
	float field_200;
	float field_204;
	float field_208;
	float field_20c;
	float field_210;
	float field_214;
	float field_218;
	float field_21c;
	float field_220;
	float field_224;
	float field_228;
	float field_22c;
	float field_230;
	float field_234;
	float field_238;
	float field_23c;
	float field_240;
	float field_244;
	float field_248;
	float field_24c;
	float field_250;
	float field_254;
	float field_258;
	float field_25c;
	float field_260;
	float field_264;
	float field_268;
	float field_26c;
	float field_270;
	float field_274;
	float field_278;
	float field_27c;
	float field_280;
	float field_284;
	float field_288;
	float field_28c;
	float field_290;
	float field_294;
	float field_298;
	float field_29c;
	float field_2a0;
	float field_2a4;
	float field_2a8;
	float field_2ac;
	float field_2b0;
	float field_2b4;
	float field_2b8;
	float field_2bc;
	float field_2c0;
	float field_2c4;
	float field_2c8;
	float field_2cc;
	float field_2d0;
	float field_2d4;
	float field_2d8;
	float field_2dc;
	float field_2e0;
	float field_2e4;
	float field_2e8;
	float field_2ec;
	float field_2f0;
	float field_2f4;
	float field_2f8;
	float field_2fc;
	float field_300;
	float field_304;
	float field_308;
	float field_30c;
	float field_310;
	float field_314;
	float field_318;
	float field_31c;
	float field_320;
	float field_324;
	float field_328;
	float field_32c;
	float field_330;
	float field_334;
	float field_338;
	float field_33c;
	float field_340;
	float field_344;
	float field_348;
	float field_34c;
	float field_350;
	float field_354;
	float field_358;
	float field_35c;
	float field_360;
	float field_364;
	float field_368;
	float field_36c;
	float field_370;
	float field_374;
	float field_378;
	float field_37c;
	float field_380;
	float field_384;
	float field_388;
	float field_38c;
	float field_390;
	float field_394;
	float field_398;
	float field_39c;
	float field_3a0;
	float field_3a4;
	float field_3a8;
	float field_3ac;
	float field_3b0;
	float field_3b4;
	float field_3b8;
	float field_3bc;
	float field_3c0;
	float field_3c4;
	float field_3c8;
	float field_3cc;
	float field_3d0;
	float field_3d4;
	float field_3d8;
	float field_3dc;
	float field_3e0;
	float field_3e4;
	float field_3e8;
	float field_3ec;
	float field_3f0;
	float field_3f4;
	float field_3f8;
	float field_3fc;
	float field_400;
	float field_404;
	float field_408;
	float field_40c;
	float field_410;
	float field_414;
	float field_418;
	float field_41c;
	float field_420;
	float field_424;
	float field_428;
	float field_42c;
	float field_430;
	float field_434;
	float field_438;
	float field_43c;
	float field_440;
	float field_444;
	float field_448;
	float field_44c;
	float field_450;
	float field_454;
	float field_458;
	float field_45c;
	float field_460;
	float field_464;
	float field_468;
	float field_46c;
	float field_470;
	float field_474;
	float field_478;
	float field_47c;
	float field_480;
	float field_484;
	float field_488;
	float field_48c;
	float field_490;
	float field_494;
	float field_498;
	float field_49c;
	float field_4a0;
	float field_4a4;
	float field_4a8;
	float field_4ac;
	float field_4b0;
	float field_4b4;
	float field_4b8;
	float field_4bc;
	float field_4c0;
	float field_4c4;
	float field_4c8;
	float field_4cc;
	float field_4d0;
	float field_4d4;
	float field_4d8;
	float field_4dc;
	float field_4e0;
	float field_4e4;
	float field_4e8;
	float field_4ec;
	float field_4f0;
	float field_4f4;
	float field_4f8;
	float field_4fc;
	float field_500;
	float field_504;
	float field_508;
	float field_50c;
	float field_510;
	float field_514;
	float field_518;
	float field_51c;
	float field_520;
	float field_524;
	float field_528;
	float field_52c;
	float field_530;
	float field_534;
	float field_538;
	float field_53c;
	float field_540;
	float field_544;
	float field_548;
	float field_54c;
	float field_550;
	float field_554;
	float field_558;
	float field_55c;
	float field_560;
	float field_564;
	float field_568;
	float field_56c;
	float field_570;
	float field_574;
	float field_578;
	float field_57c;
	float field_580;
	float field_584;
	float field_588;
	float field_58c;
	float field_590;
	float field_594;
	float field_598;
	float field_59c;
	float field_5a0;
	float field_5a4;
	float field_5a8;
	float field_5ac;
	float field_5b0;
	float field_5b4;
	float field_5b8;
	float field_5bc;
	float field_5c0;
	float field_5c4;
	float field_5c8;
	float field_5cc;
	float field_5d0;
	float field_5d4;
	float field_5d8;
	float field_5dc;
	float field_5e0;
	float field_5e4;
	float field_5e8;
	float field_5ec;
	float field_5f0;
	float field_5f4;
	float field_5f8;
	float field_5fc;
	float field_600;
	float field_604;
	float field_608;
	float field_60c;
	float field_610;
	float field_614;
	float field_618;
	float field_61c;
	float field_620;
	float field_624;
	float field_628;
	float field_62c;
	float field_630;
	float field_634;
	float field_638;
	float field_63c;
	float field_640;
	float field_644;
	float field_648;
	float field_64c;
	float field_650;
	float field_654;
	float field_658;
	float field_65c;
	float field_660;
	float field_664;
	float field_668;
	float field_66c;
	float field_670;
	float field_674;
	float field_678;
	float field_67c;
	float field_680;
	float field_684;
	float field_688;
	float field_68c;
	float field_690;
	float field_694;
	float field_698;
	float field_69c;
	float field_6a0;
	float field_6a4;
	float field_6a8;
	float field_6ac;
	float field_6b0;
	float field_6b4;
	float field_6b8;
	float field_6bc;
	float field_6c0;
	float field_6c4;
	float field_6c8;
	float field_6cc;
	float field_6d0;
	float field_6d4;
	float field_6d8;
	float field_6dc;
	float field_6e0;
	float field_6e4;
	float field_6e8;
	float field_6ec;
	float field_6f0;
	float field_6f4;
	float field_6f8;
	float field_6fc;
	float field_700;
	float field_704;
	float field_708;
	float field_70c;
	float field_710;
	float field_714;
	float field_718;
	float field_71c;
	float field_720;
	float field_724;
	float field_728;
	float field_72c;
	float field_730;
	float field_734;
	float field_738;
	float field_73c;
	float field_740;
	float field_744;
	float field_748;
	float field_74c;
	float field_750;
	float field_754;
	float field_758;
	float field_75c;
	float field_760;
	float field_764;
	float field_768;
	float field_76c;
	float field_770;
	float field_774;
	float field_778;
	float field_77c;
	float field_780;
	float field_784;
	float field_788;
	float field_78c;
	float field_790;
	float field_794;
	float field_798;
	float field_79c;
	float field_7a0;
	float field_7a4;
	float field_7a8;
	float field_7ac;
	float field_7b0;
	float field_7b4;
	float field_7b8;
	float field_7bc;
	float field_7c0;
	float field_7c4;
	float field_7c8;
	float field_7cc;
	float field_7d0;
	float field_7d4;
	float field_7d8;
	float field_7dc;
	float field_7e0;
	float field_7e4;
	float field_7e8;
	float field_7ec;
	float field_7f0;
	float field_7f4;
	float field_7f8;
	float field_7fc;
	float field_800;
	float field_804;
	float field_808;
	float field_80c;
	float field_810;
	float field_814;
	float field_818;
	float field_81c;
	float field_820;
	float field_824;
	float field_828;
	float field_82c;
	float field_830;
	float field_834;
	float field_838;
	float field_83c;
	float field_840;
	float field_844;
	float field_848;
	float field_84c;
	float field_850;
	float field_854;
	float field_858;
	float field_85c;
	float field_860;
	float field_864;
	float field_868;
	float field_86c;
	float field_870;
	float field_874;
	float field_878;
	float field_87c;
	float field_880;
	float field_884;
	float field_888;
	float field_88c;
	float field_890;
	float field_894;
	float field_898;
	float field_89c;
	float field_8a0;
	float field_8a4;


	float field_8a8;
	float field_8ac;
	float field_8b0;
	float field_8b4;
	float field_8b8;
	float field_8bc;
	float field_8c0;
	float field_8c4;
	float field_8c8;
	float field_8cc;
	float field_8d0;
	float field_8d4;
	float field_8d8;
	float field_8dc;
	float field_8e0;
	float field_8e4;
	float field_8e8;
	float field_8ec;
	float field_8f0;
	float field_8f4;
	float field_8f8;
	float field_8fc;
	float field_900;
	float field_904;
	float field_908;
	float field_90c;
	float field_910;
	float field_914;
	float field_918;
	float field_91c;
	float field_920;
	float field_924;
	float field_928;
	float field_92c;
	float field_930;
	float field_934;
	float field_938;
	float field_93c;
	float field_940;
	float field_944;
	float field_948;
	float field_94c;
	float field_950;
	float field_954;
	float field_958;
	float field_95c;
	float field_960;
	float field_964;
	float field_968;
	float field_96c;
	float field_970;
	float field_974;
	float field_978;
	float field_97c;
	float field_980;
	float field_984;
	float field_988;
	float field_98c;
	float field_990;
	float field_994;
	float field_998;
	float field_99c;
	float field_9a0;
	float field_9a4;
	float field_9a8;
	float field_9ac;
	float field_9b0;
	float field_9b4;
	float field_9b8;
	float field_9bc;
	float field_9c0;
	float field_9c4;
	float field_9c8;
	float field_9cc;
	float field_9d0;
	float field_9d4;
	float field_9d8;
	float field_9dc;
	float field_9e0;
	float field_9e4;
	float field_9e8;
	float field_9ec;
	float field_9f0;
	float field_9f4;
	float field_9f8;
	float field_9fc;
	float field_a00;
	float field_a04;
	float field_a08;
	float field_a0c;
	float field_a10;
	float field_a14;
	float field_a18;
	float field_a1c;
	float field_a20;
	float field_a24;


};

member_detour(TargetMinusOne__detour, cEmpireGrowthSim, void(uint32_t a, uint32_t b)) {
	void detoured(int a, int b) {
		//uint32_t noun = this->GetNounID();
		//uint32_t type = this->GetType();
		char16_t* name = this->GetName(99);
		original_function(this, a, b);
	}
};

member_detour(Creator__detour, cEmpireGrowthSim, cEmpireGrowthSim* ()) {
	cEmpireGrowthSim* detoured() {
		cEmpireGrowthSim* pointer = original_function(this);
		return pointer;
	}
};

member_detour(AIWar__detour, cEmpireGrowthSim, void(int empireID)) {
	void detoured(int empireID) {
		original_function(this, empireID);
		int a = 5;
	}
};

void AttachDetours()
{
	//DeclareAlliance__detour::attach(GetAddress(cRelationshipManager, DeclareAlliance));
	//BreakAlliance__detour::attach(GetAddress(cRelationshipManager, BreakAlliance));
	DeclareWar__detour::attach(GetAddress(cRelationshipManager, DeclareWar));
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
	TargetMinusOne__detour::attach(Address(0x00fea3a0));
	Creator__detour::attach(Address(0x00feab60));
	AIWar__detour::attach(Address(0x00fe9410));
}

// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

