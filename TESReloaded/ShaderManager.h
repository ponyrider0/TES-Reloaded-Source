#pragma once

#include <d3d9.h>
#include <d3dx9mesh.h>

enum ShaderTypeEnum
{
	Vertex,
	Pixel,
};

struct RuntimeVariable {
  int offset, length;
  const char *name;
  union mem {
    bool condition;
    struct iv { int vec[4]; } *integer;
    struct fv { float vec[4]; } *floating;
    struct tv { D3DSAMPLERSTATETYPE Type; DWORD Value; } *state;
    IDirect3DBaseTexture9 *texture;
  } vals;
};

class RuntimeShaderRecord
{
public:
	RuntimeShaderRecord();
	~RuntimeShaderRecord();

	void					CreateCT();
	void					SetCT();
	bool					LoadShader(const char *Name);
	DWORD*					LoadTexture(const char *sName, int *TexNum, DWORD *States);
	void*					pCustomCT;

	IDirect3DTexture9 **	pCopyRT; 
	IDirect3DTexture9 **	pCopyDS;

	RuntimeVariable *		pBool;
	RuntimeVariable *		pInt4;
	RuntimeVariable *		pFloat4;
	RuntimeVariable *		pTexture;
	RuntimeVariable *		pSampler;

	char					Name[20];
	void*					pFunction;
	char* 					pSource;
	ID3DXBuffer*			pErrors;
	ID3DXBuffer*			pShader;
	ID3DXConstantTable*		pTable;
	ShaderTypeEnum			ShaderType;

};

struct ShaderConstants
{

	D3DXVECTOR4		rcpres;
	D3DXVECTOR4		rcpresh;
	D3DXVECTOR4		rcpresd;

	D3DXVECTOR4		SunDir;
	D3DXVECTOR4		SunTiming;
	D3DXVECTOR4		SunAmount;
	D3DXVECTOR4		GameTime;
	D3DXVECTOR4		TikTiming;
	D3DXVECTOR4		TextureData;
	float			FoV;
	float			FrameTime;
	
	TESWeather*		pWeather;
	TESObjectCELL*	currentCell;
	TESWorldSpace*	currentWorldSpace;
	float			currentsunGlare;
	float			currentwindSpeed;
	UInt8			oldsunGlare;
	UInt8			oldwindSpeed;
	D3DXVECTOR4		fogColor;
	D3DXVECTOR4		oldfogColor;
	D3DXVECTOR4		sunColor;
	D3DXVECTOR4		oldsunColor;

	float			fogStart;
	float			currentfogStart;
	float			oldfogStart;
	float			fogEnd;
	float			currentfogEnd;
	float			oldfogEnd;
	float			WaterHeight;
	bool			HasWater;
	
	// Water settings
	D3DXVECTOR4		Water_waveSpeed;
	D3DXVECTOR4		Water_waterCoefficients;
	D3DXVECTOR4		Water_waveParams;
	D3DXVECTOR4		Water_waterVolume;
	D3DXVECTOR4		Water_foamAndLODSettings;
	D3DXVECTOR4		Water_depthColor;
	D3DXVECTOR4		Water_shallowColor;
	float			Water_depthDarkness;
	float			Water_IceAmount;

	// HDR settings
	D3DXVECTOR4		HDR_ToneMapping;

	// Grass settings
	D3DXVECTOR4		Grass_Scale;

	// POM settings
	D3DXVECTOR4		POM_ParallaxData;
	
	// Terrain settings
	D3DXVECTOR4		Terrain_Data;

	// Skin settings
	D3DXVECTOR4		Skin_SkinData;
	D3DXVECTOR4		Skin_SkinColor;
	
	// Shadows settings
	D3DXVECTOR4		Shadows_Data;
	
	// Precipitations settings
	D3DXVECTOR4		Precipitations_Data;

	// WaterLens settings
	D3DXVECTOR4		WaterLens_Time;
	float			WaterLens_Amount;
	float			WaterLens_Viscosity;
	float			WaterLens_Percent;

	// GodRays settings
	D3DXVECTOR4		GodRays_Ray;
	D3DXVECTOR4		GodRays_RayColor;
	int				GodRays_LightShaftPasses;
	float			GodRays_RayDensity;
	float			GodRays_RayVisibility;
	float			GodRays_Saturate;
	float			GodRays_Luminance;
	float			GodRays_GlobalMultiplier;

	// Depth Of Field settings
	bool			DepthOfField_Enabled;
	bool			DepthOfField_DistantBlur;
	float			DepthOfField_DistantBlurStartRange;
	float			DepthOfField_DistantBlurEndRange;
	float			DepthOfField_BaseBlurRadius;
	float			DepthOfField_BlurFallOff;
	float			DepthOfField_Radius;
	float			DepthOfField_DiameterRange;
	float			DepthOfField_NearBlurCutOff;

	// Ambient Occlusion settings
	bool			AmbientOcclusion_Enabled;
	float			AmbientOcclusion_RadiusMultiplier;
	float			AmbientOcclusion_StrengthMultiplier;
	float			AmbientOcclusion_ClampStrength;
	float			AmbientOcclusion_AngleBias;
	float			AmbientOcclusion_Range;
	float			AmbientOcclusion_LumThreshold;
	float			AmbientOcclusion_BlurDropThreshold;
	float			AmbientOcclusion_BlurRadiusMultiplier;

	// Coloring settings
	D3DXVECTOR4		Coloring_ColorCurve;
	D3DXVECTOR4		Coloring_EffectGamma;
	float			Coloring_Strength;
	float			Coloring_BaseGamma;
	float			Coloring_Fade;
	float			Coloring_Contrast;
	float			Coloring_Saturation;
	float			Coloring_Bleach;
	float			Coloring_BleachLuma;
	float			Coloring_Linearization;
	
	// Cinema settings
	float			Cinema_AspectRatio;
	float			Cinema_VignetteRadius;
	float			Cinema_VignetteDarkness;

	// Bloom settings
	D3DXVECTOR4		Bloom_BloomValues;
	float			Bloom_Luminance;
	float			Bloom_MiddleGray;
	float			Bloom_WhiteCutOff;

	// SnowAccumulation settings
	D3DXVECTOR4		SnowAccumulation_Params;

	// BloodLens settings
	D3DXVECTOR4		BloodLens_Params;
	D3DXVECTOR4		BloodLens_BloodColor;
	D3DXVECTOR4		BloodLens_Time;
	float			BloodLens_Percent;
	
	// MotionBlur settings
	D3DXVECTOR4		MotionBlur_BlurParams;
	float			MotionBlur_oldAngleX;
	float			MotionBlur_oldAngleZ;
	float			MotionBlur_AmountX;
	float			MotionBlur_AmountY;
	float			MotionBlur_oldAmountX;
	float			MotionBlur_oldAmountY;
	float			MotionBlur_oldoldAmountX;
	float			MotionBlur_oldoldAmountY;
	
	//LowHF settings
	D3DXVECTOR4		LowHF_Params;
	float			LowHF_HealthCoeff;
	float			LowHF_FatigueCoeff;

	//WetWorld settings
	D3DXVECTOR4		WetWorld_Coeffs;
	float			WetWorld_RainAmount;
	float			WetWorld_RippleWeight;

};

class ShaderManager // Never disposed
{

public:
	ShaderManager();

	void					InitializeConstants();
	void					UpdateFrameConstants();
	float					GetShaderConst(const char* Name, const char* Const);
	void					BeginScene();
	RuntimeShaderRecord*	LoadShader(const char *Name);

	ShaderConstants			ShaderConst;
	IDirect3DSurface9*		RenderSurface;
	IDirect3DTexture9* 		RenderTexture;
	bool					RenderSurfaceFilled;
	bool					DepthBufferFilled;
	time_t					currentTime;

};