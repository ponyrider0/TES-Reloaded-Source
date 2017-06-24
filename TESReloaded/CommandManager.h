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
		void GetLocationName(double* result);
		void SetCustomShaderEnabled(double* result, const char* Name, bool Value);
		void SetCustomConstant(double* result, const char* Name, D3DXVECTOR4 Value);
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