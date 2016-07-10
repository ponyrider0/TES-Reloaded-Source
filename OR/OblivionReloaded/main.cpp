#define HookD3DDevice 0 // This hooks the D3D Device. Only for developing/debugging when needed.

#include "obse_common\SafeWrite.h"
#include "RenderHook.h"
#include "ShaderIOHook.h"
#include "FormHook.h"
#include "Grass.h"
#include "WindowedMode.h"
#include "EventInterface.h"
#include "CombatMode.h"
#include "CameraMode.h"
#include "SleepingMode.h"
#include "Animation.h"
#if HookD3DDevice
#include "Hooking\apihijack.h"
#include "Hooking\apihijack.cpp"
#include "Hooking\D3D9.cpp"
#include "Hooking\K32.cpp"
#include "Hooking\U32.cpp"
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

bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{

	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "OblivionReloaded";
	info->version = 4;
#if HookD3DDevice
	if(!obse->isEditor)
	{
		HookAPICalls(&K32Hook);
		HookAPICalls(&U32Hook);
	}
#endif
	return true;
}

bool OBSEPlugin_Load(const OBSEInterface* obse)
{

	pluginHandle = obse->GetPluginHandle();

	TheCommandManager = new CommandManager();
	TheCommandManager->RegisterCommands(obse);

	if (!obse->isEditor)
	{
		TheSettingManager = new SettingManager();
		TheUtilityManager = new UtilityManager();	

		TheCommandManager->ConsoleInterface = (OBSEConsoleInterface*)obse->QueryInterface(kInterface_Console);
		TheCommandManager->ArrayInterface = (OBSEArrayVarInterface*)obse->QueryInterface(kInterface_ArrayVar);

		TheSettingManager->LoadSettings();

		CreateShaderIOHook();
		CreateRenderHook();
		CreateFormLoadHook();
		CreateSettingsHook();
		CreateAnimationHook();

		if (TheSettingManager->SettingsMain.EnableBloodLens) CreateEventHook();
		if (TheSettingManager->SettingsMain.GrassMode) CreateGrassHook();
		if (TheSettingManager->SettingsMain.WindowedMode) CreateWindowedModeHook();
		if (TheSettingManager->SettingsMain.CombatMode) CreateCombatModeHook();
		if (TheSettingManager->SettingsMain.CameraMode) CreateCameraModeHook();
		if (TheSettingManager->SettingsMain.EquipmentMode) CreateEquipmentHook();
		if (TheSettingManager->SettingsMain.SleepingMode) CreateSleepingModeHook();

		WriteRelJump(0x00498494, 0x004984A0); //Skips antialiasing deactivation if HDR is enabled on the D3DDevice
		WriteRelJump(0x005DEE60, 0x005DEE68); //Skips antialiasing deactivation if HDR is enabled on loading the video menu
		WriteRelJump(0x005DF69E, 0x005DF755); //Skips HDR deactivation changing antialising (video menu)
		WriteRelJump(0x00497D5A, 0x00497D63); //Unlocks antialising bar if HDR is enabled (video menu)
		WriteRelJump(0x005DF8E9, 0x005DF983); //Skips antialising deactivation changing HDR (video menu)
		WriteRelJump(kDecalLifetimeHook, (UInt32)DecalLifetimeHook); //Manages fDecalLifetime (video menu)
	}
	return true;

}

};