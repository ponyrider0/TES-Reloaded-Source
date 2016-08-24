#include "SleepingMode.h"
#include "Hooking\detours\detours.h"
#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#define kMessageBoxServeSentenceText 0x00B38B30
#define kShowSleepWaitMenu 0x0057B420
#define kServeSentence 0x00670700
#define SleepingState 9
#elif defined(SKYRIM)
#include "skse\SafeWrite.h"
#include "skse\GameCamera.h"
#include "skse\GameReferences.h"
#define kMessageBoxServeSentenceText 0x01B19068
#define kShowSleepWaitMenu 0x00887B20
#define kServeSentence 0x0074B7E0
#define SleepingState 7
#endif

static const char** MessageBoxServeSentenceText = (const char**)kMessageBoxServeSentenceText;

static bool Served = false;

class SleepingMode {

public:
#if defined(OBLIVION)
	bool TrackRemoveWornItems(Actor* Act, UInt8 Arg2, int Arg3);
#elif defined(SKYRIM)
	void TrackSetFurnitureCameraState(TESFurniture* Furniture);
	int TrackProcessSleepWaitMenu(UInt32 Arg1);
#endif
	int TrackServeSentence();
};

#if defined(OBLIVION)
bool (__thiscall SleepingMode::* RemoveWornItems)(Actor*, UInt8, int);
bool (__thiscall SleepingMode::* TrackRemoveWornItems)(Actor*, UInt8, int);
bool SleepingMode::TrackRemoveWornItems(Actor* Act, UInt8 Arg2, int Arg3) { // ecx is MiddleHighProcess or HighProcess; this function must not be executed for the Player
	
	if (Act == (*g_thePlayer)) return 1;
	return (this->*RemoveWornItems)(Act, Arg2, Arg3);

}

bool (__cdecl * ProcessSleepWaitMenu)() = (bool (__cdecl *)())0x005D7090;
bool __cdecl TrackProcessSleepWaitMenu() {

	if ((*g_thePlayer)->JailedState && (*g_thePlayer)->isSleeping) Served = true;
	return ProcessSleepWaitMenu();

}

void (__cdecl * CloseSleepWaitMenu)() = (void (__cdecl *)())0x005D6A10;
void __cdecl TrackCloseSleepWaitMenu() {

	CloseSleepWaitMenu();
	if (Served) {
		Served = false;
		TheUtilityManager->ShowMessageBoxQuestion(*MessageBoxServeSentenceText, (void*)0x00671600);
	}

}

#elif defined(SKYRIM)
void (__thiscall SleepingMode::* SetFurnitureCameraState)(TESFurniture*);
void (__thiscall SleepingMode::* TrackSetFurnitureCameraState)(TESFurniture*);
void SleepingMode::TrackSetFurnitureCameraState(TESFurniture* Furniture) {

	PlayerCamera* Camera = (PlayerCamera*)this;
	UInt8 SitSleepState = TheUtilityManager->GetSitSleepState((Actor*)*g_thePlayer);

	if (SitSleepState == 7 && Camera->cameraState->stateId == PlayerCamera::kCameraState_Furniture) TheUtilityManager->ThisStdCall(0x00730EE0, *g_thePlayer, 0);
	(this->*SetFurnitureCameraState)(Furniture);

}

int (__thiscall SleepingMode::* ProcessSleepWaitMenu)(UInt32);
int (__thiscall SleepingMode::* TrackProcessSleepWaitMenu)(UInt32);
int SleepingMode::TrackProcessSleepWaitMenu(UInt32 Arg1) {
	
	bool IsPlayerSleeping = (*g_thePlayer)->flags725 & 4;

	if ((*g_thePlayer)->JailedState && IsPlayerSleeping) Served = true;
	bool r = (this->*ProcessSleepWaitMenu)(Arg1);
	if (Served && !IsPlayerSleeping) {
		Served = false;
		TheUtilityManager->ShowMessageBoxQuestion(*MessageBoxServeSentenceText, (void*)0x00499D70);
	}
	return r;

}
#endif

int (__thiscall SleepingMode::* ServeSentence)();
int (__thiscall SleepingMode::* TrackServeSentence)();
int SleepingMode::TrackServeSentence() {

#if defined(OBLIVION)
	TheUtilityManager->ThisStdCall(0x0066C600, *g_thePlayer);
	(*g_thePlayer)->unk61C = 0.0f;
	(*g_thePlayer)->Resurrect(0, 0, 0);
#elif defined(SKYRIM)
	TheUtilityManager->ThisStdCall(0x00730EE0, *g_thePlayer, 0);
#endif
	return (this->*ServeSentence)();

}

void (__cdecl * ShowSleepWaitMenu)(bool) = (void (__cdecl *)(bool))kShowSleepWaitMenu;
void __cdecl TrackShowSleepWaitMenu(bool IsSleeping) {

	UInt8 SitSleepState = TheUtilityManager->GetSitSleepState((Actor*)*g_thePlayer);

	if (SitSleepState == 0) {
		if (TheSettingManager->SettingsMain.SleepingModeRest)
			ShowSleepWaitMenu(0);
		else
			TheUtilityManager->ShowMessage(TheSettingManager->SettingsMain.SleepingModeRestMessage);
	}
	else if (SitSleepState == SleepingState)
		ShowSleepWaitMenu(1);

}

void CreateSleepingModeHook()
{
#if defined(OBLIVION)
	*((int *)&RemoveWornItems)				= 0x0064BAC0;
	TrackRemoveWornItems					= &SleepingMode::TrackRemoveWornItems;
#elif defined (SKYRIM)
	*((int *)&SetFurnitureCameraState)		= 0x0083D6E0;
	TrackSetFurnitureCameraState			= &SleepingMode::TrackSetFurnitureCameraState;
	*((int *)&ProcessSleepWaitMenu)			= 0x00887F90;
	TrackProcessSleepWaitMenu				= &SleepingMode::TrackProcessSleepWaitMenu;
#endif
	*((int *)&ServeSentence)				= kServeSentence;
	TrackServeSentence						= &SleepingMode::TrackServeSentence;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
#if defined(OBLIVION)
	DetourAttach(&(PVOID&)RemoveWornItems,		*((PVOID *)&TrackRemoveWornItems));
	DetourAttach(&(PVOID&)ProcessSleepWaitMenu,			   &TrackProcessSleepWaitMenu);
	DetourAttach(&(PVOID&)CloseSleepWaitMenu,			   &TrackCloseSleepWaitMenu);
#elif defined(SKYRIM)
	DetourAttach(&(PVOID&)SetFurnitureCameraState,	*((PVOID *)&TrackSetFurnitureCameraState));
	DetourAttach(&(PVOID&)ProcessSleepWaitMenu,		*((PVOID *)&TrackProcessSleepWaitMenu));
#endif
	DetourAttach(&(PVOID&)ServeSentence,			*((PVOID *)&TrackServeSentence));
	DetourAttach(&(PVOID&)ShowSleepWaitMenu,				   &TrackShowSleepWaitMenu);
	DetourTransactionCommit();

#if defined(OBLIVION)
	WriteRelJump(0x004AEA1C, 0x004AEAEE); // Enables the Player to get into the bed
	WriteRelJump(0x004AE961, 0x004AEAEE); // Enables the Player to get into the bed when in prison
	WriteRelJump(0x00672BFF, 0x00672C18); // Enables the rest key when in prison
#elif defined(SKYRIM)
	WriteRelJump(0x0049A351, 0x0049A367); // Enables the Player to get into the bed
	WriteRelJump(0x0049A30F, 0x0049A367); // Enables the Player to get into the bed when in prison
#endif

}