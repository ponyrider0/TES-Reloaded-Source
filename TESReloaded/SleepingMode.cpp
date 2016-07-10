#if defined(OBLIVION)
#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#elif defined(SKYRIM)
#include "skse\SafeWrite.h"
#endif
#include "SleepingMode.h"
#include "Hooking\detours\detours.h"

static const UInt32 kRestKeyHook = 0x00672BF6;
static const UInt32 kRestKeyReturn1 = 0x00672BFD;
static const UInt32 kRestKeyReturn2 = 0x00672D94;
static const UInt32 kServeSentenceHook = 0x00671C1C;
static const UInt32 kServeSentenceReturn = 0x00671E16;

static const char** kServeSentenceText		= (const char**)0x00B38B30;
static const char** kButtonTextYes			= (const char**)0x00B38CF8;
static const char** kButtonTextNo			= (const char**)0x00B38D00;

static bool ServeSentence = false;

class SleepingMode {

public:
	bool TrackRemoveWornItems(Actor* Act, UInt8 Arg2, int Arg3);

};

bool (__thiscall SleepingMode::* RemoveWornItems)(Actor*, UInt8, int);
bool (__thiscall SleepingMode::* TrackRemoveWornItems)(Actor*, UInt8, int);
bool SleepingMode::TrackRemoveWornItems(Actor* Act, UInt8 Arg2, int Arg3) { // ecx is MiddleHighProcess or HighProcess; this function must not be executed for the Player
	
	if (Act == (*g_thePlayer)) return 1;
	return (this->*RemoveWornItems)(Act, Arg2, Arg3);

}

bool (__cdecl * ProcessRestRequest)() = (bool (__cdecl *)())0x005D7090;
bool __cdecl TrackProcessRestRequest() {
	
	bool r = ProcessRestRequest();

	if (r && (*g_thePlayer)->JailedState > 0 && (*g_thePlayer)->HoursToSleep == 0) ServeSentence = true;
	return r;

}

void QueueMessage() {

	QueueUIMessage(TheSettingManager->SettingsMain.SleepingModeRestMessage, NULL, 1, 2.0f);

}

static __declspec(naked) void RestKeyHook()
{
	
	__asm
	{
		mov		eax, dword ptr [ebx+0x058]
		cmp     byte ptr [eax+0x11D], 9
		jnz		loc_jump
		cmp     dword ptr [ebx+0x608], 0
		jmp		kRestKeyReturn1

	loc_jump:
		pushad
		call	QueueMessage
		popad
		jmp		kRestKeyReturn2
	}

}

bool ShowMessage() {
	
	bool r = false;

	if ((*g_thePlayer)->GetSitSleepState() == 0) {
		ServeSentence = false;
		ShowMessageBox(*kServeSentenceText, (_ShowMessageBox_Callback)0x00671600, 1, *kButtonTextYes, *kButtonTextNo, 0);
		r = true;
	}
	return r;

}

static __declspec(naked) void ServeSentenceHook()
{

	__asm
	{
		pushad
		cmp		ServeSentence, 1
		jnz		loc_return
		call	ShowMessage
		test	al, al
		jz		loc_return
		popad
		pop		edi
		pop		esi
		pop		ebx
		mov		esp, ebp
		pop		ebp
		retn	4

	loc_return:
		popad
		jmp		kServeSentenceReturn
	}

}

void CreateSleepingModeHook()
{

	*((int *)&RemoveWornItems)	= 0x0064BAC0;
	TrackRemoveWornItems		= &SleepingMode::TrackRemoveWornItems;
	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)RemoveWornItems,		*((PVOID *)&TrackRemoveWornItems));
	DetourAttach(&(PVOID&)ProcessRestRequest,			   &TrackProcessRestRequest);
	DetourTransactionCommit();

	if (!TheSettingManager->SettingsMain.SleepingModeRest) WriteRelJump(kRestKeyHook, (UInt32)RestKeyHook);
	WriteRelJump(kServeSentenceHook, (UInt32)ServeSentenceHook);
	WriteRelJump(0x004AE941, 0x004AEAA8);	// Enables the Player to get into the bed
	WriteRelJump(0x00672BFF, 0x00672C18);	// Enables the rest key when in prison
	WriteRelJump(0x005D7210, 0x005D7218);	// Does not use the bIntantLevelUp setting (you must rest/sleep to level up)

	SafeWrite8(0x00672D6E, 1); // Enables the level up when rest key is pressed

}
#elif defined(SKYRIM)
void CreateSleepingModeHook()
{

}
#endif