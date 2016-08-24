#pragma once

#if defined(OBLIVION)
#include "obse\PluginAPI.h"
#include "obse\GameObjects.h"
#elif defined(SKYRIM)
#include "skse\PluginAPI.h"
#endif

class CommandManager // Never disposed
{

public:
	CommandManager();

	class PluginCommands
	{
	public:
		void GetVersion(double* result);
		void PurgeResources(double* result);
		void GetLocationName(double* result);
		void GetCustomEffectValue(double* result, const char* Name, const char* Const);
		void SetCustomEffectValue(double* result, const char* Name, const char* Const, float Value);
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