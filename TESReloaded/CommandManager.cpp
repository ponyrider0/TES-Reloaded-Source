#include "CommandInfo.h"
#if defined(OBLIVION)
#include "Animation.h"
#define CommandPrefix "OR"
#define PluginVersion "Oblivion Reloaded v6.0.0"
#elif defined(SKYRIM)
#include "skse\skse_version.h"
#include "skse\PapyrusVM.h"
#include "Hooking\detours\detours.h"
#include "CommandManager.h"
#define CommandPrefix "SR"
#define PluginVersion "Skyrim Reloaded v2.0.0"
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

bool Cmd_GetVersion_Execute(COMMAND_ARGS)
{
	TheCommandManager->Commands.GetVersion(result);
	return true;
}

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

bool Cmd_SetShaderValue_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Const[80];
	float Value[4];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Const, &Value[0], &Value[1], &Value[2], &Value[3])) TheCommandManager->Commands.SetShaderValue(result, Name, Const, Value);
	return true;
}

bool Cmd_SetCustomShaderValue_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Const[80];
	float Value[4];
	
	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Const, &Value[0], &Value[1], &Value[2], &Value[3])) TheCommandManager->Commands.SetCustomShaderValue(result, Name, Const, Value);
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

#if defined(SKYRIM)
#define CommandPrefix ""
DEFINE_COMMAND_PLUGIN_R(GetSKSEVersion, , "Returns the SKSE version", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(ClearInvalidRegistrations, , "Clears invalid event registrations", 0, 0, NULL, NULL);
#define CommandPrefix "SR"
#endif
DEFINE_COMMAND_PLUGIN_R(GetVersion,				, "Returns the plugin version", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(PurgeResources,			, "Purges resources from the video memory", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(GetLocationName,		, "Gets the name of the current worldspace and current cell - this is a console command only", 0, 0, NULL, NULL);
DEFINE_COMMAND_PLUGIN_R(SetShaderValue,			, "Sets the value of a shader", 0, 6, kParams_TwoString_FourFloat, NULL);
DEFINE_COMMAND_PLUGIN_R(SetCustomShaderValue,	, "Sets the value of a custom effect", 0, 6, kParams_TwoString_FourFloat, NULL);
DEFINE_COMMAND_PLUGIN_R(GetAnimGroup,			, "Gets if a specific animation for the specific group is set", 1, 2, kParams_TwoInts, NULL);
DEFINE_COMMAND_PLUGIN_R(SetAnimGroup,			, "Sets a specific animation for the specific group", 1, 5, kParams_FourIntsOneString, NULL);
DEFINE_COMMAND_PLUGIN_R(ResetAnimGroup,			, "Reset a permanent specific animation for the specific group to default", 1, 2, kParams_TwoInts, NULL);

#if defined(OBLIVION)
CommandManager::CommandManager() {

	_MESSAGE("Starting the command manager...");
	TheCommandManager = this;

}

void CommandManager::RegisterCommands(const OBSEInterface* obse)
{
	obse->SetOpcodeBase(0x2800);
	obse->RegisterCommand(&kCommandInfo_GetVersion);
	obse->RegisterCommand(&kCommandInfo_PurgeResources);
	obse->RegisterCommand(&kCommandInfo_GetLocationName);
	obse->RegisterCommand(&kCommandInfo_SetShaderValue);
	obse->RegisterCommand(&kCommandInfo_SetCustomShaderValue);
	obse->RegisterCommand(&kCommandInfo_GetAnimGroup);
	obse->RegisterCommand(&kCommandInfo_SetAnimGroup);
	obse->RegisterCommand(&kCommandInfo_ResetAnimGroup);
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
		commandTable.Add(&kCommandInfo_SetShaderValue);
		commandTable.Add(&kCommandInfo_SetCustomShaderValue);
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
	ConsolePatched = false;

}
#endif

void CommandManager::PluginCommands::GetVersion(double* result)
{
	if (IsConsoleMode()) Console_Print(PluginVersion);
	*result = 1;
}

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

void CommandManager::PluginCommands::SetShaderValue(double* result, const char* Name, const char* ConstantName, D3DXVECTOR4 Value)
{
	TheShaderManager->SetShaderValue(Name, ConstantName, Value);
	*result = 1;
}

void CommandManager::PluginCommands::SetCustomShaderValue(double* result, const char* Name, const char* ConstantName, D3DXVECTOR4 Value)
{
	TheShaderManager->SetCustomShaderValue(Name, ConstantName, Value);
	*result = 1;
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
