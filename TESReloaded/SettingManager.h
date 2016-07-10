#pragma once
#include <map>

#if defined(OBLIVION)
#include "obse\PluginAPI.h"
#define ScreenshotFilenamePrefix "Oblivion"
#define SettingsPath "\\Data\\Shaders\\OblivionReloaded\\"
#define MainSettingsFile "\\Data\\OBSE\\Plugins\\OblivionReloaded.ini"
#define ShadersPath "Data\\Shaders\\OblivionReloaded\\"
#define EffectsPath "Data\\Shaders\\OblivionReloaded\\"
#define ShieldAnimFile "Characters\\_Male\\IdleAnims\\shieldout.kf"
#define ShieldAnimFullFile "Meshes\\Characters\\_Male\\IdleAnims\\shieldout.kf"
#define WaterDisplacementMapSize 256.0f
#define WaterHeightMapSize 256.0f
#define MultipleElement OBSEArrayElement
#elif defined(SKYRIM)
#define ScreenshotFilenamePrefix "Skyrim"
#define SettingsPath "\\Data\\Shadersfx\\SkyrimReloaded\\"
#define MainSettingsFile "\\Data\\SKSE\\Plugins\\SkyrimReloaded.ini"
#define ShadersPath "Data\\Shadersfx\\SkyrimReloaded\\"
#define EffectsPath "Data\\Shadersfx\\SkyrimReloaded\\"
#define ShieldAnimFile "Characters\\_Male\\IdleAnims\\shieldout.kf"
#define ShieldAnimFullFile "Meshes\\Characters\\_Male\\IdleAnims\\shieldout.kf"
#define WaterDisplacementMapSize 256.0f
#define WaterHeightMapSize 256.0f
struct MultipleElement
{

public:
	enum
	{
		kType_Invalid,
		kType_Numeric,
		kType_String,
	};
	union
	{
		float		flo;
		char*		str;
	};
	UInt8			type;

	~MultipleElement() { Reset(); }
	MultipleElement() : type(kType_Invalid) { }
	MultipleElement(float _flo) : flo(_flo), type(kType_Numeric) { }
	MultipleElement(const char* _str) : type(kType_String) { CopyString(_str); }
	MultipleElement(const MultipleElement& data) { if (data.type == kType_String) CopyString(data.str); else flo = data.flo; type = data.type; }
	MultipleElement& operator=(const MultipleElement& data) { if (this != &data) { Reset(); if (data.type == kType_String) CopyString(data.str); else flo = data.flo; type = data.type; } return *this; }

private:
	void Reset() { if (type == kType_String) { FormHeap_Free(str); type = kType_Invalid; str = NULL; } }
	void CopyString(const char* _str) {
		UInt32 size = strlen(_str) + 1;
		str = (char*)FormHeap_Allocate(size);
		memcpy(str, _str, size);
	}
};
#endif

struct SettingsMainStruct
{
	char ScreenshotPath[MAX_PATH];
	char SleepingModeRestMessage[80];
	char MainSettingsFullFile[MAX_PATH];
	bool WaterManagement;
	bool WindowedMode;
	bool CustomEffects;
	bool FrameRate;
	bool SaveSettings;
	bool CombatMode;
	bool CameraMode;
	bool EquipmentMode;
	bool SleepingMode;
	bool GrassMode;
	UInt8 CameraModeHUDReticle;
	bool CameraModeChasingFirst;
	bool CameraModeChasingThird;
	UInt8 CameraModeDialogFirst;
	UInt8 CameraModeDialogThird;
	bool SleepingModeRest;
	bool EnableWater;
	bool EnableGrass;
	bool EnablePrecipitations;
	bool EnableHDR;
	bool EnablePOM;
	bool EnableSkin;
	bool EnableTerrain;
	bool EnableBlood;
	bool EnableShadows;
	bool EnableUnderwater;
	bool EnableWaterLens;
	bool EnableGodRays;
	bool EnableDepthOfField;
	bool EnableAmbientOcclusion;
	bool EnableColoring;
	bool EnableCinema;
	bool EnableBloom;
	bool EnableSnowAccumulation;
	bool EnableBloodLens;
	bool EnableMotionBlur;
	bool EnableLowHF;
	bool EnableWetWorld;
	bool EnableSharpening;
	bool EnableSMAA;
	bool DevelopCompileShaders;
	bool DevelopCompileEffects;
	bool DevelopShadersFolder;
	bool DevelopTraceShaders;
	bool DevelopTraceVanillaShaders;
	bool DevelopTraceCulling;
	UInt8 ScreenshotType;
	int WaterReflectionMapSize;
	int FrameRateAverage;
	int FrameRateGap;
	int FrameRateDelay;
	int FrameRateGridStep;
	int FrameRateFadeMinObjects;
	int FrameRateFadeMinActors;
	int FrameRateGridMin;
	float FrameRateFadeStep;
	float FarPlaneDistance;
	float FoV;
	NiPoint3 CameraModeOffset;
	float CameraModeNearDistanceFirst;
	float CameraModeNearDistanceThird;
	NiPoint3 CameraModeDialogOffset;
};

struct SettingsWaterStruct
{
	bool IceEnabled;
	float choppiness;
	float waveWidth;
	float waveSpeed;
	float reflectivity;
	float causticsStrength;
	float causticsStrengthS;
	float shoreFactor;
	float turbidity;
	float inScattCoeff;
	float inExtCoeff_R;
	float inExtCoeff_G;
	float inExtCoeff_B;
	float depthDarkness;
	float IceinExtCoeff_R;
	float IceinExtCoeff_G;
	float IceinExtCoeff_B;
	float Icechoppiness;
	float IcewaveWidth;
	float Icereflectivity;
	float IceshoreFactor;
	float Iceturbidity;
	float LODdistance;
	float MinLOD;
	float LensTimeMultA;
	float LensTimeMultB;
	float LensTime;
	float LensAmount;
	float LensViscosity;
};

struct SettingsGrassStruct
{
	bool WindEnabled;
	int GrassDensity;
	float WindCoefficient;
	float ScaleX;
	float ScaleY;
	float ScaleZ;
	float MinHeight;
	float MinDistance;
	float MaxDistance;
};

struct SettingsHDRStruct
{
	float ToneMapping;
	float ToneMappingBlur;
	float ToneMappingColor;
};

struct SettingsPOMStruct
{
	float HeightMapScale;
	float ShadowSoftening;
	float MinSamples;
	float MaxSamples;
};

struct SettingsTerrainStruct
{
	float DistantSpecular;
	float DistantNoise;
	float NearSpecular;
	float MiddleSpecular;
};

struct SettingsSkinStruct
{
	float Attenuation;
	float SpecularPower;
	float MaterialThickness;
	float RimScalar;
	float ExtCoeffRed;
	float ExtCoeffGreen;
	float ExtCoeffBlue;
};

struct SettingsGodRaysStruct
{
	bool SunGlareEnabled;
	int LightShaftPasses;
	float RayIntensity;
	float RayLength;
	float RayDensity;
	float RayVisibility;
	float Luminance;
	float GlobalMultiplier;
	float RayR;
	float RayG;
	float RayB;
	float Saturate;
};

struct SettingsDepthOfFieldStruct
{
	bool Enabled;
	bool DialogMode;
	bool DistantBlur;
	float DistantBlurStartRange;
	float DistantBlurEndRange;
	float BaseBlurRadius;
	float BlurFallOff;
	float Radius;
	float DiameterRange;
	float NearBlurCutOff;
};

struct SettingsAmbientOcclusionStruct
{
	bool Enabled;
	float RadiusMultiplier;
	float StrengthMultiplier;
	float ClampStrength;
	float AngleBias;
	float Range;
	float LumThreshold;
	float BlurDropThreshold;
	float BlurRadiusMultiplier;
};

struct SettingsColoringStruct
{
	float Strength;
	float BaseGamma;
	float Fade;
	float Contrast;
	float Saturation;
	float Bleach;
	float BleachLuma;
	float ColorCurve;
	float ColorCurveR;
	float ColorCurveG;
	float ColorCurveB;
	float EffectGamma;
	float EffectGammaR;
	float EffectGammaG;
	float EffectGammaB;
	float Linearization;
};

struct SettingsCinemaStruct
{
	bool DialogMode;
	float AspectRatio;
	float VignetteRadius;
	float VignetteDarkness;
};

struct SettingsBloomStruct
{
	float BloomIntensity;
	float OriginalIntensity;
	float BloomSaturation;
	float OriginalSaturation;
	float Luminance;
	float MiddleGray;
	float WhiteCutOff;
};

struct SettingsPrecipitationsStruct
{
	float Intensity;
	float Velocity;
	float Size;
	float SnowAmount;
	float SnowIncrease;
	float SnowDecrease;
	float SnowSunPower;
	float SnowBlurNormDropThreshhold;
	float SnowBlurRadiusMultiplier;
	float RainAmount;
	float RainIncrease;
	float RainDecrease;
	float PuddleCoeff_R;
	float PuddleCoeff_G;
	float PuddleCoeff_B;
	float PuddleSpecularMultiplier;
};

struct SettingsBloodStruct
{
	float LensChance;
	float LensColorR;
	float LensColorG;
	float LensColorB;
	float LensIntensity;
	float LensTime;
};

struct SettingsMotionBlurStruct
{
	bool Enabled;
	float GaussianWeight;
	float BlurScale;
	float BlurOffsetMax;
	float BlurCutOff;
};

struct SettingsLowHFStruct
{
	float HealthLimit;
	float FatigueLimit;
	float LumaMultiplier;
	float BlurMultiplier;
	float VignetteMultiplier;
	float DarknessMultiplier;
};

struct SettingsSharpeningStruct
{
	float Strength;
	float Clamp;
	float Offset;
};

struct SettingsShadowsStruct
{
	float ShadowIntensity;
	float SelfShadowIntensity;
};

struct SettingsCameraStruct
{
	bool Move;
	bool Rotate;
	bool Look;
	bool MoveTo;
	bool RotateTo;
	bool LookTo;
	NiPoint3 Pos;
	NiPoint3 LookPos;
	NiPoint3 CameraPos;
	float AngX;
	float AngY;
	float AngZ;
	TESObjectREFR* Ref;
};

typedef std::map<std::string, SettingsWaterStruct> SettingsWaterList;
typedef std::map<std::string, SettingsDepthOfFieldStruct> SettingsDepthOfFieldList;
typedef std::map<std::string, SettingsAmbientOcclusionStruct> SettingsAmbientOcclusionList;
typedef std::map<std::string, SettingsColoringStruct> SettingsColoringList;
typedef std::map<std::string, SettingsBloomStruct> SettingsBloomList;
typedef std::map<std::string, SettingsMotionBlurStruct> SettingsMotionBlurList;
typedef std::map<std::string, MultipleElement> SettingsList;

class SettingManager
{

public:
	SettingManager();

	void							LoadSettings();
	void							SaveSettings(const char* Name);
	SettingsList					GetShaders();
	SettingsList					GetSections(const char* Name);
	SettingsList					GetSettings(const char* Name, const char* Section);
	float							GetSetting(const char* Name, const char* Section, const char* Setting);
	void							SetSetting(const char* Name, const char* Section, const char* Setting, float Value);
	bool							GetEnabled(const char* Name);
	SettingsWaterStruct*			GetSettingsWater(const char* PlayerLocation);
	SettingsDepthOfFieldStruct*		GetSettingsDepthOfField(const char* Section);
	SettingsAmbientOcclusionStruct*	GetSettingsAmbientOcclusion(const char* Section);
	SettingsColoringStruct*			GetSettingsColoring(const char* PlayerLocation);
	SettingsBloomStruct*			GetSettingsBloom(const char* PlayerLocation);
	SettingsMotionBlurStruct*		GetSettingsMotionBlur(const char* Section);
	void							InitCamera();
	
	char							CurrentPath[MAX_PATH];
	char							GameFilePath[MAX_PATH];
	bool							GameLoading;
	SettingsMainStruct				SettingsMain;
	SettingsGrassStruct				SettingsGrass;
	SettingsHDRStruct				SettingsHDR;
	SettingsPOMStruct				SettingsPOM;
	SettingsTerrainStruct			SettingsTerrain;
	SettingsSkinStruct				SettingsSkin;
	SettingsGodRaysStruct			SettingsGodRays;
	SettingsCinemaStruct			SettingsCinema;
	SettingsPrecipitationsStruct	SettingsPrecipitations;
	SettingsBloodStruct				SettingsBlood;
	SettingsLowHFStruct				SettingsLowHF;
	SettingsSharpeningStruct		SettingsSharpening;
	SettingsShadowsStruct			SettingsShadows;
	SettingsCameraStruct			SettingsCamera;

private:
	char Water_IceEnabled[255];
	char Water_choppiness[255];
	char Water_waveWidth[255];
	char Water_waveSpeed[255];
	char Water_reflectivity[255];
	char Water_causticsStrength[255];
	char Water_causticsStrengthS[255];
	char Water_shoreFactor[255];
	char Water_turbidity[255];
	char Water_inScattCoeff[255];
	char Water_inExtCoeff_R[255];
	char Water_inExtCoeff_G[255];
	char Water_inExtCoeff_B[255];
	char Water_IceinExtCoeff_R[255];
	char Water_IceinExtCoeff_G[255];
	char Water_IceinExtCoeff_B[255];
	char Water_Icechoppiness[255];
	char Water_IcewaveWidth[255];
	char Water_Icereflectivity[255];
	char Water_IceshoreFactor[255];
	char Water_Iceturbidity[255];
	char Water_depthDarkness[255];
	char Water_LODdistance[255];
	char Water_MinLOD[255];
	char Water_LensTimeMultA[255];
	char Water_LensTimeMultB[255];
	char Water_LensTime[255];
	char Water_LensAmount[255];
	char Water_LensViscosity[255];

	char Coloring_Strength[255];
	char Coloring_BaseGamma[255];
	char Coloring_Fade[255];
	char Coloring_Contrast[255];
	char Coloring_Saturation[255];
	char Coloring_Bleach[255];
	char Coloring_BleachLuma[255];
	char Coloring_ColorCurve[255];
	char Coloring_ColorCurveR[255];
	char Coloring_ColorCurveG[255];
	char Coloring_ColorCurveB[255];
	char Coloring_EffectGamma[255];
	char Coloring_EffectGammaR[255];
	char Coloring_EffectGammaG[255];
	char Coloring_EffectGammaB[255];
	char Coloring_Linearization[255];

	char Bloom_BloomIntensity[255];
	char Bloom_OriginalIntensity[255];
	char Bloom_BloomSaturation[255];
	char Bloom_OriginalSaturation[255];
	char Bloom_Luminance[255];
	char Bloom_MiddleGray[255];
	char Bloom_WhiteCutOff[255];

	SettingsWaterList				SettingsWater;
	SettingsDepthOfFieldList		SettingsDepthOfField;
	SettingsAmbientOcclusionList	SettingsAmbientOcclusion;
	SettingsColoringList			SettingsColoring;
	SettingsBloomList				SettingsBloom;
	SettingsMotionBlurList			SettingsMotionBlur;

};

void CreateSettingsHook();