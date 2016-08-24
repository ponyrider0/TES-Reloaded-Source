#pragma once

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
	
	__forceinline float ThisStdCallF(UInt32 _f, void* _t) {
		float r = 0.0f;
		__asm
		{
			mov ecx, _t
			call _f
			fstp r
		}
		return r;
	}

	float	GetGameTime();
	bool	IsMenuMode(int MenuType);
	bool	IsVanityView();
	UInt8	GetSitSleepState(Actor* Act);
	float	GetWaterHeight(TESObjectREFR* Ref);
	bool	ShowMessageBoxQuestion(const char* Message, void* Callback);
	void	ShowMessage(const char* Message);
	void	MatrixMultiply(NiMatrix33* s, NiMatrix33* m1, NiMatrix33* m2);
	void	MatrixVectorMultiply(NiPoint3* s, NiMatrix33* m, NiPoint3* v);
	void	GenerateRotationMatrixZXY(NiMatrix33* m, float x, float y, float z, bool degrees);
	void	GenerateRotationMatrixXYZ(NiMatrix33* m, float x, float y, float z, bool degrees);
	float	RadiansToDegrees(float radians);
	float	DegreesToRadians(float degrees);
	void	NormalizeVector3(D3DXVECTOR4& vector);

private:
	class NiMatrix33L { public: float data[9]; };

};