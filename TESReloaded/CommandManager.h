#pragma once

#if defined(OBLIVION)
#include "obse\PluginAPI.h"
#include "obse\GameObjects.h"
#elif defined(SKYRIM)
#include "skse\PluginAPI.h"
#endif

class CommandManager
{

public:
	CommandManager();

	class PluginCommands
	{
	public:
		void PurgeResources(double* result);
		void GetLocationName(double* result);
		void Screenshot(double* result);
		void LoadSettings(double* result);
		void SaveSettings(double* result, const char* Name);
		void GetSetting(double* result, const char* Name, const char* Section, const char* Setting);
		void SetSetting(double* result, const char* Name, const char* Section, const char* Setting, float Value);
		void GetValue(double* result, const char* Name, const char* Const);
		void GetCustomValue(double* result, const char* Name, const char* Const);
		void SetCustomValue(double* result, const char* Name, const char* Const, float Value);
		void GetEnabled(double* result, const char* Name);
		void SetEnabled(double* result, const char* Name, bool Value);
		void GetShaders(double* result, Script* scriptObj);
		void GetSections(double* result, const char* Name, Script* scriptObj);
		void GetSettings(double* result, const char* Name, const char* Section, Script* scriptObj);
		void EquipLeftWeapon(double* result, Actor* Act, TESObjectWEAP* Weapon);
		void GetLeftWeapon(double* result, Actor* Act);
		void CameraMove(double* result);
		void CameraRotate(double* result);
		void CameraLook(double* result);
		void CameraMoveTo(double* result);
		void CameraRotateTo(double* result);
		void CameraLookTo(double* result);
		void CameraReset(double* result);
		void CameraGetPosX(double* result);
		void CameraGetPosY(double* result);
		void CameraGetPosZ(double* result);
		void GetAnimGroup(double* result, Actor* Act, int AnimGroup, int AnimType);
		void SetAnimGroup(double* result, Actor* Act, int Permanent, int PlayAnim, int AnimGroup, int AnimType, const char* Filename);
		void ResetAnimGroup(double* result, Actor* Act, int AnimGroup, int AnimType);
		void GetVersion(double* result);
	}; 

	PluginCommands					Commands;
#if defined(OBLIVION)
	void							RegisterCommands(const OBSEInterface* obse);
	OBSEArrayVarInterface::Array*	GetOBSEArray(SettingsList data, Script* callingScript);
	void							EquipItem(Actor* Act, TESForm* Item);
	void							UnequipItem(Actor* Act, TESForm* Item);
	void							AddItem(Actor* Act, TESForm* Item);
	void							RemoveItem(Actor* Act, TESForm* Item);
	
	OBSEConsoleInterface*			ConsoleInterface;
	OBSEArrayVarInterface*			ArrayInterface;
#elif defined(SKYRIM)
	SKSEPapyrusInterface*			PapyrusInterface;
	bool							ConsolePatched;
#endif

};

void CreateConsoleCommandHook();