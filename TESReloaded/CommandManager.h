#pragma once

#if defined(OBLIVION)
#include "obse\PluginAPI.h"
#include "obse\GameObjects.h"
#elif defined(SKYRIM)
#include "skse\PluginAPI.h"
#endif
#include <d3dx9math.h>

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
		void SetShaderValue(double* result, const char* Name, const char* ConstantName, D3DXVECTOR4 Value);
		void SetCustomShaderValue(double* result, const char* Name, const char* ConstantName, D3DXVECTOR4 Value);
		void GetAnimGroup(double* result, Actor* Act, int AnimGroup, int AnimType);
		void SetAnimGroup(double* result, Actor* Act, int Permanent, int PlayAnim, int AnimGroup, int AnimType, const char* Filename);
		void ResetAnimGroup(double* result, Actor* Act, int AnimGroup, int AnimType);
	}; 

	PluginCommands					Commands;
#if defined(OBLIVION)
	void							RegisterCommands(const OBSEInterface* obse);
#elif defined(SKYRIM)
	SKSEPapyrusInterface*			PapyrusInterface;
	bool							ConsolePatched;
#endif

};

void CreateConsoleCommandHook();