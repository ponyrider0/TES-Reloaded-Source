#pragma once

#if defined(OBLIVION)
#define kWorldSceneGraph 0x00B333CC
#define kSky 0x00B365C4
#define kSettingFullScreen 0x00B06C74
#elif defined(SKYRIM)
#define kWorldSceneGraph 0x01B2E8BC
#define kSky 0x01B1160C
#define kSettingFullScreen 0x012CF5EC
#endif

static const UInt32*	SettingGridsToLoad				= (UInt32*)0x00B06A2C; // Only Oblivion
static const UInt32*	SettingGridDistantCount			= (UInt32*)0x00B06A90; // Only Oblivion
static float*			SettingWorldFoV					= (float*)0x012C8D78; // Only Skyrim
static float*			Setting1stPersonFoV				= (float*)0x012C8D84; // Only Skyrim
static UInt8*			SettingFullScreen				= (UInt8*)kSettingFullScreen;
static float*			SettingNearDistance				= (float*)0x00B03134; // Only Oblivion
static float*			SettingLODFadeOutMultActors		= (float*)0x00B0762C; // Only Oblivion
static float*			SettingLODFadeOutMultItems		= (float*)0x00B07624; // Only Oblivion
static float*			SettingLODFadeOutMultObjects	= (float*)0x00B0761C; // Only Oblivion
static UInt32*			SettingMinGrassSize				= (UInt32*)0x00B09B20; // Only Oblivion
static float*			SettingGrassEndDistance			= (float*)0x00B09B18; // Only Oblivion
static float*			SettingGrassStartFadeDistance	= (float*)0x00B09B10; // Only Oblivion
static float*			SettingGrassWindMagnitudeMax	= (float*)0x00B09B30; // Only Oblivion
static float*			SettingGrassWindMagnitudeMin	= (float*)0x00B09B28; // Only Oblivion
static float*			SettingTexturePctThreshold		= (float*)0x00B08B6C; // Only Oblivion
static float*			SettingDecalLifetime			= (float*)0x00B097C8; // Only Oblivion


static const float*		WaterBlend					= (float*)0x00B45E50; // Only Oblivion
static float*			LocalGrassWindMagnitudeMax  = (float*)0x00B46068; // Only Oblivion
static float*			LocalGrassWindMagnitudeMin  = (float*)0x00B46064; // Only Oblivion
static UInt8*			LocalWaterHiRes				= (UInt8*)0x00B45FD0; // Only Oblivion

class UtilityManager
{

public:
	UtilityManager();

	template <typename T1, typename T2, typename T3, typename T4> __forceinline UInt32 ThisStdCall(UInt32 _f, void* _t, T1 a1, T2 a2, T3 a3, T4 a4) { class T {}; union { UInt32 x; UInt32(T::*m)(T1, T2, T3, T4); } u = { _f }; return ((T*)_t->*u.m)(a1, a2, a3, a4); }
	template <typename T1, typename T2, typename T3> __forceinline UInt32 ThisStdCall(UInt32 _f, void* _t, T1 a1, T2 a2, T3 a3) { class T {}; union { UInt32 x; UInt32(T::*m)(T1, T2, T3); } u = { _f }; return ((T*)_t->*u.m)(a1, a2, a3); }
	template <typename T1, typename T2> __forceinline UInt32 ThisStdCall(UInt32 _f, void* _t, T1 a1, T2 a2) { class T {}; union { UInt32 x; UInt32(T::*m)(T1, T2); } u = { _f }; return ((T*)_t->*u.m)(a1, a2); }
	template <typename T1> __forceinline UInt32 ThisStdCall(UInt32 _f, void* _t, T1 a1) { class T {}; union { UInt32 x; UInt32(T::*m)(T1); } u = { _f }; return ((T*)_t->*u.m)(a1); }
	__forceinline UInt32 ThisStdCall(UInt32 _f, void* _t) { class T {}; union { UInt32 x; UInt32(T::*m)(); } u = { _f }; return ((T*)_t->*u.m)(); }

	float	GetGameTime();
	bool	IsMenuMode(int MenuType);
	bool	IsVanityView();
	void	MatrixMultiply(NiMatrix33* s, NiMatrix33* m1, NiMatrix33* m2);
	void	MatrixVectorMultiply(NiPoint3* s, NiMatrix33* m, NiPoint3* v);
	void	GenerateRotationMatrixZXY(NiMatrix33* m, float x, float y, float z, bool degrees);
	void	GenerateRotationMatrixXYZ(NiMatrix33* m, float x, float y, float z, bool degrees);
	float	RadiansToDegrees(float radians);
	float	DegreesToRadians(float degrees);
	void	NormalizeVector3(D3DXVECTOR4& vector);

	bool	Screenshot;

private:
	class NiMatrix33L { public: float data[9]; };

};