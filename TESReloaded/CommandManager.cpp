#include "CommandInfo.h"
#if defined(OBLIVION)
#include "Animation.h"
#define CommandPrefix "OR"
#define PluginVersion "Oblivion Reloaded v4.2.3"
#elif defined(SKYRIM)
#include "skse\skse_version.h"
#include "skse\PapyrusVM.h"
#include "Hooking\detours\detours.h"
#include "CommandManager.h"
#define CommandPrefix "SR"
#define PluginVersion "Skyrim Reloaded v1.3.3"
#endif

#define DEFINE_COMMAND_PLUGIN_R(name, altName, description, refRequired, numParams, paramInfo, parser) \
	extern bool Cmd_ ## name ## _Execute(COMMAND_ARGS); \
	static CommandInfo (kCommandInfo_ ## name) = { \
	CommandPrefix#name, \
	#altName, \
	0, \
	#description, \
	refRequired, \
	numParams, \
	paramInfo, \
	Cmd_ ## name ## _Execute, \
	parser, \
	NULL, \
	0 \
}

#if defined(SKYRIM)
bool Cmd_GetSKSEVersion_Execute(COMMAND_ARGS)
{
	Console_Print("SKSE v%d.%d.%d, Release %d", SKSE_VERSION_INTEGER, SKSE_VERSION_INTEGER_MINOR, SKSE_VERSION_INTEGER_BETA, SKSE_VERSION_RELEASEIDX);
	return true;
}

bool Cmd_ClearInvalidRegistrations_Execute(COMMAND_ARGS)
{
	UInt32 count = (*g_skyrimVM)->ClearInvalidRegistrations();
	Console_Print("Removed %d invalid OnUpdate registration(s)", count);
	return true;
}
#endif

bool Cmd_PurgeResources_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.PurgeResources(result);
	return true;
}

bool Cmd_GetLocationName_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.GetLocationName(result);
	return true;
}

bool Cmd_Screenshot_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.Screenshot(result);
	return true;
}

bool Cmd_LoadSettings_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.LoadSettings(result);
	return true;
}

bool Cmd_SaveSettings_Execute(COMMAND_ARGS)
{
	char Name[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name)) TheCommandManager->Commands.SaveSettings(result, Name);
	return true;
}

bool Cmd_GetSetting_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Section[80];
	char Setting[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Section, &Setting)) TheCommandManager->Commands.GetSetting(result, Name, Section, Setting);
	return true;
}

bool Cmd_SetSetting_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Section[80];
	char Setting[80];
	float Value;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Section, &Setting, &Value)) TheCommandManager->Commands.SetSetting(result, Name, Section, Setting, Value);
	return true;
}

bool Cmd_GetValue_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Const[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Const)) TheCommandManager->Commands.GetValue(result, Name, Const);
	return true;
}

bool Cmd_GetCustomValue_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Const[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Const)) TheCommandManager->Commands.GetCustomValue(result, Name, Const);
	return true;
}

bool Cmd_SetCustomValue_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Const[80];
	float Value;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Const, &Value)) TheCommandManager->Commands.SetCustomValue(result, Name, Const, Value);
	return true;
}

bool Cmd_GetEnabled_Execute(COMMAND_ARGS)
{
	char Name[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name)) TheCommandManager->Commands.GetEnabled(result, Name);
	return true;
}

bool Cmd_SetEnabled_Execute(COMMAND_ARGS)
{
	char Name[80];
	int Value;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Value)) TheCommandManager->Commands.SetEnabled(result, Name, Value);
	return true;
}

bool Cmd_GetShaders_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.GetShaders(result, scriptObj);
	return true;
}

bool Cmd_GetSections_Execute(COMMAND_ARGS)
{
	char Name[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name)) TheCommandManager->Commands.GetSections(result, Name, scriptObj);
	return true;
}

bool Cmd_GetSettings_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Section[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Section)) TheCommandManager->Commands.GetSettings(result, Name, Section, scriptObj);
	return true;
}

bool Cmd_EquipLeftWeapon_Execute(COMMAND_ARGS)
{
	TESObjectWEAP* Weapon = NULL;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Weapon)) TheCommandManager->Commands.EquipLeftWeapon(result, (Actor*)thisObj, Weapon);
	return true;
}

bool Cmd_GetLeftWeapon_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.GetLeftWeapon(result, (Actor*)thisObj);
	return true;
}

bool Cmd_CameraMove_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.Ref, &TheSettingManager->SettingsCamera.Pos.x, &TheSettingManager->SettingsCamera.Pos.y, &TheSettingManager->SettingsCamera.Pos.z)) TheCommandManager->Commands.CameraMove(result);
	return true;
}

bool Cmd_CameraRotate_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.Ref, &TheSettingManager->SettingsCamera.AngX, &TheSettingManager->SettingsCamera.AngY, &TheSettingManager->SettingsCamera.AngZ)) TheCommandManager->Commands.CameraRotate(result);
	return true;
}

bool Cmd_CameraLook_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.Ref, &TheSettingManager->SettingsCamera.LookPos.x, &TheSettingManager->SettingsCamera.LookPos.y, &TheSettingManager->SettingsCamera.LookPos.z)) TheCommandManager->Commands.CameraLook(result);
	return true;
}

bool Cmd_CameraMoveTo_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.Pos.x, &TheSettingManager->SettingsCamera.Pos.y, &TheSettingManager->SettingsCamera.Pos.z)) TheCommandManager->Commands.CameraMoveTo(result);
	return true;
}

bool Cmd_CameraRotateTo_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.AngX, &TheSettingManager->SettingsCamera.AngY, &TheSettingManager->SettingsCamera.AngZ)) TheCommandManager->Commands.CameraRotateTo(result);
	return true;
}

bool Cmd_CameraLookTo_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.LookPos.x, &TheSettingManager->SettingsCamera.LookPos.y, &TheSettingManager->SettingsCamera.LookPos.z)) TheCommandManager->Commands.CameraLookTo(result);
	return true;
}

bool Cmd_CameraReset_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.CameraReset(result);
	return true;
}

bool Cmd_CameraGetPosX_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.CameraGetPosX(result);
	return true;
}

bool Cmd_CameraGetPosY_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.CameraGetPosY(result);
	return true;
}

bool Cmd_CameraGetPosZ_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.CameraGetPosZ(result);
	return true;
}

bool Cmd_GetAnimGroup_Execute(COMMAND_ARGS)
{
	int AnimGroup = 0;
	int AnimType = 0;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &AnimGroup, &AnimType)) TheCommandManager->Commands.GetAnimGroup(result, (Actor*)thisObj, AnimGroup, AnimType);
	return true;
}

bool Cmd_SetAnimGroup_Execute(COMMAND_ARGS)
{
	int Permanent = 0;
	int PlayAnim = 0;
	int AnimGroup = 0;
	int AnimType = 0;
	char Filename[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Permanent, &PlayAnim, &AnimGroup, &AnimType, &Filename)) TheCommandManager->Commands.SetAnimGroup(result, (Actor*)thisObj, Permanent, PlayAnim, AnimGroup, AnimType, Filename);
	return true;
}

bool Cmd_ResetAnimGroup_Execute(COMMAND_ARGS)
{
	int AnimGroup = 0;
	int AnimType = 0;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &AnimGroup, &AnimType)) TheCommandManager->Commands.ResetAnimGroup(result, (Actor*)thisObj, AnimGroup, AnimType);
	return true;
}

bool Cmd_GetVersion_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.GetVersion(result);
	return true;
}

#if defined(SKYRIM)
#define CommandPrefix ""
DEFINE_COMMAND_PLUGIN_R(GetSKSEVersion, , "Returns the SKSE version", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(ClearInvalidRegistrations, , "Clears invalid event registrations", 0, 0, NULL, NULL);
#define CommandPrefix "SR"
#endif
DEFINE_COMMAND_PLUGIN_R(PurgeResources,	, "Purges resources from the video memory", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(GetLocationName,, "Gets the name of the current worldspace and current cell - this is a console command only", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(Screenshot,		, "Takes a screenshot without the HUD", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(LoadSettings,	, "Loads settings from configuration files", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(SaveSettings,	, "Saves settings into the shader configuration file", 0, 1, kParams_OneString, NULL);
DEFINE_COMMAND_PLUGIN_R(GetSetting,		, "Gets the current setting of a shader", 0, 3, kParams_ThreeString, NULL);
DEFINE_COMMAND_PLUGIN_R(SetSetting,		, "Sets the current setting of a shader", 0, 4, kParams_ThreeString_OneFloat, NULL);
DEFINE_COMMAND_PLUGIN_R(GetValue,		, "Gets the current value of a shader", 0, 2, kParams_TwoString, NULL);
DEFINE_COMMAND_PLUGIN_R(GetCustomValue, , "Gets the current value of a custom effect", 0, 2, kParams_TwoString, NULL);
DEFINE_COMMAND_PLUGIN_R(SetCustomValue, , "Sets the current value of a custom effect", 0, 3, kParams_TwoString_OneFloat, NULL);
DEFINE_COMMAND_PLUGIN_R(GetEnabled,		, "Gets the status of a shader", 0, 1, kParams_OneString, NULL);
DEFINE_COMMAND_PLUGIN_R(SetEnabled,		, "Enables/disables an effect", 0, 2, kParams_OneString_OneInt, NULL);
DEFINE_COMMAND_PLUGIN_R(GetShaders,		, "Gets the shaders list", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(GetSections,	, "Gets the sections list of a shader", 0, 1, kParams_OneString, NULL);
DEFINE_COMMAND_PLUGIN_R(GetSettings,	, "Gets settings of a specific section of a shader", 0, 2, kParams_TwoString, NULL);
DEFINE_COMMAND_PLUGIN_R(EquipLeftWeapon,, "Equips a weapon in the left hand", 1, 1, kParams_OneObject, NULL);
DEFINE_COMMAND_PLUGIN_R(GetLeftWeapon,	, "Gets the original weapon in the left hand", 1, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(CameraMove,		, "Moves the camera from the actor's head", 0, 4, kParams_OneActorThreeFloats, NULL);
DEFINE_COMMAND_PLUGIN_R(CameraRotate,	, "Rotates the camera in relation of the actor's head", 0, 4, kParams_OneActorThreeFloats, NULL);
DEFINE_COMMAND_PLUGIN_R(CameraLook,		, "Forces the camera to look at the actor", 0, 4, kParams_OneActorThreeFloats, NULL);
DEFINE_COMMAND_PLUGIN_R(CameraMoveTo,	, "Moves the camera in the world", 0, 3, kParams_ThreeFloats, NULL);
DEFINE_COMMAND_PLUGIN_R(CameraRotateTo, , "Rotates the camera in the world", 0, 3, kParams_ThreeFloats, NULL);
DEFINE_COMMAND_PLUGIN_R(CameraLookTo,	, "Forces the camera to look to a point in the world", 0, 3, kParams_ThreeFloats, NULL);
DEFINE_COMMAND_PLUGIN_R(CameraReset,	, "Restores the camera to the normal state", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(CameraGetPosX,	, "Gets the camera position for the x axis", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(CameraGetPosY,	, "Gets the camera position for the y axis", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(CameraGetPosZ,	, "Gets the camera position for the z axis", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(GetAnimGroup,	, "Gets if a specific animation for the specific group is set", 1, 2, kParams_TwoInts, NULL);
DEFINE_COMMAND_PLUGIN_R(SetAnimGroup,	, "Sets a specific animation for the specific group", 1, 5, kParams_FourIntsOneString, NULL);
DEFINE_COMMAND_PLUGIN_R(ResetAnimGroup, , "Reset a permanent specific animation for the specific group to default", 1, 2, kParams_TwoInts, NULL);
DEFINE_COMMAND_PLUGIN_R(GetVersion,		, "Returns the plugin version", 0, 0, NULL, NULL);

#if defined(OBLIVION)
CommandManager::CommandManager() {

	_MESSAGE("Starting the command manager...");
	ConsoleInterface = NULL;
	ArrayInterface = NULL;

}

void CommandManager::RegisterCommands(const OBSEInterface* obse)
{
	obse->SetOpcodeBase(0x2800);
	obse->RegisterCommand(&kCommandInfo_PurgeResources);
	obse->RegisterCommand(&kCommandInfo_GetLocationName);
	obse->RegisterCommand(&kCommandInfo_Screenshot);
	obse->RegisterCommand(&kCommandInfo_LoadSettings);
	obse->RegisterCommand(&kCommandInfo_SaveSettings);
	obse->RegisterCommand(&kCommandInfo_GetSetting);
	obse->RegisterCommand(&kCommandInfo_SetSetting);
	obse->RegisterCommand(&kCommandInfo_GetValue);
	obse->RegisterCommand(&kCommandInfo_GetCustomValue);
	obse->RegisterCommand(&kCommandInfo_SetCustomValue);
	obse->RegisterCommand(&kCommandInfo_GetEnabled);
	obse->RegisterCommand(&kCommandInfo_SetEnabled);
	obse->RegisterTypedCommand(&kCommandInfo_GetShaders, kRetnType_Array);
	obse->RegisterTypedCommand(&kCommandInfo_GetSections, kRetnType_Array);
	obse->RegisterTypedCommand(&kCommandInfo_GetSettings, kRetnType_Array);
	obse->RegisterCommand(&kCommandInfo_EquipLeftWeapon);
	obse->RegisterCommand(&kCommandInfo_GetLeftWeapon);
	obse->RegisterCommand(&kCommandInfo_CameraMove);
	obse->RegisterCommand(&kCommandInfo_CameraRotate);
	obse->RegisterCommand(&kCommandInfo_CameraLook);
	obse->RegisterCommand(&kCommandInfo_CameraMoveTo);
	obse->RegisterCommand(&kCommandInfo_CameraRotateTo);
	obse->RegisterCommand(&kCommandInfo_CameraLookTo);
	obse->RegisterCommand(&kCommandInfo_CameraReset);
	obse->RegisterCommand(&kCommandInfo_CameraGetPosX);
	obse->RegisterCommand(&kCommandInfo_CameraGetPosY);
	obse->RegisterCommand(&kCommandInfo_CameraGetPosZ);
	obse->RegisterCommand(&kCommandInfo_GetAnimGroup);
	obse->RegisterCommand(&kCommandInfo_SetAnimGroup);
	obse->RegisterCommand(&kCommandInfo_ResetAnimGroup);
	obse->RegisterCommand(&kCommandInfo_GetVersion);
}

OBSEArrayVarInterface::Array* CommandManager::GetOBSEArray(SettingsList data, Script* callingScript)
{
	OBSEArrayVarInterface::Array* arr = TheCommandManager->ArrayInterface->CreateStringMap(NULL, NULL, 0, callingScript);

	for (std::map<std::string, MultipleElement>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
		TheCommandManager->ArrayInterface->SetElement(arr, iter->first.c_str(), iter->second);
	return arr;
}

void CommandManager::EquipItem(Actor* Act, TESForm* Item) {

	char Command[80];
	
	if (Item) {
		sprintf(Command,"EquipItemNS %08X", Item->refID);
		ConsoleInterface->RunScriptLine2(Command, Act, true);
	}

}

void CommandManager::UnequipItem(Actor* Act, TESForm* Item) {

	char Command[80];

	if (Item) {
		sprintf(Command,"UnequipItemNS %08X", Item->refID);
		ConsoleInterface->RunScriptLine2(Command, Act, true);
	}

}

void CommandManager::AddItem(Actor* Act, TESForm* Item) {

	char Command[80];
	
	if (Item) {
		sprintf(Command,"AddItemNS %08X 1", Item->refID);
		ConsoleInterface->RunScriptLine2(Command, Act, true);
	}

}

void CommandManager::RemoveItem(Actor* Act, TESForm* Item) {

	char Command[80];
	
	if (Item) {
		sprintf(Command,"RemoveItemNS %08X 1", Item->refID);
		ConsoleInterface->RunScriptLine2(Command, Act, true);
	}

}
#elif defined (SKYRIM)
CommandTable	commandTable;

const CommandInfo* commandsStart = (CommandInfo*)0x0124E880;
const CommandInfo* commandsEnd = (CommandInfo*)0x01255B30;

static const CommandTable::PatchLocation kPatch_ScriptCommands_Start[] =
{
	{ 0x00516B0B, 0x00 },
	{ 0x00516CD4, 0x04 },
	{ 0x00516D6D, 0x08 },
	{ 0x00517D87, 0x00 },
	{ 0x0051B33F, 0x00 },
	{ 0x00542459, 0x0C },
	{ 0x00542471, 0x00 },
	{ 0x00542489, 0x04 },
	{ 0x005424A5, 0x0C },
	{ 0x005424BF, 0x04 },
	{ 0x005424DD, 0x00 },
	{ 0x005424FF, 0x00 },
	{ 0x0054251D, 0x04 },
	{ 0x00542543, 0x00 },
	{ 0x0054255A, 0x00 },
	{ 0x005EA479, 0x20 },
	{ 0x005EA485, 0x10 },
	{ 0x005EA54E, 0x12 },
	{ 0x005EA591, 0x12 },
	{ 0x005EA59D, 0x14 },
	{ 0x005EA5D8, 0x12 },
	{ 0x005EA5E5, 0x14 },
	{ 0x005EA646, 0x12 },
	{ 0x005EA652, 0x14 },
	{ 0x005EA680, 0x12 },
	{ 0x005EA6F1, 0x12 },
	{ 0x005EA723, 0x12 },
	{ 0x005EA72F, 0x14 },
	{ 0x005EA785, 0x12 },
	{ 0x005EA7C3, 0x12 },
	{ 0x005EA7CF, 0x14 },
	{ 0x005EA878, 0x12 },
	{ 0x005EA8A7, 0x14 },
	{ 0x005EA984, 0x12 },
	{ 0x005EA9A6, 0x14 },
	{ 0x005EAABE, 0x12 },
	{ 0x005EAB08, 0x12 },
	{ 0x005EAB18, 0x14 },
	{ 0x005EABB1, 0x14 },
	{ 0x006A2D0C, 0x12 },
	{ 0x006A2D39, 0x14 },
	{ 0 },
};

static const CommandTable::PatchLocation kPatch_ScriptCommands_End[] =
{
	{ 0x00516C12, 0x08 },
	{ 0x00516CFF, 0x04 },
	{ 0 },
};

static const CommandTable::PatchLocation kPatch_ScriptCommands_MaxIdx[] =
{
	{ 0x00502C10 + 0x0087 + 1,	0 },
	{ 0x00516AD0 + 0x0029 + 2,	(UInt32)(-0x1001) },
	{ 0x00516C10 + 0x0000 + 6,	0 },
	{ 0x00517C20 + 0x0156 + 1,	(UInt32)(-0x1001) },
	{ 0x0051ABE0 + 0x0751 + 2,	(UInt32)(-0x1001) },
	{ 0x005E98C0 + 0x000D + 2,	(UInt32)(-0x1001) },
	{ 0x005E98C0 + 0x001D + 1,	(UInt32)(-0x1001) },
	{ 0x005E98C0 + 0x0029 + 2,	(UInt32)(-0x1001) },
	{ 0 },
};

static const CommandTable::PatchSet kPatchSet =
{
	kPatch_ScriptCommands_Start,
	kPatch_ScriptCommands_End,
	kPatch_ScriptCommands_MaxIdx
};

void (__cdecl * ToggleConsole)() = (void (__cdecl *)())0x00847210;
void __cdecl TrackToggleConsole() {

	if (!TheCommandManager->ConsolePatched) {
		commandTable.Init(0x1000, 0x1480);
		commandTable.Read(commandsStart, commandsEnd);
		commandTable.Add();
		commandTable.Add(&kCommandInfo_GetSKSEVersion);
		commandTable.Add(&kCommandInfo_ClearInvalidRegistrations);
		commandTable.Add();
		commandTable.Add(&kCommandInfo_GetVersion);
		commandTable.Add(&kCommandInfo_PurgeResources);
		commandTable.Add(&kCommandInfo_GetLocationName);
		commandTable.Add(&kCommandInfo_LoadSettings);
		commandTable.Add();
		commandTable.PatchEXE(&kPatchSet);
		TheCommandManager->ConsolePatched = true;
	}
	ToggleConsole();
}

void CreateConsoleCommandHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)ToggleConsole,	&TrackToggleConsole);
	DetourTransactionCommit();
}

CommandManager::CommandManager()
{
	_MESSAGE("Starting the command manager...");
	PapyrusInterface = NULL;
	ConsolePatched = false;
}
#endif

void CommandManager::PluginCommands::PurgeResources(double* result)
{
	TheRenderManager->device->EvictManagedResources();
	*result = 1;
}

void CommandManager::PluginCommands::GetLocationName(double* result)
{
	if (IsConsoleMode()) {
		const char* worldName = NULL;
		const char* cellName = TheShaderManager->ShaderConst.currentCell->GetEditorName();

		if (TESWorldSpace *currentWS = TheShaderManager->ShaderConst.currentWorldSpace) worldName = currentWS->GetEditorName();
		std::string s = "Worldspace: ";
		if (worldName != NULL)
			s += worldName;
		else
			s += "(no worldspace)";
		s += " - Cell: ";
		s += cellName;
		Console_Print(s.c_str());
	}
	*result = 1;
}

void CommandManager::PluginCommands::Screenshot(double* result)
{
	TheUtilityManager->Screenshot = true;
	*result = 1;
}

void CommandManager::PluginCommands::LoadSettings(double* result)
{
	TheSettingManager->LoadSettings();
	*result = 1;
}

void CommandManager::PluginCommands::SaveSettings(double* result, const char* Name)
{
	TheSettingManager->SaveSettings(Name);
	*result = 1;
}

void CommandManager::PluginCommands::GetSetting(double* result, const char* Name, const char* Section, const char* Setting)
{
	*result = TheSettingManager->GetSetting(Name, Section, Setting);
}

void CommandManager::PluginCommands::SetSetting(double* result, const char* Name, const char* Section, const char* Setting, float Value)
{
	TheSettingManager->SetSetting(Name, Section, Setting, Value);
}

void CommandManager::PluginCommands::GetValue(double* result, const char* Name, const char* Const)
{
	*result = TheShaderManager->GetShaderConst(Name, Const);
}

void CommandManager::PluginCommands::GetCustomValue(double* result, const char* Name, const char* Const)
{
	* result = TheEffectManager->GetCustomEffectConst(Name, Const);
}

void CommandManager::PluginCommands::SetCustomValue(double* result, const char* Name, const char* Const, float Value)
{
	TheEffectManager->SetCustomEffectConst(Name, Const, Value);
	*result = 1;
}

void CommandManager::PluginCommands::GetEnabled(double* result, const char* Name)
{
	*result = TheSettingManager->GetEnabled(Name);
}

void CommandManager::PluginCommands::SetEnabled(double* result, const char* Name, bool Value)
{
	TheEffectManager->SetEffectEnabled(Name, Value);
	*result = 1;
}

void CommandManager::PluginCommands::GetShaders(double* result, Script* scriptObj)
{
#if defined(OBLIVION)
	TheCommandManager->ArrayInterface->AssignCommandResult(TheCommandManager->GetOBSEArray(TheSettingManager->GetShaders(), scriptObj), result);
#endif
}

void CommandManager::PluginCommands::GetSections(double* result, const char* Name, Script* scriptObj)
{
#if defined(OBLIVION)
	TheCommandManager->ArrayInterface->AssignCommandResult(TheCommandManager->GetOBSEArray(TheSettingManager->GetSections(Name), scriptObj), result);
#endif
}

void CommandManager::PluginCommands::GetSettings(double* result, const char* Name, const char* Section, Script* scriptObj)
{
#if defined(OBLIVION)
	TheCommandManager->ArrayInterface->AssignCommandResult(TheCommandManager->GetOBSEArray(TheSettingManager->GetSettings(Name, Section), scriptObj), result);
#endif
}

void CommandManager::PluginCommands::EquipLeftWeapon(double* result, Actor* Act, TESObjectWEAP* Weapon)
{
	*result = 0;
#if defined(OBLIVION)
	if (TheSettingManager->SettingsMain.EquipmentMode) {
		bool Found = false;
		if (Act->process->GetProcessLevel() == 0 && Weapon->typeID == FormType::kFormType_Weapon) {
			if ((Weapon->type == TESObjectWEAP::kType_BladeOneHand || Weapon->type == TESObjectWEAP::kType_BluntOneHand) && !(Weapon->flags & 0x400) && !Weapon->scriptable.script) {
				ExtraContainerChanges::Data* (__cdecl * GetExtraContainerChangesData)(Actor*) = (ExtraContainerChanges::Data*(*)(Actor*))0x00485E00;
				ExtraContainerChanges::Data* ContainerChangesData = GetExtraContainerChangesData(Act);
				if (ContainerChangesData && ContainerChangesData->objList) {
					for (ExtraContainerChanges::Entry* ContainerChangesEntry = ContainerChangesData->objList; ContainerChangesEntry; ContainerChangesEntry = ContainerChangesEntry->next) {
						ExtraContainerChanges::EntryData* ContainerChangesEntryData = ContainerChangesEntry->data;
						if (ContainerChangesEntryData && ContainerChangesEntryData->type == Weapon) {
							Found = true;
							if (ContainerChangesEntryData->countDelta == 1) {
								ExtraContainerChanges::EntryExtendData* ContainerChangesEntryExtendData = ContainerChangesEntryData->extendData;
								if (ContainerChangesEntryExtendData && ContainerChangesEntryExtendData->data && ContainerChangesEntryExtendData->data->HasType(kExtraData_Worn))
									Found = false;
							}
							if (Found) break;
						}
					}
				}
				if (Found) {
					NiNode* ActorNode = Act->niNode;
					HighProcess* Proc = (HighProcess*)Act->process;
					ExtraContainerChanges::EntryData* ShieldData = Proc->equippedShieldData;
					NiORExtraData* ORData = TheEquipmentManager->GetORData(ActorNode);
					TESObjectARMO* LeftWeapon = TheEquipmentManager->GetLeftWeapon(Weapon);
					if (ShieldData && ShieldData->type == ORData->LeftWeapon)
						TheCommandManager->UnequipItem(Act, ORData->LeftWeapon);
					ORData->OriginalWeapon = Weapon;
					ORData->LeftWeapon = LeftWeapon;
					TheCommandManager->RemoveItem(Act, Weapon);
					TheCommandManager->AddItem(Act, LeftWeapon);
					TheCommandManager->EquipItem(Act, LeftWeapon);
					TheEquipmentManager->AddLeftWeaponRef(LeftWeapon);
				}
			}
		}
		*result = 1;
	}	
#endif
}

void CommandManager::PluginCommands::GetLeftWeapon(double* result, Actor* Act)
{
	*result = 0;
#if defined(OBLIVION)
	UInt32* refResult = (UInt32*)result;

	*refResult = NULL;
	if (TheSettingManager->SettingsMain.EquipmentMode) {
		NiORExtraData* ORData = TheEquipmentManager->GetORData(Act->niNode);
		if (ORData->OriginalWeapon) *refResult = ORData->OriginalWeapon->refID;
	}
#endif
}

void CommandManager::PluginCommands::CameraMove(double* result)
{
	TheSettingManager->SettingsCamera.Move = TheSettingManager->SettingsCamera.Pos.x + TheSettingManager->SettingsCamera.Pos.y + TheSettingManager->SettingsCamera.Pos.z;
	if (TheSettingManager->SettingsCamera.Move) {
		TheSettingManager->SettingsCamera.Look = false;
		TheSettingManager->SettingsCamera.MoveTo = false;
		TheSettingManager->SettingsCamera.RotateTo = false;
		TheSettingManager->SettingsCamera.LookTo = false;
	}
	*result = 1;
}

void CommandManager::PluginCommands::CameraRotate(double* result)
{
	TheSettingManager->SettingsCamera.Rotate = TheSettingManager->SettingsCamera.Pos.x + TheSettingManager->SettingsCamera.Pos.y + TheSettingManager->SettingsCamera.Pos.z;
	if (TheSettingManager->SettingsCamera.Rotate) {
		TheSettingManager->SettingsCamera.Look = false;
		TheSettingManager->SettingsCamera.MoveTo = false;
		TheSettingManager->SettingsCamera.RotateTo = false;
		TheSettingManager->SettingsCamera.LookTo = false;
	}
	*result = 1;
}

void CommandManager::PluginCommands::CameraLook(double* result)
{
	TheSettingManager->SettingsCamera.Look = true;
	TheSettingManager->SettingsCamera.Move = false;
	TheSettingManager->SettingsCamera.Rotate = false;
	TheSettingManager->SettingsCamera.RotateTo = false;
	TheSettingManager->SettingsCamera.LookTo = false;
	*result = 1;
}

void CommandManager::PluginCommands::CameraMoveTo(double* result)
{
	TheSettingManager->SettingsCamera.MoveTo = TheSettingManager->SettingsCamera.Pos.x + TheSettingManager->SettingsCamera.Pos.y + TheSettingManager->SettingsCamera.Pos.z;
	TheSettingManager->SettingsCamera.Move = false;
	TheSettingManager->SettingsCamera.Rotate = false;
	*result = 1;
}

void CommandManager::PluginCommands::CameraRotateTo(double* result)
{
	TheSettingManager->SettingsCamera.RotateTo = TheSettingManager->SettingsCamera.Pos.x + TheSettingManager->SettingsCamera.Pos.y + TheSettingManager->SettingsCamera.Pos.z;
	TheSettingManager->SettingsCamera.Move = false;
	TheSettingManager->SettingsCamera.Rotate = false;
	TheSettingManager->SettingsCamera.Look = false;
	TheSettingManager->SettingsCamera.LookTo = false;
	*result = 1;
}

void CommandManager::PluginCommands::CameraLookTo(double* result)
{
	TheSettingManager->SettingsCamera.LookTo = TheSettingManager->SettingsCamera.Pos.x + TheSettingManager->SettingsCamera.Pos.y + TheSettingManager->SettingsCamera.Pos.z;
	TheSettingManager->SettingsCamera.Move = false;
	TheSettingManager->SettingsCamera.Rotate = false;
	TheSettingManager->SettingsCamera.Look = false;
	TheSettingManager->SettingsCamera.RotateTo = false;
	*result = 1;
}

void CommandManager::PluginCommands::CameraReset(double* result)
{
	TheSettingManager->InitCamera();
	*result = 1;
}

void CommandManager::PluginCommands::CameraGetPosX(double* result)
{
	*result = TheSettingManager->SettingsCamera.CameraPos.x;
}

void CommandManager::PluginCommands::CameraGetPosY(double* result)
{
	*result = TheSettingManager->SettingsCamera.CameraPos.y;
}

void CommandManager::PluginCommands::CameraGetPosZ(double* result)
{
	*result = TheSettingManager->SettingsCamera.CameraPos.z;
}

void CommandManager::PluginCommands::GetAnimGroup(double* result, Actor* Act, int AnimGroup, int AnimType)
{
	*result = 0;
#if defined(OBLIVION)
	if (Act->process->GetProcessLevel() <= 1) {
		MiddleHighProcess* Proc = (MiddleHighProcess*)Act->process;
		ActorAnimData* AnimData = Proc->animData;
		if (AnimData) {
			AnimSequenceBase* AnimSequence = NULL;
			UInt16 AnimGroupKey = ((UInt8)AnimType << 8) | (UInt8)AnimGroup;
			if (ThisStdCall(0x00470960, AnimData->animsMap, AnimGroupKey, &AnimSequence) && AnimSequence) {
				if (AnimSequence->IsSingle()) {
					AnimSequenceSingleEx* AnimSequenceS = (AnimSequenceSingleEx*)AnimSequence;
					if (AnimSequenceS->ORAnim) *result = 1;
				}
				else {
					AnimSequenceMultipleEx* AnimSequenceM = (AnimSequenceMultipleEx*)AnimSequence;
					if (AnimSequenceM->ORAnim) *result = 1;
				}
			}
		}
	}
#endif
}

void CommandManager::PluginCommands::SetAnimGroup(double* result, Actor* Act, int Permanent, int PlayAnim, int AnimGroup, int AnimType, const char* Filename)
{
	*result = 0;
#if defined(OBLIVION)
	if (Act->process->GetProcessLevel() <= 1) {
		BSAnimGroupSequence* AnimGroupSequence = NULL;
		MiddleHighProcess* Proc = (MiddleHighProcess*)Act->process;
		ActorAnimDataEx* AnimData = (ActorAnimDataEx*)Proc->animData;
		if (AnimData) {
			AnimSequenceBase* AnimSequence = NULL;
			UInt16 AnimGroupKey = ((UInt8)AnimType << 8) | (UInt8)AnimGroup;
			if (ThisStdCall(0x00470960, AnimData->animsMap, AnimGroupKey, &AnimSequence) && AnimSequence) {
				NiTList<BSAnimGroupSequence>::Node* Iter = AnimData->ORAnims->start;
				while (Iter) {
					AnimGroupSequence = Iter->data;
					if (!stricmp(AnimGroupSequence->filePath, Filename)) break;
					Iter = Iter->next;
					AnimGroupSequence = NULL;
				}
				if (AnimSequence->IsSingle()) {
					AnimSequenceSingleEx* AnimSequenceS = (AnimSequenceSingleEx*)AnimSequence;
					if (Permanent) AnimSequenceS->ORAnim = AnimGroupSequence;
					if (Permanent == 0 || PlayAnim == 1) ThisStdCall(0x00474530, AnimData, AnimGroupSequence, AnimGroupKey, -1);
				}
				else {
					AnimSequenceMultipleEx* AnimSequenceM = (AnimSequenceMultipleEx*)AnimSequence;
					if (Permanent) AnimSequenceM->ORAnim = AnimGroupSequence;
					if (Permanent == 0 || PlayAnim == 1) ThisStdCall(0x00474530, AnimData, AnimGroupSequence, AnimGroupKey, -1);
				}
				*result = 1;
			}
		}
	}
#endif
}

void CommandManager::PluginCommands::ResetAnimGroup(double* result, Actor* Act, int AnimGroup, int AnimType)
{
	*result = 0;
#if defined(OBLIVION)
	if (Act->process->GetProcessLevel() <= 1) {
		MiddleHighProcess* Proc = (MiddleHighProcess*)Act->process;
		ActorAnimData* AnimData = Proc->animData;
		if (AnimData) {
			AnimSequenceBase* AnimSequence = NULL;
			UInt16 AnimGroupKey = ((UInt8)AnimType << 8) | (UInt8)AnimGroup;
			if (ThisStdCall(0x00470960, AnimData->animsMap, AnimGroupKey, &AnimSequence)) {
				if (AnimSequence->IsSingle()) {
					AnimSequenceSingleEx* AnimSequenceS = (AnimSequenceSingleEx*)AnimSequence;
					AnimSequenceS->ORAnim = NULL;
				}
				else {
					AnimSequenceMultipleEx* AnimSequenceM = (AnimSequenceMultipleEx*)AnimSequence;
					AnimSequenceM->ORAnim = NULL;
				}
				*result = 1;
			}
		}
	}
#endif
}

void CommandManager::PluginCommands::GetVersion(double* result)
{
	if (IsConsoleMode()) Console_Print(PluginVersion);
	*result = 1;
}
