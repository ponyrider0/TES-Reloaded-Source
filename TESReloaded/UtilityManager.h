#pragma once
#if defined(SKYRIM)
#include "skse\ninodes.h"
#endif
static const float*		WaterBlend						= (float*)0x00B45E50; // Only Oblivion
static float*			LocalGrassWindMagnitudeMax		= (float*)0x00B46068; // Only Oblivion
static float*			LocalGrassWindMagnitudeMin		= (float*)0x00B46064; // Only Oblivion
static UInt8*			LocalWaterHiRes					= (UInt8*)0x00B45FD0; // Only Oblivion

class UtilityManager
{

public:
	UtilityManager();

	template <typename T1, typename T2, typename T3, typename T4> __forceinline UInt32 ThisStdCall(UInt32 _f, void* _t, T1 a1, T2 a2, T3 a3, T4 a4) { class T {}; union { UInt32 x; UInt32(T::*m)(T1, T2, T3, T4); } u = { _f }; return ((T*)_t->*u.m)(a1, a2, a3, a4); }
	template <typename T1, typename T2, typename T3> __forceinline UInt32 ThisStdCall(UInt32 _f, void* _t, T1 a1, T2 a2, T3 a3) { class T {}; union { UInt32 x; UInt32(T::*m)(T1, T2, T3); } u = { _f }; return ((T*)_t->*u.m)(a1, a2, a3); }
	template <typename T1, typename T2> __forceinline UInt32 ThisStdCall(UInt32 _f, void* _t, T1 a1, T2 a2) { class T {}; union { UInt32 x; UInt32(T::*m)(T1, T2); } u = { _f }; return ((T*)_t->*u.m)(a1, a2); }
	template <typename T1> __forceinline UInt32 ThisStdCall(UInt32 _f, void* _t, T1 a1) { class T {}; union { UInt32 x; UInt32(T::*m)(T1); } u = { _f }; return ((T*)_t->*u.m)(a1); }
	__forceinline UInt32 ThisStdCall(UInt32 _f, void* _t) { class T {}; union { UInt32 x; UInt32(T::*m)(); } u = { _f }; return ((T*)_t->*u.m)(); }
	

	float								GetGameTime();
	bool								IsMenu(int MenuType);
	bool								IsMenu();
	bool								IsConsole();
	bool								IsVanityView();
	UInt8								GetSitSleepState(Actor* Act);
	float								GetWaterHeight(TESForm* Ref);
	TESWaterForm*						GetWaterForm(TESObjectCELL* Cell);
	float								GetFarPlaneDistance(SceneGraph* WorldSceneGraph);
	void								Play(TESSound* Sound);
	bool								IsFemale(Actor* Act);
#if defined(OBLIVION)
	int									GetInventoryCount(TESObjectREFR* Ref);
	ExtraContainerChanges::EntryData*	GetInventoryItem(TESObjectREFR* Ref, UInt32 Index);
	float								GetExtraHealthValue(ExtraDataList* DataList);
	void								SetExtraHealthValue(ExtraDataList* DataList, float Value);
	void								QueueIdle(ActorAnimData* AnimData, TESIdleForm* Idle, Actor* Act, UInt32 Flags);
#endif
	void								PurgeCellBuffers();
	bool								ShowMessageBoxQuestion(const char* Message, void* Callback);
	void								ShowMessage(const char* Message);
	void								MatrixMultiply(NiMatrix33* s, NiMatrix33* m1, NiMatrix33* m2);
	void								MatrixVectorMultiply(NiPoint3* s, NiMatrix33* m, NiPoint3* v);
	void								GenerateRotationMatrixZXY(NiMatrix33* m, NiPoint3* v, bool degrees);
	void								GenerateRotationMatrixXYZ(NiMatrix33* m, NiPoint3* v, bool degrees);
	float								RadiansToDegrees(float radians);
	float								DegreesToRadians(float degrees);
	void								NormalizeVector3(D3DXVECTOR4* vector);

private:
	class NiMatrix33L { public: float data[9]; };

	__forceinline float ThisCallF(UInt32 Method, void* Instance) {
		float Result = 0.0f;
		__asm
		{
			mov		ecx, Instance
			call	Method
			fstp	Result
		}
		return Result;
	}

};