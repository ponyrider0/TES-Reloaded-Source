#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#include "obse\GameData.h"
#endif
#include "Grass.h"

#if defined(OBLIVION)
static const UInt32 kGrassHook = 0x004EBF87;
static const UInt32 kGrassReturn = 0x004EC4E8;

void (__cdecl * DrawGrass)(TESObjectCELL*, NiNode*, float, float, float, float, float, int, float, float, float) = (void (__cdecl *)(TESObjectCELL*, NiNode*, float, float, float, float, float, int, float, float, float))0x004EB3F0;

void UpdateGrass(TESObjectCELL* Cell, NiNode* GrassNode, float CameraLocalTranslateX, float CameraLocalTranslateY, float CameraLocalTranslateZ, float CameraForwardX, float CameraForwardY, int Arg8, float StartFadingDistance, float EndDistance, float Arg11)
{

	TES* pTES = TES::GetSingleton();
	
	for (int x = 0 ; x < *SettingGridsToLoad ; x++) {
		for (int y = 0 ; y < *SettingGridsToLoad ; y++) {
			DrawGrass(pTES->gridCellArray->GetGridEntry(x, y)->cell, GrassNode, CameraLocalTranslateX, CameraLocalTranslateY, CameraLocalTranslateZ, CameraForwardX, CameraForwardY, Arg8, StartFadingDistance, EndDistance, Arg11);
		}
	}

}

static __declspec(naked) void GrassHook()
{

	__asm
	{
		call	UpdateGrass
		jmp		kGrassReturn
	}

}

void CreateGrassHook() {
	
	WriteRelJump(kGrassHook, (UInt32)GrassHook);

}
#endif