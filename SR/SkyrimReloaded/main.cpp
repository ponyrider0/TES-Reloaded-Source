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

	float GetCustomEffectValue(StaticFunctionTag* FunctionTag, BSFixedString Name, BSFixedString Const)
	{
		double result;

		TheCommandManager->Commands.GetCustomEffectValue(&result, Name.data, Const.data);
		return result;
	}
	
	bool SetCustomEffectValue(StaticFunctionTag* FunctionTag, BSFixedString Name, BSFixedString Const, float Value)
	{
		double result;

		TheCommandManager->Commands.SetCustomEffectValue(&result, Name.data, Const.data, Value);
		return result;
	}

	bool RegisterCommands(VMClassRegistry* registry)
	{
		registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, float, BSFixedString, BSFixedString>("SRGetCustomEffectValue", SRClass, GetCustomEffectValue, registry));
		registry->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, float>("SRSetCustomEffectValue", SRClass, SetCustomEffectValue, registry));
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

	if (!skse->isEditor) {
		TheCommandManager = new CommandManager();
		TheSettingManager = new SettingManager();
		TheUtilityManager = new UtilityManager();	
		
		TheCommandManager->PapyrusInterface = (SKSEPapyrusInterface*)skse->QueryInterface(kInterface_Papyrus);
		TheCommandManager->PapyrusInterface->Register(SRPapyrus::RegisterCommands);

		TheSettingManager->LoadSettings();
		CreateConsoleCommandHook();
		CreateShaderIOHook();
		CreateRenderHook();
		CreateFormLoadHook();
		CreateSettingsHook();
		//if (TheSettingManager->SettingsMain.EnableBloodLens) CreateEventHook();
		if (TheSettingManager->SettingsMain.GrassMode) CreateGrassHook();
		if (TheSettingManager->SettingsMain.WindowedMode) CreateWindowedModeHook();
		if (TheSettingManager->SettingsMain.CameraMode) CreateCameraModeHook();
		if (TheSettingManager->SettingsMain.EquipmentMode) CreateEquipmentHook();
		if (TheSettingManager->SettingsMain.SleepingMode) CreateSleepingModeHook();
	}
	return true;
}

};