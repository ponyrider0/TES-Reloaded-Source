#ifndef	HOOKING_D3D9_CPP
#define	HOOKING_D3D9_CPP

#include "D3D9.hpp"
#include "..\D3D9Interface.h"

// Hook structure.
enum
{
	D3DFN_Direct3DCreate9 = 0
};

SDLLHook D3DHook =
{
	"D3D9.DLL", NULL,
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
		{ "Direct3DCreate9", TESRDirect3DCreate9},
		{ NULL, NULL }
	}
};

// Hook function.
IDirect3D9* FAR WINAPI TESRDirect3DCreate9(UINT sdk_version)
{
	Direct3DCreate9_t old_func = (Direct3DCreate9_t) D3DHook.Functions[D3DFN_Direct3DCreate9].OrigFn;
	IDirect3D9* d3d = old_func(sdk_version);

	return d3d ? new TESRDirect3D9(d3d) : 0;
}

#endif
