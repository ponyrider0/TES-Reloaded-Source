#define HookD3DDevice 0  // This hooks the D3D Device. Only for developing/debugging when needed.

#include "skse\PapyrusVM.h"
#include "skse\PapyrusNativeFunctions.h"
#include "RenderHook.h"
#include "ShaderIOHook.h"
#include "FormHook.h"
#include "Grass.h"
#include "WindowedMode.h"
#include "EventInterface.h"
#include "CameraMode.h"
#include "SleepingMode.h"
#if HookD3DDevice
#include "Hooking\apihijack.h"
#include "Hooking\apihijack.cpp"
#include "Hooking\D3D9.cpp"
#include "Hooking\K32.cpp"
#include "Hooking\U32.cpp"
#endif

IDebugLog			debugLog("SkyrimReloaded.log");
PluginHandle		pluginHandle = kPluginHandle_Invalid;

namespace SRPapyrus
{
	const char* SRClass = "SRCommands";

	bool Screenshot(StaticFunctionTag* FunctionTag)
	{
		double result;

		TheCommandManager->Commands.Screenshot(&result);
		return result;
	}

	bool RegisterCommands(VMClassRegistry* registry)
	{
		registry->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("SRScreenshot", SRClass, Screenshot, registry));
		registry->SetFunctionFlags(SRClass, "SRScreenshot", VMClassRegistry::kFunctionFlag_NoWait);
		return true;
	}

}

extern "C"
{

bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "SkyrimReloaded";
	info->version = 1;
#if HookD3DDevice
	if(!skse->isEditor) {
		HookAPICalls(&K32Hook);
		HookAPICalls(&U32Hook);
	}
#endif
	return true;
}

bool SKSEPlugin_Load(const SKSEInterface* skse)
{
	pluginHandle = skse->GetPluginHandle();
	
	TheCommandManager = new CommandManager();
	TheCommandManager->PapyrusInterface = (SKSEPapyrusInterface*)skse->QueryInterface(kInterface_Papyrus);
	TheCommandManager->PapyrusInterface->Register(SRPapyrus::RegisterCommands);

	if (!skse->isEditor) {
		TheSettingManager = new SettingManager();
		TheUtilityManager = new UtilityManager();	
		
		TheSettingManager->LoadSettings();
		CreateConsoleCommandHook();
		CreateShaderIOHook();
		CreateRenderHook();
		CreateFormLoadHook();
		//CreateSettingsHook();
		//if (TheSettingManager->SettingsMain.EnableBloodLens) CreateEventHook();
		if (TheSettingManager->SettingsMain.GrassMode) CreateGrassHook();
		if (TheSettingManager->SettingsMain.WindowedMode) CreateWindowedModeHook();
		if (TheSettingManager->SettingsMain.CameraMode) CreateCameraModeHook();
		if (TheSettingManager->SettingsMain.EquipmentMode) CreateEquipmentHook();
		if (TheSettingManager->SettingsMain.SleepingMode) CreateSleepingModeHook();
		//Sleep(3000);
	}
	return true;
}

};