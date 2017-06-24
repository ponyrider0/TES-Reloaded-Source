#pragma once
#if defined(OBLIVION)
#include "obse\NiExtraData.h"
#elif defined(SKYRIM)
#include "skse\NiNodes.h"
#endif
static const float*		WaterBlend						= (float*)0x00B45E50; // Only Oblivion
static float*			LocalGrassWindMagnitudeMax		= (float*)0x00B46068; // Only Oblivion
static float*			LocalGrassWindMagnitudeMin		= (float*)0x00B46064; // Only Oblivion
static UInt8*			LocalWaterHiRes					= (UInt8*)0x00B45FD0; // Only Oblivion

class UtilityManager
{

public:
	UtilityManager();

	float								GetGameTime();
	bool								IsMenu(int MenuType);
	bool								IsConsole();
	bool								IsVanityView();
	UInt8								GetSitSleepState(Actor* Act);
	float								GetWaterHeight(TESForm* Ref);
	TESWaterForm*						GetWaterForm(TESObjectCELL* Cell);
	float								GetFarPlaneDistance(SceneGraph* WorldSceneGraph);
	bool								ShowMessageBoxQuestion(const char* Message, void* Callback);
	void								ShowMessage(const char* Message);
	void								RestoreCamera();
	void								SetCameraFOV(SceneGraph* WorldSceneGraph, float FoV);
	void								MatrixMultiply(NiMatrix33* s, NiMatrix33* m1, NiMatrix33* m2);
	void								MatrixVectorMultiply(NiPoint3* s, NiMatrix33* m, NiPoint3* v);
	void								GenerateRotationMatrixZXY(NiMatrix33* m, NiPoint3* v, bool degrees);
	void								GenerateRotationMatrixXYZ(NiMatrix33* m, NiPoint3* v, bool degrees);
	float								RadiansToDegrees(float radians);
	float								DegreesToRadians(float degrees);
	void								NormalizeVector3(D3DXVECTOR4* vector);
#if defined(OBLIVION)
	bool								IsMenu();
	void								Play(TESSound* Sound);
	bool								IsFemale(Actor* Act);
	ExtraContainerChanges::EntryData*	GetInventoryItem(TESObjectREFR* Ref, UInt32 Index);
	void								UpdateInventory();
	ExtraContainerChanges::Data*		GetExtraContainerChangesData(ExtraDataList* DataList);
	float								GetExtraHealthValue(ExtraDataList* DataList);
	void								SetExtraHealthValue(ExtraDataList* DataList, float Value);
	void								QueueIdle(ActorAnimData* AnimData, TESIdleForm* Idle, Actor* Act, UInt32 Flags);
	TESObjectExtraData*					GetRef(NiNode* ActorNode);
	void								RemoveModel(char* WeaponModelPath);
	void								PurgeCellBuffers();
	UInt8								GetAnimSequence(NiTPointerMap<AnimSequenceBase>* AnimsMap, UInt16 AnimGroupKey, AnimSequenceBase** AnimSequence);
#elif defined(SKYRIM)
	void								SetupCamera(NiCamera* Camera);
	void								QueueNiNodeUpdate(Actor* Act);
	void								UpdateOverShoulder(PlayerCamera* Camera, bool IsWeaponOut);
	bool								IsCamSwitchControlEnabled(PlayerControls* Controls);
	void								SetCameraState(TESCameraState* State);
	BSFixedString*						CreateBSString(BSFixedString* FixedString, char* Value);
	void								DisposeBSString(BSFixedString* FixedString);
#endif

private:
	class NiMatrix33L { public: float data[9]; };

	template <typename T1, typename T2, typename T3, typename T4> __forceinline UInt32 ThisCall(UInt32 Method, void* Instance, T1 Arg1, T2 Arg2, T3 Arg3, T4 Arg4) { class T {}; union { UInt32 x; UInt32(T::*m)(T1, T2, T3, T4); } u = { Method }; return ((T*)Instance->*u.m)(Arg1, Arg2, Arg3, Arg4); }
	template <typename T1, typename T2, typename T3> __forceinline UInt32 ThisCall(UInt32 Method, void* Instance, T1 Arg1, T2 Arg2, T3 Arg3) { class T {}; union { UInt32 x; UInt32(T::*m)(T1, T2, T3); } u = { Method }; return ((T*)Instance->*u.m)(Arg1, Arg2, Arg3); }
	template <typename T1, typename T2> __forceinline UInt32 ThisCall(UInt32 Method, void* Instance, T1 Arg1, T2 Arg2) { class T {}; union { UInt32 x; UInt32(T::*m)(T1, T2); } u = { Method }; return ((T*)Instance->*u.m)(Arg1, Arg2); }
	template <typename T1> __forceinline UInt32 ThisCall(UInt32 Method, void* Instance, T1 Arg1) { class T {}; union { UInt32 x; UInt32(T::*m)(T1); } u = { Method }; return ((T*)Instance->*u.m)(Arg1); }
	__forceinline UInt32 ThisCall(UInt32 Method, void* Instance) { class T {}; union { UInt32 x; UInt32(T::*m)(); } u = { Method }; return ((T*)Instance->*u.m)(); }
	
	__forceinline float ThisCallF(UInt32 Method, void* Instance) { class T {}; union { UInt32 x; float(T::*m)(); } u = { Method }; return ((T*)Instance->*u.m)(); }

};