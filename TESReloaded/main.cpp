#include "obse\GameData.h"
#include "obse_common\SafeWrite.h"
#include "obse\ParamInfos.h"
#include "RenderHook.h"
#include "ShaderIOHook.h"
#include "FileHook.h"
#include "Grass.h"
#include "WindowedMode.h"
#include "EventInterface.h"
#include "CombatMode.h"
#include "RenderManager.h"
#include "CameraMode.h"
#include "SleepingMode.h"
#include "Animation.h"
#include "Hooking\apihijack.h"
#include "Hooking\apihijack.cpp"
#include "Hooking\D3D9.cpp"
#include "Hooking\K32.cpp"
#include "Hooking\U32.cpp"

static ParamInfo kParams_TwoString[2] = 
{
	{ "string",	kParamType_String, 0 },
	{ "string",	kParamType_String, 0 },
};

static ParamInfo kParams_TwoString_OneFloat[3] = 
{
	{ "string",	kParamType_String, 0 },
	{ "string",	kParamType_String, 0 },
	{ "float",  kParamType_Float,  0 },
};

static ParamInfo kParams_ThreeString[3] = 
{
	{ "string",	kParamType_String, 0 },
	{ "string",	kParamType_String, 0 },
	{ "string",	kParamType_String, 0 },
};

static ParamInfo kParams_ThreeString_OneFloat[4] = 
{
	{ "string",	kParamType_String, 0 },
	{ "string",	kParamType_String, 0 },
	{ "string",	kParamType_String, 0 },
	{ "float",  kParamType_Float,  0 },
};

static ParamInfo kParams_OneObject[1] = 
{
	{ "object",	kParamType_TESObject, 0 },
};

static ParamInfo kParams_ThreeFloats[3] =
{
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
};

static ParamInfo kParams_OneActorThreeFloats[4] =
{
	{ "objectRef", kParamType_ObjectRef, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
};

static ParamInfo kParams_FourIntsOneString[5] =
{
	{ "int", kParamType_Integer, 0 },
	{ "int", kParamType_Integer, 0 },
	{ "int", kParamType_Integer, 0 },
	{ "int", kParamType_Integer, 0 },
	{ "string",	kParamType_String,	0 },
};

static const UInt32 kDecalLifetimeHook = 0x005DF53C;
static const UInt32 kDecalLifetimeReturn = 0x005DF55D;

IDebugLog			debugLog("OblivionReloaded.log");
PluginHandle		pluginHandle = kPluginHandle_Invalid;

OBSEArrayVarInterface::Array* StringMapFromStdMap(const std::map<std::string, OBSEArrayVarInterface::Element>& data, Script* callingScript)
{

	OBSEArrayVarInterface::Array* arr = TheOBSEManager->ArrayInterface->CreateStringMap(NULL, NULL, 0, callingScript);

	for (std::map<std::string, OBSEArrayVarInterface::Element>::const_iterator iter = data.begin(); iter != data.end(); ++iter) {
		TheOBSEManager->ArrayInterface->SetElement(arr, iter->first.c_str(), iter->second);
	}
	return arr;

}

void DecalLifetime(int DecalType)
{

	if (DecalType == 0)
		*kDecalLifetime = 0;
	else if (DecalType == 1)
		*kDecalLifetime = 10;
	else if (DecalType == 2)
		if (*kDecalLifetime == 0) *kDecalLifetime = 15;

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

bool Cmd_ORPurgeResources_Execute(COMMAND_ARGS)
{
	TheRenderManager->device->EvictManagedResources();
	return true;
}

bool Cmd_ORGetLocationName_Execute(COMMAND_ARGS)
{
	const char* worldName = NULL;
	const char* cellName = TheShaderManager->ShaderConst.currentCell->GetEditorName();
	
	if (TESWorldSpace *currentWS = TheShaderManager->ShaderConst.currentWorldSpace)
		worldName = currentWS->GetEditorName();

	std::string s = "Worldspace: ";
	if (worldName != NULL)
		s+= worldName;
	else
		s+= "(no worldspace/interior)";
	s+= " - Cell: ";
	s+= cellName;
	Console_Print(s.c_str());
	return true;
}

bool Cmd_ORScreenshot_Execute(COMMAND_ARGS)
{
	g_ScreenshotRequest = true;
	return true;
}

bool Cmd_ORLoadSettings_Execute(COMMAND_ARGS)
{
	TheSettingManager->LoadSettings();
	return true;
}

bool Cmd_ORSaveSettings_Execute(COMMAND_ARGS)
{
	char Name[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name))
		TheSettingManager->SaveSettings(Name);
	return true;
}

bool Cmd_ORGetSetting_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Section[80];
	char Setting[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Section, &Setting))
		*result = TheSettingManager->GetSetting(Name, Section, Setting);
	return true;
}

bool Cmd_ORSetSetting_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Section[80];
	char Setting[80];
	float Value;
	
	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Section, &Setting, &Value))
		TheSettingManager->SetSetting(Name, Section, Setting, Value);
	return true;
}

bool Cmd_ORGetValue_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Const[80];
	
	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Const))
		*result = TheShaderManager->GetShaderConst(Name, Const);
	return true;
}

bool Cmd_ORGetCustomValue_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Const[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Const))
		*result = TheEffectManager->GetCustomEffectConst(Name, Const);
	return true;
}

bool Cmd_ORSetCustomValue_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Const[80];
	float Value;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Const, &Value))
		TheEffectManager->SetCustomEffectConst(Name, Const, Value);
	return true;
}

bool Cmd_ORGetEnabled_Execute(COMMAND_ARGS)
{
	char Name[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name))
		*result = TheSettingManager->GetEnabled(Name);
	return true;
}

bool Cmd_ORSetEnabled_Execute(COMMAND_ARGS)
{
	char Name[80];
	int Value;
	
	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Value))
		TheEffectManager->SetEffectEnabled(Name, Value);
	return true;
}

bool Cmd_ORGetShaders_Execute(COMMAND_ARGS)
{
	OBSEArrayVarInterface::Array* OBSEMap = StringMapFromStdMap(TheSettingManager->GetShaders(), scriptObj);
	TheOBSEManager->ArrayInterface->AssignCommandResult(OBSEMap, result);
	return true;
}

bool Cmd_ORGetSections_Execute(COMMAND_ARGS)
{
	char Name[80];
			
	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name)) {
		OBSEArrayVarInterface::Array* OBSEMap = StringMapFromStdMap(TheSettingManager->GetSections(Name), scriptObj);
		TheOBSEManager->ArrayInterface->AssignCommandResult(OBSEMap, result);
	}
	return true;
}

bool Cmd_ORGetSettings_Execute(COMMAND_ARGS)
{
	char Name[80];
	char Section[80];

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Name, &Section)) {
		OBSEArrayVarInterface::Array* OBSEMap = StringMapFromStdMap(TheSettingManager->GetSettings(Name, Section), scriptObj);
		TheOBSEManager->ArrayInterface->AssignCommandResult(OBSEMap, result);
	}
	return true;
}

bool Cmd_OREquipLeftWeapon_Execute(COMMAND_ARGS)
{
	if (TheSettingManager->SettingsMain.EquipmentMode) {
		TESObjectWEAP* Weapon = NULL;
		bool Found = false;
		if (ExtractArgs(PASS_EXTRACT_ARGS, &Weapon)) {
			Actor* Act = (Actor*)thisObj;
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
							TheOBSEManager->UnequipItem(Act, ORData->LeftWeapon);
						ORData->OriginalWeapon = Weapon;
						ORData->LeftWeapon = LeftWeapon;
						TheOBSEManager->RemoveItem(Act, Weapon);
						TheOBSEManager->AddItem(Act, LeftWeapon);
						TheOBSEManager->EquipItem(Act, LeftWeapon);
						TheEquipmentManager->AddLeftWeaponRef(LeftWeapon);
					}
				}
			}
		}
	}
	return true;
}

bool Cmd_ORGetLeftWeapon_Execute(COMMAND_ARGS)
{
	UInt32* refResult = (UInt32*)result;

	*refResult = NULL;
	if (TheSettingManager->SettingsMain.EquipmentMode) {
		Actor* Act = (Actor*)thisObj;
		NiORExtraData* ORData = TheEquipmentManager->GetORData(Act->niNode);
		if (ORData->OriginalWeapon)
			*refResult = ORData->OriginalWeapon->refID;
	}
	return true;
}

bool Cmd_ORCameraMove_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.Ref, &TheSettingManager->SettingsCamera.Pos.x, &TheSettingManager->SettingsCamera.Pos.y, &TheSettingManager->SettingsCamera.Pos.z)) {
		TheSettingManager->SettingsCamera.Move = TheSettingManager->SettingsCamera.Pos.x + TheSettingManager->SettingsCamera.Pos.y + TheSettingManager->SettingsCamera.Pos.z;
		if (TheSettingManager->SettingsCamera.Move) {
			TheSettingManager->SettingsCamera.Look = false;
			TheSettingManager->SettingsCamera.MoveTo = false;
			TheSettingManager->SettingsCamera.RotateTo = false;
			TheSettingManager->SettingsCamera.LookTo = false;
		}
	}
	return true;
}

bool Cmd_ORCameraRotate_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.Ref, &TheSettingManager->SettingsCamera.AngX, &TheSettingManager->SettingsCamera.AngY, &TheSettingManager->SettingsCamera.AngZ)) {
		TheSettingManager->SettingsCamera.Rotate = TheSettingManager->SettingsCamera.Pos.x + TheSettingManager->SettingsCamera.Pos.y + TheSettingManager->SettingsCamera.Pos.z;
		if (TheSettingManager->SettingsCamera.Rotate) {
			TheSettingManager->SettingsCamera.Look = false;
			TheSettingManager->SettingsCamera.MoveTo = false;
			TheSettingManager->SettingsCamera.RotateTo = false;
			TheSettingManager->SettingsCamera.LookTo = false;
		}
	}
	return true;
}

bool Cmd_ORCameraLook_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.Ref, &TheSettingManager->SettingsCamera.LookPos.x, &TheSettingManager->SettingsCamera.LookPos.y, &TheSettingManager->SettingsCamera.LookPos.z)) {
		TheSettingManager->SettingsCamera.Look = true;
		TheSettingManager->SettingsCamera.Move = false;
		TheSettingManager->SettingsCamera.Rotate = false;
		TheSettingManager->SettingsCamera.RotateTo = false;
		TheSettingManager->SettingsCamera.LookTo = false;
	}
	return true;
}

bool Cmd_ORCameraMoveTo_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.Pos.x, &TheSettingManager->SettingsCamera.Pos.y, &TheSettingManager->SettingsCamera.Pos.z)) {
		TheSettingManager->SettingsCamera.MoveTo = TheSettingManager->SettingsCamera.Pos.x + TheSettingManager->SettingsCamera.Pos.y + TheSettingManager->SettingsCamera.Pos.z;
		TheSettingManager->SettingsCamera.Move = false;
		TheSettingManager->SettingsCamera.Rotate = false;
	}
	return true;
}

bool Cmd_ORCameraRotateTo_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.AngX, &TheSettingManager->SettingsCamera.AngY, &TheSettingManager->SettingsCamera.AngZ)) {
		TheSettingManager->SettingsCamera.RotateTo = TheSettingManager->SettingsCamera.Pos.x + TheSettingManager->SettingsCamera.Pos.y + TheSettingManager->SettingsCamera.Pos.z;
		TheSettingManager->SettingsCamera.Move = false;
		TheSettingManager->SettingsCamera.Rotate = false;
		TheSettingManager->SettingsCamera.Look = false;
		TheSettingManager->SettingsCamera.LookTo = false;
	}
	return true;
}

bool Cmd_ORCameraLookTo_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(PASS_EXTRACT_ARGS, &TheSettingManager->SettingsCamera.LookPos.x, &TheSettingManager->SettingsCamera.LookPos.y, &TheSettingManager->SettingsCamera.LookPos.z)) {
		TheSettingManager->SettingsCamera.LookTo = TheSettingManager->SettingsCamera.Pos.x + TheSettingManager->SettingsCamera.Pos.y + TheSettingManager->SettingsCamera.Pos.z;
		TheSettingManager->SettingsCamera.Move = false;
		TheSettingManager->SettingsCamera.Rotate = false;
		TheSettingManager->SettingsCamera.Look = false;
		TheSettingManager->SettingsCamera.RotateTo = false;
	}
	return true;
}

bool Cmd_ORCameraReset_Execute(COMMAND_ARGS)
{
	TheSettingManager->InitCamera();
	return true;
}

bool Cmd_ORCameraGetPosX_Execute(COMMAND_ARGS)
{
	*result = TheSettingManager->SettingsCamera.CameraPos.x;
	return true;
}

bool Cmd_ORCameraGetPosY_Execute(COMMAND_ARGS)
{
	*result = TheSettingManager->SettingsCamera.CameraPos.y;
	return true;
}

bool Cmd_ORCameraGetPosZ_Execute(COMMAND_ARGS)
{
	*result = TheSettingManager->SettingsCamera.CameraPos.z;
	return true;
}

bool Cmd_ORGetAnimGroup_Execute(COMMAND_ARGS)
{
	Actor* Act = (Actor*)thisObj;
	int AnimGroup = 0;
	int AnimType = 0;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &AnimGroup, &AnimType) && Act->process->GetProcessLevel() <= 1) {
		MiddleHighProcess* Proc = (MiddleHighProcess*)Act->process;
		ActorAnimData* AnimData = Proc->animData;
		if (AnimData) {
			AnimSequenceBase* AnimSequence = NULL;
			UInt16 AnimGroupKey = ((UInt8)AnimType << 8) | (UInt8)AnimGroup;
			if (ThisStdCall(0x00470960, AnimData->animsMap, AnimGroupKey, &AnimSequence) && AnimSequence) {
				if (AnimSequence->IsSingle()) {
					AnimSequenceSingleEx* AnimSequenceS = (AnimSequenceSingleEx*)AnimSequence;
					if (AnimSequenceS->ORAnim) *result = true;
				}
				else {
					AnimSequenceMultipleEx* AnimSequenceM = (AnimSequenceMultipleEx*)AnimSequence;
					if (AnimSequenceM->ORAnim) *result = true;
				}
			}
		}
	}
	return true;
}

bool Cmd_ORSetAnimGroup_Execute(COMMAND_ARGS)
{
	Actor* Act = (Actor*)thisObj;
	int Permanent = 0;
	int PlayAnim = 0;
	int AnimGroup = 0;
	int AnimType = 0;
	char Filename[80];
	BSAnimGroupSequence* AnimGroupSequence = NULL;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &Permanent, &PlayAnim, &AnimGroup, &AnimType, &Filename) && Act->process->GetProcessLevel() <= 1) {
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
			}
		}
	}
	return true;
}

bool Cmd_ORResetAnimGroup_Execute(COMMAND_ARGS)
{
	Actor* Act = (Actor*)thisObj;
	int AnimGroup = 0;
	int AnimType = 0;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &AnimGroup, &AnimType) && Act->process->GetProcessLevel() <= 1) {
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
			}
		}
	}
	return true;
}

DEFINE_COMMAND_PLUGIN(ORPurgeResources, "Purges resources from the video memory", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ORGetLocationName, "Gets the name of the current worldspace and current cell - this is a console command only", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ORScreenshot, "Takes a screenshot without the HUD", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ORLoadSettings, "Loads settings from configuration files", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ORSaveSettings, "Saves settings into the shader configuration file", 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(ORGetSetting, "Gets the current setting of a shader", 0, 3, kParams_ThreeString);
DEFINE_COMMAND_PLUGIN(ORSetSetting, "Sets the current setting of a shader", 0, 4, kParams_ThreeString_OneFloat);
DEFINE_COMMAND_PLUGIN(ORGetValue, "Gets the current value of a shader", 0, 2, kParams_TwoString);
DEFINE_COMMAND_PLUGIN(ORGetCustomValue, "Gets the current value of a custom effect", 0, 2, kParams_TwoString);
DEFINE_COMMAND_PLUGIN(ORSetCustomValue, "Sets the current value of a custom effect", 0, 3, kParams_TwoString_OneFloat);
DEFINE_COMMAND_PLUGIN(ORGetEnabled, "Gets the status of a shader", 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(ORSetEnabled, "Enables/disables an effect", 0, 2, kParams_OneString_OneInt);
DEFINE_COMMAND_PLUGIN(ORGetShaders, "Gets the shaders list", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ORGetSections, "Gets the sections list of a shader", 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(ORGetSettings, "Gets settings of a specific section of a shader", 0, 2, kParams_TwoString);
DEFINE_COMMAND_PLUGIN(OREquipLeftWeapon, "Equips a weapon in the left hand", 1, 1, kParams_OneObject);
DEFINE_COMMAND_PLUGIN(ORGetLeftWeapon, "Gets the original weapon in the left hand", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(ORCameraMove, "Moves the camera from the actor's head", 0, 4, kParams_OneActorThreeFloats);
DEFINE_COMMAND_PLUGIN(ORCameraRotate, "Rotates the camera in relation of the actor's head", 0, 4, kParams_OneActorThreeFloats);
DEFINE_COMMAND_PLUGIN(ORCameraLook, "Forces the camera to look at the actor", 0, 4, kParams_OneActorThreeFloats);
DEFINE_COMMAND_PLUGIN(ORCameraMoveTo, "Moves the camera in the world", 0, 3, kParams_ThreeFloats);
DEFINE_COMMAND_PLUGIN(ORCameraRotateTo, "Rotates the camera in the world", 0, 3, kParams_ThreeFloats);
DEFINE_COMMAND_PLUGIN(ORCameraLookTo, "Forces the camera to look to a point in the world", 0, 3, kParams_ThreeFloats);
DEFINE_COMMAND_PLUGIN(ORCameraReset, "Restores the camera to the normal state", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ORCameraGetPosX, "Gets the camera position for the x axis", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ORCameraGetPosY, "Gets the camera position for the y axis", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ORCameraGetPosZ, "Gets the camera position for the z axis", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ORGetAnimGroup, "Gets if a specific animation for the specific group is set", 1, 2, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(ORSetAnimGroup, "Sets a specific animation for the specific group", 1, 5, kParams_FourIntsOneString);
DEFINE_COMMAND_PLUGIN(ORResetAnimGroup, "Reset a permanent specific animation for the specific group to default", 1, 2, kParams_TwoInts);

extern "C" {

bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{

	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "OblivionReloaded";
	info->version = 4;
#if 0 // This hooks the D3D Device. Only for developing/debugging when needed.
	if(!obse->isEditor)
	{
		HookAPICalls(&K32Hook);
		HookAPICalls(&U32Hook);
	}
#endif
	return true;
}

bool OBSEPlugin_Load(const OBSEInterface * obse)
{

	pluginHandle = obse->GetPluginHandle();
	
	obse->SetOpcodeBase(0x2800);
	obse->RegisterCommand(&kCommandInfo_ORPurgeResources);
	obse->RegisterCommand(&kCommandInfo_ORGetLocationName);
	obse->RegisterCommand(&kCommandInfo_ORScreenshot);
	obse->RegisterCommand(&kCommandInfo_ORLoadSettings);
	obse->RegisterCommand(&kCommandInfo_ORSaveSettings);
	obse->RegisterCommand(&kCommandInfo_ORGetSetting);
	obse->RegisterCommand(&kCommandInfo_ORSetSetting);
	obse->RegisterCommand(&kCommandInfo_ORGetValue);
	obse->RegisterCommand(&kCommandInfo_ORGetCustomValue);
	obse->RegisterCommand(&kCommandInfo_ORSetCustomValue);
	obse->RegisterCommand(&kCommandInfo_ORGetEnabled);
	obse->RegisterCommand(&kCommandInfo_ORSetEnabled);
	obse->RegisterTypedCommand(&kCommandInfo_ORGetShaders, kRetnType_Array);
	obse->RegisterTypedCommand(&kCommandInfo_ORGetSections, kRetnType_Array);
	obse->RegisterTypedCommand(&kCommandInfo_ORGetSettings, kRetnType_Array);
	obse->RegisterCommand(&kCommandInfo_OREquipLeftWeapon);
	obse->RegisterCommand(&kCommandInfo_ORGetLeftWeapon);
	obse->RegisterCommand(&kCommandInfo_ORCameraMove);
	obse->RegisterCommand(&kCommandInfo_ORCameraRotate);
	obse->RegisterCommand(&kCommandInfo_ORCameraLook);
	obse->RegisterCommand(&kCommandInfo_ORCameraMoveTo);
	obse->RegisterCommand(&kCommandInfo_ORCameraRotateTo);
	obse->RegisterCommand(&kCommandInfo_ORCameraLookTo);
	obse->RegisterCommand(&kCommandInfo_ORCameraReset);
	obse->RegisterCommand(&kCommandInfo_ORCameraGetPosX);
	obse->RegisterCommand(&kCommandInfo_ORCameraGetPosY);
	obse->RegisterCommand(&kCommandInfo_ORCameraGetPosZ);
	obse->RegisterCommand(&kCommandInfo_ORGetAnimGroup);
	obse->RegisterCommand(&kCommandInfo_ORSetAnimGroup);
	obse->RegisterCommand(&kCommandInfo_ORResetAnimGroup);

	if (!obse->isEditor)
	{
		TheSettingManager = SettingManager::GetSingleton();
		TheOBSEManager = OBSEManager::GetSingleton();

		TheOBSEManager->ConsoleInterface = (OBSEConsoleInterface*)obse->QueryInterface(kInterface_Console);
		TheOBSEManager->ArrayInterface = (OBSEArrayVarInterface*)obse->QueryInterface(kInterface_ArrayVar);
		TheSettingManager->LoadSettings();
		CreateShaderIOHook();
		CreateRenderHook();
		CreateFormLoadHook();
		CreateSettingsHook();
		CreateAnimationHook();
		if (TheSettingManager->SettingsMain.EnableBloodLens) CreateEventHook();
		if (TheSettingManager->SettingsMain.EnableGrass) CreateGrassHook();
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