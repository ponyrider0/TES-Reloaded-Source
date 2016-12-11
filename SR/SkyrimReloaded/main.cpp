#define HookD3DDevice 0
#define WaitForDebugger 0

#include "skse\PapyrusVM.h"
#include "skse\PapyrusNativeFunctions.h"
#include "RenderHook.h"
#include "ShaderIOHook.h"
#include "FormHook.h"
#include "Grass.h"
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

	bool SetShaderValue(StaticFunctionTag* FunctionTag, BSFixedString Name, BSFixedString Const, float Value1, float Value2, float Value3, float Value4)
	{
		double result;
		float Value[4] = { Value1, Value2, Value3, Value4 };

		TheCommandManager->Commands.SetShaderValue(&result, Name.data, Const.data, Value);
		return result;
	}
	
	bool SetCustomShaderValue(StaticFunctionTag* FunctionTag, BSFixedString Name, BSFixedString Const, float Value1, float Value2, float Value3, float Value4)
	{
		double result;
		float Value[4] = { Value1, Value2, Value3, Value4 };

		TheCommandManager->Commands.SetCustomShaderValue(&result, Name.data, Const.data, Value);
		return result;
	}

	bool RegisterCommands(VMClassRegistry* registry)
	{
		registry->RegisterFunction(new NativeFunction6<StaticFunctionTag, bool, BSFixedString, BSFixedString, float, float, float, float>("SetShaderValue", SRClass, SetShaderValue, registry));
		registry->RegisterFunction(new NativeFunction6<StaticFunctionTag, bool, BSFixedString, BSFixedString, float, float, float, float>("SetCustomShaderValue", SRClass, SetCustomShaderValue, registry));
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
		CreateGameEventHook();

		if (TheSettingManager->SettingsMain.CameraMode) CreateCameraModeHook();
		if (TheSettingManager->SettingsMain.SleepingMode) CreateSleepingModeHook();

#ifdef _DEBUG
	#if WaitForDebugger
			while (!IsDebuggerPresent()) Sleep(10);
	#endif
#endif
	}
	return true;
}

};