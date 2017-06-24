#if defined(OBLIVION)
#include "obse\GameData.h"
#include "obse\GameOSDepend.h"
#include "obse\GameTasks.h"
#define kMessageBoxButtonTextYes 0x00B38CF8
#define kMessageBoxButtonTextNo 0x00B38D00
#define kGetWaterHeight 0x004CACE0
#define kGetFarPlaneDistance 0x00410EE0
#elif defined(SKYRIM)
#include "skse\GameMenus.h"
#include "UtilityManager.h"
#define kMessageBoxButtonTextYes 0x01B17FA0
#define kMessageBoxButtonTextNo 0x01B17FAC
#define kGetWaterHeight 0x004D62D0
#define kGetFarPlaneDistance 0x00B17D50
#endif

static const char** MessageBoxButtonTextYes		= (const char**)kMessageBoxButtonTextYes;
static const char** MessageBoxButtonTextNo		= (const char**)kMessageBoxButtonTextNo;

UtilityManager::UtilityManager()
{
	
	_MESSAGE("Starting the utility manager...");
	TheUtilityManager = this;

}

float UtilityManager::GetGameTime()
{

#if defined(OBLIVION)
	return TimeGlobals::GameHour() * 60.0f * 60.0f;
#elif defined(SKYRIM)
	class TimeGlobals
	{
	public:
		UInt32		unk00;			// 00
		TESGlobal*	GameYear;		// 04
		TESGlobal*	GameMonth;		// 08
		TESGlobal*	GameDay;		// 0C
		TESGlobal*	GameHour;		// 10
		TESGlobal*	GameDaysPassed;	// 14
		TESGlobal*	TimeScale;		// 18
		UInt32		unk1C;			// 1C
		UInt32		unk20;			// 20
	};
	TimeGlobals* Globals = (TimeGlobals*)0x012E5690;
	return Globals->GameHour->data * 60.0f * 60.0f;
#endif

}

bool UtilityManager::IsMenu(int MenuType)
{
	
#if defined(OBLIVION)
	bool (__cdecl * MenuMode)(int) = (bool(*)(int))0x0057C140;
	return MenuMode(MenuType);
#elif defined(SKYRIM)
	if (MenuType == 1009)
		return MenuManager::GetSingleton()->IsMenuOpen(&UIStringHolder::GetSingleton()->dialogueMenu);
	else
		return false;
#endif

}

bool UtilityManager::IsConsole()
{

#if defined(OBLIVION)
	return IsConsoleOpen();
#elif defined(SKYRIM)
	return MenuManager::GetSingleton()->IsMenuOpen(&UIStringHolder::GetSingleton()->console);
#endif

}

bool UtilityManager::IsVanityView()
{

#if defined(OBLIVION)
	return *(bool*)0x00B3BB04;
#elif defined(SKYRIM)
	return false;
#endif

}

UInt8 UtilityManager::GetSitSleepState(Actor* Act)
{
#if defined(OBLIVION)
	return Act->GetSitSleepState();
#elif defined(SKYRIM)
	return Act->actorState.flags04 >> 14 & 0xF;
#endif
}

float UtilityManager::GetWaterHeight(TESForm* Ref)
{
	return ThisCallF(kGetWaterHeight, Ref);
}

TESWaterForm* UtilityManager::GetWaterForm(TESObjectCELL* Cell)
{
#if defined(OBLIVION)
	return NULL;
#elif defined(SKYRIM)
	return (TESWaterForm*)ThisCall(0x004C0800, Cell);
#endif
}

float UtilityManager::GetFarPlaneDistance(SceneGraph* WorldSceneGraph)
{
	return ThisCallF(kGetFarPlaneDistance, WorldSceneGraph);
}

bool UtilityManager::ShowMessageBoxQuestion(const char* Message, void* Callback)
{
#if defined(OBLIVION)
	return ShowMessageBox(Message, (_ShowMessageBox_Callback)Callback, 1, *MessageBoxButtonTextYes, *MessageBoxButtonTextNo, NULL);
#elif defined(SKYRIM)
	bool (__cdecl * ShowMessageBox)(const char*, void*, UInt32, UInt32, UInt32, ...) = (bool (__cdecl *)(const char*, void*, UInt32, UInt32, UInt32, ...))0x0087AC60;
	return ShowMessageBox(Message, Callback, 1, 25, 4, *MessageBoxButtonTextYes, *MessageBoxButtonTextNo, NULL);
#endif
}

void UtilityManager::ShowMessage(const char* Message)
{
#if defined(OBLIVION)
	QueueUIMessage(Message, NULL, 1, 2.0f);
#elif defined(SKYRIM)
	void (__cdecl * ShowMessage)(const char*, const char*, bool) = (void (__cdecl *)(const char*, const char*, bool))0x008997A0;
	return ShowMessage(Message, NULL, 1);
#endif
}

void UtilityManager::RestoreCamera()
{
#if defined(OBLIVION)
	ThisCall(0x0066C600, *g_thePlayer);
#elif defined(SKYRIM)
	ThisCall(0x00730EE0, *g_thePlayer, 0);
#endif
}

void UtilityManager::SetCameraFOV(SceneGraph* WorldSceneGraph, float FoV)
{
#if defined(OBLIVION)
	ThisCall(0x00664A40, *g_thePlayer, FoV);
#elif defined(SKYRIM)
	ThisCall(0x00B17960, WorldSceneGraph, FoV, 0, NULL, 0);
#endif
}

void UtilityManager::MatrixMultiply(NiMatrix33* ss, NiMatrix33* sm1, NiMatrix33* sm2) {

	NiMatrix33L* s = (NiMatrix33L*)ss;
	NiMatrix33L* m1 = (NiMatrix33L*)sm1;
	NiMatrix33L* m2 = (NiMatrix33L*)sm2;

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			s->data[3 * y + x] = 0;
			for (int i = 0; i < 3; i++) {
				s->data[3 * y + x] += m1->data[3 * y + i] * m2->data[3 * i + x];
			}
		}
	}

}

void UtilityManager::MatrixVectorMultiply(NiPoint3* r, NiMatrix33* sm, NiPoint3* v) {
	
	NiMatrix33L* m = (NiMatrix33L*)sm;

	r->x = m->data[0] * v->x + m->data[1] * v->y + m->data[2] * v->z;
	r->y = m->data[3] * v->x + m->data[4] * v->y + m->data[5] * v->z;
	r->z = m->data[6] * v->x + m->data[7] * v->y + m->data[8] * v->z;

}

void UtilityManager::GenerateRotationMatrixZXY(NiMatrix33* sm, NiPoint3* v, bool degrees) {
	
	NiMatrix33L* m = (NiMatrix33L*)sm;
	float a = v->x;
	float b = v->y;
	float c = v->z;

	if (degrees) {
		a = DegreesToRadians(a);
		b = DegreesToRadians(b);
		c = DegreesToRadians(c);
	}
	m->data[0] = cos(a) * cos(c) - sin(a) * sin(b) * sin(c);
	m->data[1] = -cos(b) * sin(a);
	m->data[2] = cos(a) * sin(c) + cos(c) * sin(a) * sin(b);
	m->data[3] = cos(c) * sin(a) + cos(a) * sin(b) * sin(c);
	m->data[4] = cos(a) * cos(b);
	m->data[5] = sin(a) * sin(c) - cos(a) * cos(c) * sin(b);
	m->data[6] = -cos(b) * sin(c);
	m->data[7] = sin(b);
	m->data[8] = cos(b) * cos(c);

}

void UtilityManager::GenerateRotationMatrixXYZ(NiMatrix33* sm, NiPoint3* v, bool degrees) {
	
	NiMatrix33L* m = (NiMatrix33L*)sm;
	float a = v->x;
	float b = v->y;
	float c = v->z;

	if (degrees) {
		a = DegreesToRadians(a);
		b = DegreesToRadians(b);
		c = DegreesToRadians(c);
	}
	m->data[0] = cos(b) * cos(c);
	m->data[1] = -cos(b) * sin(c);
	m->data[2] = sin(b);
	m->data[3] = cos(a) * sin(c) + cos(c) * sin(a) * sin(b);
	m->data[4] = cos(a) * cos(c) - sin(a) * sin(b) * sin(c);
	m->data[5] = -cos(b) * sin(a);
	m->data[6] = sin(a) * sin(c) - cos(a) * cos(c) * sin(b);
	m->data[7] = cos(c) * sin(a) + cos(a) * sin(b) * sin(c);
	m->data[8] = cos(a) * cos(b);

}

float UtilityManager::RadiansToDegrees(float radians)
{
	return radians * 180 / M_PI;
}

float UtilityManager::DegreesToRadians(float degrees)
{
	return degrees * M_PI / 180;
}

void  UtilityManager::NormalizeVector3(D3DXVECTOR4* vector) {
	
	float len = sqrt((vector->x * vector->x) + (vector->y * vector->y) + (vector->z * vector->z));
	if (len) {
		vector->x = vector->x / len;
		vector->y = vector->y / len;
		vector->z = vector->z / len;
	}
	else {
		vector->x = 0.0f;
		vector->y = 0.0f;
		vector->z = 1.0f;
	}

}

#if defined(OBLIVION)
bool UtilityManager::IsMenu()
{
	bool(__cdecl * MenuMode)() = (bool(*)())0x00578F60;
	return MenuMode();
}

void UtilityManager::Play(TESSound* Sound)
{
	TESGameSound* GameSound = (TESGameSound*)ThisCall(0x006AE0A0, (*g_osGlobals)->sound, Sound->refID, 0x101, 0);
	ThisCall(0x006B7190, GameSound, 0);
	ThisCall(0x006B73E0, GameSound);
}

bool UtilityManager::IsFemale(Actor* Act)
{
	return ThisCall(0x005E1DF0, Act);
}

ExtraContainerChanges::EntryData* UtilityManager::GetInventoryItem(TESObjectREFR* Ref, UInt32 Index)
{
	return (ExtraContainerChanges::EntryData*)ThisCall(0x004D88F0, Ref, Index, 0);
}

void UtilityManager::UpdateInventory()
{
	Actor* Act = *g_thePlayer;
	void (*UpdateInventoryMenu)() = (void(*)())0x005AADC0;

	ThisStdCall(0x00668CC0, Act);
	ThisStdCall(0x006575B0, Act->process, Act, 1, 0, 0);
	ThisStdCall(0x0060E260, Act);
	UpdateInventoryMenu();
}

ExtraContainerChanges::Data* UtilityManager::GetExtraContainerChangesData(ExtraDataList* DataList)
{
	return (ExtraContainerChanges::Data*)ThisCall(0x0041E6F0, DataList);
}

float UtilityManager::GetExtraHealthValue(ExtraDataList* DataList)
{
	return ThisCallF(0x0041E810, DataList);
}

void UtilityManager::SetExtraHealthValue(ExtraDataList* DataList, float Value)
{
	ThisCall(0x0041ECB0, DataList, Value);
}

void UtilityManager::QueueIdle(ActorAnimData* AnimData, TESIdleForm* Idle, Actor* Act, UInt32 Flags)
{
	ThisCall(0x00477DB0, AnimData, Idle, Act, Flags, 3);
}

TESObjectExtraData * UtilityManager::GetRef(NiNode* ActorNode)
{
	return (TESObjectExtraData*)ThisCall(0x006FF9C0, ActorNode, "REF");
}

void UtilityManager::RemoveModel(char* WeaponModelPath)
{
	ThisCall(0x004384E0, ModelLoader::GetSingleton(), WeaponModelPath, 1, 1);
}

void UtilityManager::PurgeCellBuffers()
{
	TES* pTES = TES::GetSingleton();
	ThisCall(0x00442630, pTES, 1, 0);
	ThisCall(0x0043FC20, pTES, 0);
	ThisCall(0x00404C60, (*g_osGlobals), 1);
}

UInt8 UtilityManager::GetAnimSequence(NiTPointerMap<AnimSequenceBase>* AnimsMap, UInt16 AnimGroupKey, AnimSequenceBase** AnimSequence)
{
	return (UInt8)ThisCall(0x00470960, AnimsMap, AnimGroupKey, AnimSequence);
}
#elif defined(SKYRIM)
void UtilityManager::SetupCamera(NiCamera* Camera)
{
	ThisCall(0x00633670, TheRenderManager, Camera);
}

void UtilityManager::QueueNiNodeUpdate(Actor* Act)
{
	ThisCall(0x00730EE0, Act, 0);
}

void UtilityManager::UpdateOverShoulder(PlayerCamera* Camera, bool IsWeaponOut)
{
	ThisCall(0x0083C7E0, Camera, IsWeaponOut);
}

bool UtilityManager::IsCamSwitchControlEnabled(PlayerControls* Controls)
{
	return ThisCall(0x00772A20, Controls);
}

void UtilityManager::SetCameraState(TESCameraState* State)
{
	ThisCall(0x006533D0, State->camera, State->camera->thirdPersonState2);
}

BSFixedString* UtilityManager::CreateBSString(BSFixedString* FixedString, char* Value)
{
	return (BSFixedString*)ThisCall(0x00A511C0, FixedString, Value);
}

void UtilityManager::DisposeBSString(BSFixedString* FixedString)
{
	ThisCall(0x00A511B0, FixedString);
}
#endif