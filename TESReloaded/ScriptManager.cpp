#if defined(OBLIVION)
#include "Hooking\detours\detours.h"

BaseScript::BaseScript()
{
	ElapsedTime = 0.0f;
}

LowHSoundScript::LowHSoundScript()
{
	HeartSlow = NULL;
	ElapsedTime = -1.0f;
}

void LowHSoundScript::Run()
{
	if (!TheUtilityManager->IsMenu()) {
		if (TheShaderManager->ShaderConst.LowHF_Data.x || ElapsedTime != -1.0f) {
			float HealthCoeff = TheShaderManager->ShaderConst.LowHF_HealthCoeff;

			if ((*g_thePlayer)->DeadState == 0) {
				if (HealthCoeff) {
					if (ElapsedTime == -1.0f && HealthCoeff >= 0.5) {
						TheUtilityManager->Play(HeartSlow);
						ElapsedTime = 0.0f;
					}
					else if (ElapsedTime >= 0.0f) {
						ElapsedTime += TheFrameRateManager->ElapsedTime;
						if (ElapsedTime >= 1.5f) ElapsedTime = -1.0f;
					}
				}
			}
		}
	}
}

LowFSoundScript::LowFSoundScript()
{
	BreathingF = NULL;
	BreathingM = NULL;
	ElapsedTime = -1.0f;
}

void LowFSoundScript::Run()
{
	if (!TheUtilityManager->IsMenu()) {
		if (TheShaderManager->ShaderConst.LowHF_Data.x || ElapsedTime != -1.0f) {
			float FatigueCoeff = TheShaderManager->ShaderConst.LowHF_FatigueCoeff;

			if ((*g_thePlayer)->DeadState == 0) {
				if (FatigueCoeff) {
					if (ElapsedTime == -1.0f && FatigueCoeff >= 0.5) {
						if (TheUtilityManager->IsFemale((*g_thePlayer))) {
							TheUtilityManager->Play(BreathingF);
							BreathingTime = 8.0f;
						}
						else {
							TheUtilityManager->Play(BreathingM);
							BreathingTime = 1.0f;
						}
						ElapsedTime = 0.0f;
					}
					else if (ElapsedTime >= 0.0f) {
						ElapsedTime += TheFrameRateManager->ElapsedTime;
						if (ElapsedTime >= BreathingTime) ElapsedTime = -1.0f;
					}
				}
			}
		}
	}
}

PurgerScript::PurgerScript() { }

void PurgerScript::Run()
{
	if (!TheUtilityManager->IsMenu()) {
		int PurgerTime = TheSettingManager->SettingsMain.PurgerTime;
		bool PurgerKeyPressed = TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.PurgerKey);

		if (PurgerTime || PurgerKeyPressed) {
			ElapsedTime += TheFrameRateManager->ElapsedTime;
			if (ElapsedTime >= PurgerTime || PurgerKeyPressed) {
				if (TheSettingManager->SettingsMain.PurgerPurgeTexture) TheRenderManager->device->EvictManagedResources();
				if (TheSettingManager->SettingsMain.PurgerPurgeCells) TheUtilityManager->PurgeCellBuffers();
				ElapsedTime = 0.0f;
			}
		}
	}
}

void EquipmentSetupChoice()
{
	UInt8(*GetButtonPressed)() = (UInt8(*)())0x00578D70;

	TheScriptManager->EquipmentSetup->ConfigStep = (EquipmentSetupScript::StepType)(GetButtonPressed() + EquipmentSetupScript::StepType::Normal);
	if (TheScriptManager->EquipmentSetup->ConfigStep <= EquipmentSetupScript::StepType::Swimming)
		TheScriptManager->EquipmentSetup->EquipItems(EquipmentSetupScript::StepType::Normal, TheScriptManager->EquipmentSetup->ConfigStep);
	else
		TheScriptManager->EquipmentSetup->ConfigStep = EquipmentSetupScript::StepType::None;
}

EquipmentSetupScript::EquipmentSetupScript() {

	ConfigStep = None;
	GameStep = Normal;
	CombatState = 0;
	for (int i = 0; i < 5; i++) LeftWeapon[i] = NULL;

}

void EquipmentSetupScript::Run()
{
	if (ConfigStep == None && !TheUtilityManager->IsMenu()) {
		StepType CurrentStep = GetCurrentEquipmentType();
		if (CurrentStep != GameStep) {
			if (CurrentStep == Normal || CurrentStep == Combat || (TheSettingManager->SettingsMain.EquipmentModeSleepingEquipment && CurrentStep == Sleeping) || (TheSettingManager->SettingsMain.EquipmentModeSwimmingEquipment && CurrentStep == Swimming)) {
				EquipItems(GameStep, CurrentStep);
				GameStep = CurrentStep;
			}
		}
	}
	else if (ConfigStep == Request) {
		if (GameStep == EquipmentSetupScript::StepType::Normal && !CombatState) {
			ConfigStep = Choice;
			ShowMessageBox("Equipment setup", EquipmentSetupChoice, 1, "Combat", "Sleep", "Swim", "Cancel", NULL);
		}
		else {
			ConfigStep = EquipmentSetupScript::StepType::None;
			TheUtilityManager->ShowMessage("You cannot use the equipment menu now.");
		}
	}
	else if (ConfigStep >= Combat && !TheUtilityManager->IsMenu()) {
		EquipItems(ConfigStep, Normal);
		ConfigStep = None;
	}
}

void EquipmentSetupScript::EquipItems(StepType From, StepType To)
{
	PlayerCharacter* Act = *g_thePlayer;
	HighProcessEx* Process = (HighProcessEx*)Act->process;
	ExtraContainerChanges::Data* ExtraContainerChangesData = TheUtilityManager->GetExtraContainerChangesData((ExtraDataList*)&Act->baseExtraList);

	Items[From - Normal].clear();
	if (ExtraContainerChangesData) {
		for (ExtraContainerChanges::Entry* Entry = ExtraContainerChangesData->objList; Entry; Entry = Entry->next) {
			if (Entry->data && Entry->data->extendData) {
				for (ExtraContainerChanges::EntryExtendData* ExtendData = Entry->data->extendData; ExtendData; ExtendData = ExtendData->next) {
					if (ExtendData->data && ExtendData->data->IsWorn()) {
						TESForm* Form = Entry->data->type;
						Items[From - Normal].push_back(Form);
						if (Form->typeID == FormType::kFormType_Weapon && ExtendData->data->HasType(kExtraData_WornLeft)) LeftWeapon[From - Normal] = (TESObjectWEAP*)Form;
						Act->UnequipItem(Form, 1, ExtendData->data, 0, false, 0);
					}
				}
			}
		}
	}
	for each (TESForm* Form in Items[To - Normal]) {
		if (LeftWeapon[To - Normal] && Form == LeftWeapon[To - Normal]) {
			LeftWeapon[To - Normal] = NULL;
			Process->LeftEquippingState = HighProcessEx::State::In;
		}
		Act->EquipItem(Form, 1, NULL, 0, 0);
		Process->LeftEquippingState = HighProcessEx::State::None;
	}
	TheUtilityManager->UpdateInventory();
}

EquipmentSetupScript::StepType EquipmentSetupScript::GetCurrentEquipmentType()
{
	HighProcess* Process = (HighProcess*)(*g_thePlayer)->process;
	StepType CurrentStep = Normal;
	
	if (CombatState) CurrentStep = Combat;
	if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.EquipmentModeCombatEquipmentKey)) {
		if (CombatState) CurrentStep = Normal; else CurrentStep = Combat;
		CombatState = !CombatState;
	}
	else if (Process->SitSleepState >= 8 && Process->SitSleepState <= 10) {
		if (!CombatState) CurrentStep = Sleeping;
	}
	else if (Process->GetMovementFlags() & 0x800) {
		if (!CombatState) {
			ElapsedTime = 0.0f;
			CurrentStep = Swimming;
		}
	}
	if (CurrentStep == Normal && GameStep == Swimming && ElapsedTime < 2.0f) {
		ElapsedTime += TheFrameRateManager->ElapsedTime;
		CurrentStep = Swimming;
	}
	return CurrentStep;
}

class ScriptHook {

public:
	void TrackRunScripts();

};

void (__thiscall ScriptHook::* RunScripts)();
void (__thiscall ScriptHook::* TrackRunScripts)();
void ScriptHook::TrackRunScripts() {
	
	(this->*RunScripts)();
	TheScriptManager->Run();
}

ScriptManager::ScriptManager()
{
	_MESSAGE("Starting the script manager...");
	TheScriptManager = this;
	
	LowHSound = new LowHSoundScript();
	LowFSound = new LowFSoundScript();
	Purger = new PurgerScript();
	EquipmentSetup = new EquipmentSetupScript();
}

void ScriptManager::Run()
{
	if (TheSettingManager->SettingsMain.PurgerEnabled) Purger->Run();
	if (TheSettingManager->SettingsMain.EnableLowHF) {
		if (TheSettingManager->SettingsMain.LowHFSoundEnableHealth) LowHSound->Run();
		if (TheSettingManager->SettingsMain.LowHFSoundEnableFatigue) LowFSound->Run();
	}
	if (TheSettingManager->SettingsMain.EquipmentMode) EquipmentSetup->Run();
}

void CreateScriptHook()
{
	*((int *)&RunScripts)	= 0x004402F0;
	TrackRunScripts			= &ScriptHook::TrackRunScripts;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)RunScripts,	*((PVOID *)&TrackRunScripts));
	DetourTransactionCommit();
}
#elif defined(SKYRIM)
ScriptManager::ScriptManager()
{
	TheScriptManager = this;
}
#endif