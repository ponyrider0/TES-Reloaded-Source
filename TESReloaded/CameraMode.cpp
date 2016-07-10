#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#include "CameraMode.h"
#include "Hooking\detours\detours.h"

static const UInt32 kUpdateCameraHook = 0x0066BE6E;
static const UInt32 kUpdateCameraReturn = 0x0066BE7C;
static const UInt32 kThirdPersonCameraHook = 0x00671AC9;
static const UInt32 kThirdPersonCameraReturn = 0x00671AD0;
static const UInt32 kOnCameraPOVHook = 0x00672FDA;
static const UInt32 kOnCameraPOVReturn = 0x00672FE2;
static const UInt32 kOnDialogHook = 0x0059EEDB;
static const UInt32 kOnDialogReturn = 0x0059EEE6;
static const UInt32 kOnExitingDialogHook = 0x0059E5EB;
static const UInt32 kOnExitingDialogReturn = 0x0059E5F8;
static const UInt32 kHUDReticleHook = 0x00673421;
static const UInt32 kHUDReticleReturn1 = 0x0067342A;
static const UInt32 kHUDReticleReturn2 = 0x00673440;

static bool FirstPersonView = 0;
static bool BowAction = 0;
static UInt8 HUDReticle = 0;
static NiNode* DialogActorNode = NULL;

class CameraMode {

public:
	float TrackToggleCamera(UInt8 FirstPersonFlag);
	int TrackProcessAction(float Arg1, float Arg2);
	void TrackSetDialogCamera(Actor* Act, float Arg2, float Arg3);

};

float (__thiscall CameraMode::* ToggleCamera)(UInt8);
float (__thiscall CameraMode::* TrackToggleCamera)(UInt8);
float CameraMode::TrackToggleCamera(UInt8 FirstPersonFlag) { //ecx is PlayerCharacter
	
	TheRenderManager->FirstPersonView = FirstPersonView = FirstPersonFlag;
	return (this->*ToggleCamera)(0);
	
}

int (__thiscall CameraMode::* ProcessAction)(float, float);
int (__thiscall CameraMode::* TrackProcessAction)(float, float);
int CameraMode::TrackProcessAction(float Arg1, float Arg2) {
	
	Actor* Act = (Actor*)this;
	
	if (Act == *g_thePlayer && FirstPersonView) {
		SInt16 CurrentAction = ((HighProcess*)Act->process)->currentAction;
		if (CurrentAction == HighProcess::kAction_AttackBow && !BowAction) {
			BowAction = 1;
			(this->*ToggleCamera)(1);
		}
		else if (CurrentAction == HighProcess::kAction_AttackFollowThrough && BowAction) {
			BowAction = 0;
			(this->*ToggleCamera)(0);
		}
	}
	return (this->*ProcessAction)(Arg1, Arg2);

}

void (__thiscall CameraMode::* SetDialogCamera)(Actor*, float, float);
void (__thiscall CameraMode::* TrackSetDialogCamera)(Actor*, float, float);
void CameraMode::TrackSetDialogCamera(Actor* Act, float Arg2, float Arg3) { //ecx is PlayerCharacter

	if ((FirstPersonView && TheSettingManager->SettingsMain.CameraModeDialogFirst) || (!FirstPersonView && TheSettingManager->SettingsMain.CameraModeDialogThird))
		DialogActorNode = Act->niNode;
	else
		(this->*SetDialogCamera)(Act, Arg2, Arg3);

}

NiPoint3* (__fastcall * SetCameraPosition)(PlayerCharacter* Player, int Arg2, NiPoint3* CameraPos, NiPoint3* PlayerPos, UInt8 Flag) = (NiPoint3* (__fastcall *)(PlayerCharacter* Player, int Arg2, NiPoint3* CameraPos, NiPoint3* PlayerPos, UInt8 Flag))0x0065F080;
NiPoint3* __fastcall TrackSetCameraPosition(PlayerCharacter* Player, int Arg2, NiPoint3* CameraPos, NiPoint3* PlayerPos, UInt8 Flag) {
	
	UInt8 CameraChasing;

	if (FirstPersonView) {
		Player->unk589 = 1;
		CameraChasing = !TheSettingManager->SettingsMain.CameraModeChasingFirst;
	}
	else
		CameraChasing = !TheSettingManager->SettingsMain.CameraModeChasingThird;
	return SetCameraPosition(Player, Arg2, CameraPos, PlayerPos, CameraChasing);

}

void UpdateCamera(NiAVObject* CameraNode, NiPoint3* LocalTranslate)
{

	SettingsCameraStruct* SettingsCamera = &TheSettingManager->SettingsCamera;
	NiPoint3* CameraPosition = &CameraNode->m_localTransform.pos;

	if ((TheUtilityManager->IsMenuMode(1009) || TheUtilityManager->IsMenuMode(1034)) && ((FirstPersonView && TheSettingManager->SettingsMain.CameraModeDialogFirst == 2) || (!FirstPersonView && TheSettingManager->SettingsMain.CameraModeDialogThird == 2))) {
		NiPoint3 v;
		NiMatrix33 mw, ml;
		float x, y, z, r;
		float ax, ay, az;
		NiPoint3 Pos = {0.0f, 0.0f, 0.0f};
		NiNode* ActorNode = (*g_thePlayer)->niNode;
		NiPoint3* HeadPosition = &ActorNode->GetObjectByName("Bip01 Head")->m_worldTransform.pos;
		TheUtilityManager->MatrixVectorMultiply(&v, &ActorNode->m_worldTransform.rot, &TheSettingManager->SettingsMain.CameraModeDialogOffset);
		CameraPosition->x = HeadPosition->x + v.x;
		CameraPosition->y = HeadPosition->y + v.y;
		CameraPosition->z = HeadPosition->z + v.z;
		HeadPosition = &DialogActorNode->GetObjectByName("Bip01 Head")->m_worldTransform.pos;
		TheUtilityManager->MatrixVectorMultiply(&v, &DialogActorNode->m_worldTransform.rot, &Pos);
		x = CameraPosition->x - (HeadPosition->x + v.x);
		y = CameraPosition->y - (HeadPosition->y + v.y);
		z = CameraPosition->z - (HeadPosition->z + v.z);
		r = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		ax = (acos(z / r) * 180 / M_PI) - 90;
		ay = 0;
		az = (atan2(y, x) * 180 / M_PI) + 90;
		TheUtilityManager->GenerateRotationMatrixZXY(&mw, 0, 0, 0, 1);
		TheUtilityManager->GenerateRotationMatrixZXY(&ml, az, ax, ay, 1);
		CameraNode->m_worldTransform.rot = mw;
		CameraNode->m_localTransform.rot = ml;
	}
	else if (!TheUtilityManager->IsVanityView()) {
		if (FirstPersonView) {
			HighProcess* Proc = (HighProcess*)(*g_thePlayer)->process;
			if (!Proc->KnockedState && Proc->SitSleepState != 3 && Proc->SitSleepState != 5 && Proc->SitSleepState != 8 && Proc->SitSleepState != 9 && Proc->SitSleepState != 10) {
				NiNode* ActorNode = (*g_thePlayer)->niNode;
				NiPoint3* HeadPosition = &ActorNode->GetObjectByName("Bip01 Head")->m_worldTransform.pos;
				NiPoint3 v;
				TheUtilityManager->MatrixVectorMultiply(&v, &ActorNode->m_worldTransform.rot, &TheSettingManager->SettingsMain.CameraModeOffset);
				CameraPosition->x = HeadPosition->x + v.x;
				CameraPosition->y = HeadPosition->y + v.y;
				CameraPosition->z = HeadPosition->z + v.z;
			}
		}
		else if (SettingsCamera->Move || SettingsCamera->Rotate || SettingsCamera->Look || SettingsCamera->MoveTo || SettingsCamera->RotateTo || SettingsCamera->LookTo) {
			NiNode* RefNode = NULL;
			NiMatrix33* CameraRotation = &CameraNode->m_localTransform.rot;
			if (SettingsCamera->Ref) RefNode = SettingsCamera->Ref->niNode;
			if (SettingsCamera->Move) {
				NiPoint3* HeadPosition = &RefNode->GetObjectByName("Bip01 Head")->m_worldTransform.pos;
				NiPoint3 v;
				TheUtilityManager->MatrixVectorMultiply(&v, &RefNode->m_worldTransform.rot, &SettingsCamera->Pos);
				CameraPosition->x = HeadPosition->x + v.x;
				CameraPosition->y = HeadPosition->y + v.y;
				CameraPosition->z = HeadPosition->z + v.z;
			}
			if (SettingsCamera->Rotate) {
				NiMatrix33 m;
				TheUtilityManager->GenerateRotationMatrixZXY(&m, SettingsCamera->AngZ, SettingsCamera->AngX, SettingsCamera->AngY, 1);
				TheUtilityManager->MatrixMultiply(CameraRotation, &RefNode->m_worldTransform.rot, &m);
			}
			if (SettingsCamera->MoveTo) {
				CameraPosition->x = SettingsCamera->Pos.x;
				CameraPosition->y = SettingsCamera->Pos.y;
				CameraPosition->z = SettingsCamera->Pos.z;
				if (!SettingsCamera->RotateTo && !SettingsCamera->Look && !SettingsCamera->LookTo) {
					NiMatrix33 m;
					TheUtilityManager->GenerateRotationMatrixZXY(&m, 0, 0, 0, 1);
					CameraNode->m_worldTransform.rot = m;
					CameraNode->m_localTransform.rot = m;
				}
			}
			if (SettingsCamera->RotateTo) {
				NiMatrix33 mw, ml;
				TheUtilityManager->GenerateRotationMatrixZXY(&mw, 0, 0, 0, 1);
				TheUtilityManager->GenerateRotationMatrixZXY(&ml, SettingsCamera->AngZ, SettingsCamera->AngX, SettingsCamera->AngY, 1);
				CameraNode->m_worldTransform.rot = mw;
				CameraNode->m_localTransform.rot = ml;
			}
			if (SettingsCamera->Look || SettingsCamera->LookTo) {
				NiMatrix33 mw, ml;
				float x, y, z, r;
				float ax, ay, az;
				x = SettingsCamera->LookPos.x;
				y = SettingsCamera->LookPos.y;
				z = SettingsCamera->LookPos.z;
				if (SettingsCamera->Look) {
					NiPoint3* HeadPosition = &RefNode->GetObjectByName("Bip01 Head")->m_worldTransform.pos;
					NiPoint3 v;
					TheUtilityManager->MatrixVectorMultiply(&v, &RefNode->m_worldTransform.rot, &SettingsCamera->Pos);
					x = HeadPosition->x + v.x;
					y = HeadPosition->y + v.y;
					z = HeadPosition->z + v.z;
				}
				x = CameraPosition->x - x;
				y = CameraPosition->y - y;
				z = CameraPosition->z - z;
				r = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
				ax = (acos(z / r) * 180 / M_PI) - 90;
				ay = 0;
				az = (atan2(y, x) * 180 / M_PI) + 90;
				TheUtilityManager->GenerateRotationMatrixZXY(&mw, 0, 0, 0, 1);
				TheUtilityManager->GenerateRotationMatrixZXY(&ml, az, ax, ay, 1);
				CameraNode->m_worldTransform.rot = mw;
				CameraNode->m_localTransform.rot = ml;
			}
		}
		LocalTranslate->x = SettingsCamera->CameraPos.x = CameraNode->m_localTransform.pos.x;
		LocalTranslate->y = SettingsCamera->CameraPos.y = CameraNode->m_localTransform.pos.y;
		LocalTranslate->z = SettingsCamera->CameraPos.z = CameraNode->m_localTransform.pos.z;
	}

}

static __declspec(naked) void UpdateCameraHook()
{

	__asm
	{
		pushad
		lea		ecx, [esp+0x58]
		push	ecx
		push	eax
		call	UpdateCamera
		pop		eax
		pop		ecx
		popad
		jmp		kUpdateCameraReturn
	}

}

static __declspec(naked) void ThirdPersonCameraHook()
{

	__asm
	{
		cmp		FirstPersonView, 1
		jmp		kThirdPersonCameraReturn
	}

}

static __declspec(naked) void OnCameraPOVHook()
{

	__asm
	{
		cmp		FirstPersonView, 0
		jz		loc_firstperson
		push	0
		jmp		loc_togglecamera

	loc_firstperson:
		push	1

	loc_togglecamera:
		mov		ecx, ebx
		mov		eax, 0x0066C580
		call	eax
		jmp		kOnCameraPOVReturn
	}

}

static __declspec(naked) void OnDialogHook()
{

	__asm
	{
		mov		cl, FirstPersonView
		xor     cl, 1
		jmp		kOnDialogReturn
	}

}

static __declspec(naked) void OnExitingDialogHook()
{

	__asm
	{
		cmp		byte ptr [esi+0x07C], 0
		jz		loc_firstperson
		push	0
		jmp		loc_togglecamera

	loc_firstperson:
		push	1

	loc_togglecamera:
		mov		eax, 0x0066C580
		call	eax
		jmp		kOnExitingDialogReturn
	}

}

static __declspec(naked) void HUDReticleHook()
{

	__asm
	{
		cmp		FirstPersonView, 1
		jnz		short loc_return2
		cmp		HUDReticle, 0
		jz		short loc_return2
		cmp		HUDReticle, 1
		jnz		short loc_check
		cmp		BowAction, 0
		jz		short loc_return2
		jmp		short loc_return1

	loc_check:
		cmp		HUDReticle, 2
		jnz		short loc_return2

	loc_return1:
		jmp		kHUDReticleReturn1
	
	loc_return2:
		jmp		kHUDReticleReturn2
	}

}

void CreateCameraModeHook()
{

	*((int *)&ToggleCamera)		= 0x0066C580;
	TrackToggleCamera			= &CameraMode::TrackToggleCamera;
	*((int *)&ProcessAction)	= 0x005FCAB0;
	TrackProcessAction			= &CameraMode::TrackProcessAction;
	*((int *)&SetDialogCamera)	= 0x0066C6F0;
	TrackSetDialogCamera		= &CameraMode::TrackSetDialogCamera;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)ToggleCamera,		*((PVOID *)&TrackToggleCamera));
	DetourAttach(&(PVOID&)ProcessAction,	*((PVOID *)&TrackProcessAction));
	DetourAttach(&(PVOID&)SetDialogCamera,	*((PVOID *)&TrackSetDialogCamera));
	DetourAttach(&(PVOID&)SetCameraPosition,		   &TrackSetCameraPosition);
	DetourTransactionCommit();

	WriteRelJump(kOnCameraPOVHook,			(UInt32)OnCameraPOVHook);
	WriteRelJump(kUpdateCameraHook,			(UInt32)UpdateCameraHook);
	WriteRelJump(kThirdPersonCameraHook,	(UInt32)ThirdPersonCameraHook);
	WriteRelJump(kOnDialogHook,				(UInt32)OnDialogHook);
	WriteRelJump(kOnExitingDialogHook,		(UInt32)OnExitingDialogHook);
	WriteRelJump(kHUDReticleHook,			(UInt32)HUDReticleHook);
	WriteRelJump(0x0066B769, 0x0066B795); // Does not lower the player Z axis value (fixes the bug of the camera on feet after resurrection)
	WriteRelJump(0x006738B1, 0x00673935); // Cancels the fPlayerDeathReloadTime

	HUDReticle = TheSettingManager->SettingsMain.CameraModeHUDReticle;

}
#elif defined (SKYRIM)
void CreateCameraModeHook()
{
	
}
#endif