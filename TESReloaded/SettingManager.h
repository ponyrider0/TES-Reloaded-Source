#pragma once
#include <map>
#include <list>

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
#define kSettingFullScreen 0x00B06C74
#define kSettingNearDistance 0x00B03134
#define kSettingWorldFoV 0x00B0313C
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
#define kSettingFullScreen 0x012CF5EC
#define kSettingNearDistance 0x012C8D54
#define kSettingWorldFoV 0x012C8D78
#endif

static const UInt32*	SettingGridsToLoad				= (UInt32*)0x00B06A2C; // Only Oblivion
static const UInt32*	SettingGridDistantCount			= (UInt32*)0x00B06A90; // Only Oblivion
static float*			SettingWorldFoV					= (float*)kSettingWorldFoV;
static float*			Setting1stPersonFoV				= (float*)0x012C8D84; // Only Skyrim
static UInt8*			SettingFullScreen				= (UInt8*)kSettingFullScreen;
static float*			SettingNearDistance				= (float*)kSettingNearDistance;
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

struct SettingsMainStruct
{
	char ScreenshotPath[MAX_PATH];
	char MenuFont[80];
	char SleepingModeRestMessage[80];
	char MainSettingsFullFile[MAX_PATH];
	char MenuValueFormat[5];
	UInt8 ScreenshotKey;
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
	UInt8 MenuTextColorNormal[3];
	UInt8 MenuTextShadowColorNormal[3];
	UInt8 MenuTextColorSelected[3];
	UInt8 MenuTextShadowColorSelected[3];
	UInt8 MenuTextColorEditing[3];
	UInt8 MenuTextShadowColorEditing[3];
	UInt8 MenuTextSize;
	UInt8 MenuKeyEnable;
	UInt8 MenuKeyUp;
	UInt8 MenuKeyDown;
	UInt8 MenuKeyLeft;
	UInt8 MenuKeyRight;
	UInt8 MenuKeyPageUp;
	UInt8 MenuKeyPageDown;
	UInt8 MenuKeyAdd;
	UInt8 MenuKeySubtract;
	UInt8 MenuKeySave;
	UInt8 MenuKeyEditing;
	bool DevelopCompileShaders;
	bool DevelopCompileEffects;
	bool DevelopTraceShaders;
	UInt8 ScreenshotType;
	int WaterReflectionMapSize;
	int FrameRateAverage;
	int FrameRateGap;
	int FrameRateDelay;
	int FrameRateGridStep;
	int FrameRateFadeMinObjects;
	int FrameRateFadeMinActors;
	int FrameRateGridMin;
	int MenuTitleX;
	int MenuTitleY;
	int MenuTitleRect;
	int MenuX;
	int MenuY;
	int MenuRect;
	int MenuExtraRect;
	int MenuRowSpace;
	int MenuRowsPerPage;
	float FrameRateFadeStep;
	float FarPlaneDistance;
	float FoV;
	NiPoint3 CameraModeOffset;
	float CameraModeNearDistanceFirst;
	float CameraModeNearDistanceThird;
	NiPoint3 CameraModeDialogOffset;
	float MenuStepValue;
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
typedef std::map<std::string, std::string> ShadersList;
typedef std::list<std::string> SectionsList;
typedef std::map<std::string, float> SettingsList;

class SettingManager
{

public:
	SettingManager();

	void							LoadSettings();
	void							SaveSettings(const char* Name);
	ShadersList						GetShaders();
	SectionsList					GetSections(const char* Name);
	SettingsList					GetSettings(const char* Name, const char* Section);
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
	char Water_IceEnabled[80];
	char Water_choppiness[80];
	char Water_waveWidth[80];
	char Water_waveSpeed[80];
	char Water_reflectivity[80];
	char Water_causticsStrength[80];
	char Water_causticsStrengthS[80];
	char Water_shoreFactor[80];
	char Water_turbidity[80];
	char Water_inScattCoeff[80];
	char Water_inExtCoeff_R[80];
	char Water_inExtCoeff_G[80];
	char Water_inExtCoeff_B[80];
	char Water_IceinExtCoeff_R[80];
	char Water_IceinExtCoeff_G[80];
	char Water_IceinExtCoeff_B[80];
	char Water_Icechoppiness[80];
	char Water_IcewaveWidth[80];
	char Water_Icereflectivity[80];
	char Water_IceshoreFactor[80];
	char Water_Iceturbidity[80];
	char Water_depthDarkness[80];
	char Water_LODdistance[80];
	char Water_MinLOD[80];
	char Water_LensTimeMultA[80];
	char Water_LensTimeMultB[80];
	char Water_LensTime[80];
	char Water_LensAmount[80];
	char Water_LensViscosity[80];

	char Coloring_Strength[80];
	char Coloring_BaseGamma[80];
	char Coloring_Fade[80];
	char Coloring_Contrast[80];
	char Coloring_Saturation[80];
	char Coloring_Bleach[80];
	char Coloring_BleachLuma[80];
	char Coloring_ColorCurve[80];
	char Coloring_ColorCurveR[80];
	char Coloring_ColorCurveG[80];
	char Coloring_ColorCurveB[80];
	char Coloring_EffectGamma[80];
	char Coloring_EffectGammaR[80];
	char Coloring_EffectGammaG[80];
	char Coloring_EffectGammaB[80];
	char Coloring_Linearization[80];

	char Bloom_BloomIntensity[80];
	char Bloom_OriginalIntensity[80];
	char Bloom_BloomSaturation[80];
	char Bloom_OriginalSaturation[80];
	char Bloom_Luminance[80];
	char Bloom_MiddleGray[80];
	char Bloom_WhiteCutOff[80];

	SettingsWaterList				SettingsWater;
	SettingsDepthOfFieldList		SettingsDepthOfField;
	SettingsAmbientOcclusionList	SettingsAmbientOcclusion;
	SettingsColoringList			SettingsColoring;
	SettingsBloomList				SettingsBloom;
	SettingsMotionBlurList			SettingsMotionBlur;

};

void CreateSettingsHook();