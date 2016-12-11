#define HookD3DDevice 0
#define WaitForDebugger 0

#include "obse_common\SafeWrite.h"
#include "RenderHook.h"
#include "ShaderIOHook.h"
#include "FormHook.h"
#include "Grass.h"
#include "CombatMode.h"
#include "CameraMode.h"
#include "SleepingMode.h"
#include "Animation.h"
#ifdef _DEBUG
#if HookD3DDevice
#include "Hooking\apihijack.h"
#include "Hooking\apihijack.cpp"
#include "Hooking\D3D9.cpp"
#include "Hooking\K32.cpp"
#include "Hooking\U32.cpp"
#endif
#endif

static const UInt32 kDecalLifetimeHook = 0x005DF53C;
static const UInt32 kDecalLifetimeReturn = 0x005DF55D;

IDebugLog			debugLog("OblivionReloaded.log");
PluginHandle		pluginHandle = kPluginHandle_Invalid;

void DecalLifetime(int DecalType)
{
	if (DecalType == 0)
		*SettingDecalLifetime = 0;
	else if (DecalType == 1)
		*SettingDecalLifetime = 10;
	else if (DecalType == 2)
		if (*SettingDecalLifetime == 0) *SettingDecalLifetime = 15;
};

static __declspec(naked) void DecalLifetimeHook()
{
	__asm
	{
		mov		edi, dword ptr [esi+0x0F0]
		push	edi
		call	DecalLifetime
		pop		edi
		mov		edi, 2
		jmp		kDecalLifetimeReturn
	}
}

extern "C" {

bool OBSEPlugin_Query(const OBSEInterface* obse, PluginInfo* info)
{

	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "OblivionReloaded";
	info->version = 5;
#ifdef _DEBUG
	#if HookD3DDevice
	if(!obse->isEditor)
	{
		HookAPICalls(&K32Hook);
		HookAPICalls(&U32Hook);
	}
	#endif
#endif
	return true;
}

bool OBSEPlugin_Load(const OBSEInterface* obse)
{

	pluginHandle = obse->GetPluginHandle();

	new CommandManager();
	TheCommandManager->RegisterCommands(obse);

	if (!obse->isEditor)
	{
		new UtilityManager();
		new SettingManager();

		TheSettingManager->LoadSettings();

		CreateShaderIOHook();
		CreateRenderHook();
		CreateFormLoadHook();
		CreateSettingsHook();
		CreateAnimationHook();
		CreateGameEventHook();

		if (TheSettingManager->SettingsMain.GrassMode) CreateGrassHook();
		if (TheSettingManager->SettingsMain.CombatMode) CreateCombatModeHook();
		if (TheSettingManager->SettingsMain.CameraMode) CreateCameraModeHook();
		//if (TheSettingManager->SettingsMain.EquipmentMode) CreateEquipmentHook();
		if (TheSettingManager->SettingsMain.SleepingMode) CreateSleepingModeHook();

		WriteRelJump(0x0049849A, 0x004984A0); //Skips antialiasing deactivation if HDR is enabled on the D3DDevice
		WriteRelJump(0x004984BD, 0x004984CD); //Skips antialiasing deactivation if AllowScreenshot is enabled
		WriteRelJump(0x005DEE60, 0x005DEE68); //Skips antialiasing deactivation if HDR is enabled on loading the video menu
		WriteRelJump(0x005DF69E, 0x005DF755); //Skips HDR deactivation changing antialising (video menu)
		WriteRelJump(0x00497D5A, 0x00497D63); //Unlocks antialising bar if HDR is enabled (video menu)
		WriteRelJump(0x005DF8E9, 0x005DF983); //Skips antialising deactivation changing HDR (video menu)
		WriteRelJump(kDecalLifetimeHook, (UInt32)DecalLifetimeHook); //Manages fDecalLifetime (video menu)

#ifdef _DEBUG
	#if WaitForDebugger
		while (!IsDebuggerPresent()) Sleep(10);
	#endif
#endif
	}
	return true;

}

};