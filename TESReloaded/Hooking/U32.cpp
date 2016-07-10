#ifndef	HOOKING_U32_CPP
#define	HOOKING_U32_CPP

#include "U32.hpp"

// Hook structure.
enum
{
	U32FN_GetActiveWindow = 0
};

SDLLHook U32Hook =
{
	"user32.dll", NULL,
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
		{ "GetActiveWindow", TESRGetActiveWindow},
		{ NULL, NULL }
	}
};

extern class DebugWindow *dw;

// Hook function.
HWND WINAPI TESRGetActiveWindow(void)
{
	GetActiveWindow_t old_func = (GetActiveWindow_t) U32Hook.Functions[U32FN_GetActiveWindow].OrigFn;
	HWND hWnd = old_func();

	return hWnd;
}

#endif
