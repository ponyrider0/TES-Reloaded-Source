#ifndef	K32_HPP
#define	K32_HPP

// Function pointer types.
typedef HMODULE (WINAPI *LoadLibraryA_t)(LPCTSTR dllName);
typedef HMODULE (WINAPI *LoadLibraryW_t)(LPCWSTR dllName);
typedef FARPROC (WINAPI *GetProcAddress_t)(HMODULE hModule, LPCSTR lpProcName);

// Function prototypes.
HMODULE WINAPI TESRLoadLibraryA(LPCTSTR dllName);
//ODULE WINAPI TESRLoadLibraryW(LPCTSTR dllName);
FARPROC WINAPI TESRGetProcAddress(HMODULE hModule, LPCSTR lpProcName);

#endif
