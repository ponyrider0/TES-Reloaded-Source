#include "ScriptManager.h"

#if defined(OBLIVION)
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

LowFSoundScript::LowFSoundScript()
{
	BreathingF = NULL;
	BreathingM = NULL;
	ElapsedTime = -1.0f;
}

void LowFSoundScript::Run()
{
	if (TheShaderManager->ShaderConst.LowHF_Data.x || ElapsedTime != -1.0f) {
		float HealthCoeff = TheShaderManager->ShaderConst.LowHF_HealthCoeff;
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

PurgerScript::PurgerScript()
{

}

void PurgerScript::Run()
{
	int PurgerTime = TheSettingManager->SettingsMain.PurgerTime;
	bool PurgerKeyPressed = TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.PurgerKey);

	if (PurgerTime || PurgerKeyPressed) {
		ElapsedTime += TheFrameRateManager->ElapsedTime;
		if (ElapsedTime >= PurgerTime || PurgerKeyPressed) {
			if (TheSettingManager->SettingsMain.PurgerPurgeCells) TheUtilityManager->PurgeCellBuffers();
			if (TheSettingManager->SettingsMain.PurgerPurgeTexture) TheRenderManager->device->EvictManagedResources();
			ElapsedTime = 0.0f;
		}
	}
}

ScriptManager::ScriptManager()
{
	_MESSAGE("Starting the script manager...");
	TheScriptManager = this;

	LowHSound = new LowHSoundScript();
	LowFSound = new LowFSoundScript();
	Purger = new PurgerScript();
}

void ScriptManager::Run()
{
	if (TheSettingManager->SettingsMain.PurgerEnabled) Purger->Run();
	if (TheSettingManager->SettingsMain.LowHFSoundEnableHealth) LowHSound->Run();
	if (TheSettingManager->SettingsMain.LowHFSoundEnableFatigue) LowFSound->Run();
}
#elif defined(SKYRIM)
ScriptManager::ScriptManager()
{
	TheScriptManager = this;
}
#endif
