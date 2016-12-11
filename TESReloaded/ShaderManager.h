#pragma once
#include <d3d9.h>
#include <d3dx9mesh.h>

#define EFFECTQUADFORMAT D3DFVF_XYZ | D3DFVF_TEX1

enum EffectRecordType
{
	EffectRecordType_Underwater,
	EffectRecordType_WaterLens,
	EffectRecordType_GodRays,
	EffectRecordType_DepthOfField,
	EffectRecordType_AmbientOcclusion,
	EffectRecordType_Coloring,
	EffectRecordType_Cinema,
	EffectRecordType_Bloom,
	EffectRecordType_SnowAccumulation,
	EffectRecordType_BloodLens,
	EffectRecordType_SMAA,
	EffectRecordType_MotionBlur,
	EffectRecordType_LowHF,
	EffectRecordType_WetWorld,
	EffectRecordType_Sharpening,
	EffectRecordType_Custom,
};

enum ShaderType
{
	ShaderType_Vertex,
	ShaderType_Pixel,
};

struct ShaderConstants
{
	D3DXVECTOR4		ReciprocalResolution;
	D3DXVECTOR4		ReciprocalResolutionWater;

	D3DXVECTOR4		SunDir;
	D3DXVECTOR4		SunTiming;
	D3DXVECTOR4		SunAmount;
	D3DXVECTOR4		GameTime;
	D3DXVECTOR4		TikTiming;
	D3DXVECTOR4		TextureData;
	
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
	D3DXVECTOR4		fogData;

	float			currentfogStart;
	float			oldfogStart;
	float			currentfogEnd;
	float			oldfogEnd;
	bool			HasWater;

	// Water settings
	D3DXVECTOR4		Water_waterCoefficients;
	D3DXVECTOR4		Water_waveParams;
	D3DXVECTOR4		Water_waterVolume;
	D3DXVECTOR4		Water_waterSettings;
	D3DXVECTOR4		Water_deepColor;
	D3DXVECTOR4		Water_shallowColor;

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
	float			WaterLens_TimeAmount;
	float			WaterLens_Percent;

	// GodRays settings
	D3DXVECTOR4		GodRays_Ray;
	D3DXVECTOR4		GodRays_RayColor;
	D3DXVECTOR4		GodRays_Data;

	// Depth Of Field settings
	bool			DepthOfField_Enabled;
	D3DXVECTOR4		DepthOfField_Blur;
	D3DXVECTOR4		DepthOfField_Data;

	// Ambient Occlusion settings
	bool			AmbientOcclusion_Enabled;
	D3DXVECTOR4		AmbientOcclusion_AOData;
	D3DXVECTOR4		AmbientOcclusion_Data;

	// Coloring settings
	D3DXVECTOR4		Coloring_ColorCurve;
	D3DXVECTOR4		Coloring_EffectGamma;
	D3DXVECTOR4		Coloring_Data;
	D3DXVECTOR4		Coloring_Values;

	// Cinema settings
	D3DXVECTOR4		Cinema_Data;

	// Bloom settings
	D3DXVECTOR4		Bloom_BloomData;
	D3DXVECTOR4		Bloom_BloomValues;

	// SnowAccumulation settings
	D3DXVECTOR4		SnowAccumulation_Params;

	// BloodLens settings
	D3DXVECTOR4		BloodLens_Params;
	D3DXVECTOR4		BloodLens_BloodColor;
	D3DXVECTOR4		BloodLens_Time;
	float			BloodLens_Percent;

	// MotionBlur settings
	D3DXVECTOR4		MotionBlur_BlurParams;
	D3DXVECTOR4		MotionBlur_Data;
	float			MotionBlur_oldAngleX;
	float			MotionBlur_oldAngleZ;
	float			MotionBlur_oldAmountX;
	float			MotionBlur_oldAmountY;
	float			MotionBlur_oldoldAmountX;
	float			MotionBlur_oldoldAmountY;

	// LowHF settings
	D3DXVECTOR4		LowHF_Data;
	float			LowHF_HealthCoeff;
	float			LowHF_FatigueCoeff;

	// WetWorld settings
	D3DXVECTOR4		WetWorld_Coeffs;
	D3DXVECTOR4		WetWorld_Data;

	// Sharpening settings
	D3DXVECTOR4		Sharpening_Data;

};

struct ShaderValue
{
	UInt32				RegisterIndex;
	UInt32				RegisterCount;
	union {
	D3DXVECTOR4*		Value;
	TextureRecord*		Texture;
	};
};

class ShaderProgram
{
public:
	void					SetConstantTableValue(LPCSTR Name, UInt32 Index);

	ShaderValue*			FloatShaderValues;
	UInt32					FloatShaderValuesCount;
	ShaderValue*			TextureShaderValues;
	UInt32					TextureShaderValuesCount;
};

class ShaderRecord : public ShaderProgram // Never disposed
{
public:
	ShaderRecord();

	void					CreateCT();
	void					SetCT();
	bool					LoadShader(const char* Name);
	
	ShaderType				Type;
	bool					HasCT;
	bool					HasRB; 
	bool					HasDB;
	void*					pFunction;
	char* 					pSource;
	ID3DXBuffer*			pErrors;
	ID3DXBuffer*			pShader;
	ID3DXConstantTable*		pTable;

};

class EffectRecord : public ShaderProgram
{

public:
	EffectRecord();
	~EffectRecord();

	bool						LoadEffect(const char* Name);
	void						CreateCT();
	void						SetCT();
	void						Render(IDirect3DDevice9* D3DDevice, IDirect3DSurface9* RenderTarget, IDirect3DSurface9* RenderedSurface, bool ClearRenderTarget);

	bool						Enabled;
	char*	 					pSource;
	ID3DXBuffer*				pErrors;
	ID3DXEffect*				pEffect;

};

typedef std::map<std::string, EffectRecord*> CustomEffectRecordList;
typedef std::map<std::string, D3DXVECTOR4> CustomConstants;

class ShaderManager // Never disposed
{

public:
	ShaderManager();

	void					InitializeConstants();
	void					UpdateConstants();
	void					BeginScene();
	ShaderRecord*			LoadShader(const char *Name);
	void					CreateEffect(EffectRecordType EffectType);
	bool					LoadEffect(EffectRecord* TheEffect, char* Filename, char* CustomEffectName);
	void					DisposeEffect(EffectRecord* TheEffect);
	void					RenderEffects();
	void					EnableEffect(const char* Name, bool Value);
	void					SetShaderValue(const char* Name, const char* ConstantName, D3DXVECTOR4 Value);
	void					SetCustomShaderValue(const char* Name, const char* ConstantName, D3DXVECTOR4 Value);

	struct					EffectQuad { float x, y, z; float u, v; };
	ShaderConstants			ShaderConst;
	CustomConstants			CustomConst;
	IDirect3DTexture9*		SourceTexture;
	IDirect3DSurface9*		SourceSurface;
	IDirect3DTexture9* 		RenderedTexture;
	IDirect3DSurface9*		RenderedSurface;
	IDirect3DTexture9*		RenderTextureSMAA;
	IDirect3DSurface9*		RenderSurfaceSMAA;
	bool					RenderedBufferFilled;
	bool					DepthBufferFilled;
	IDirect3DVertexBuffer9*	EffectVertex;
	EffectRecord*			UnderwaterEffect;
	EffectRecord*			WaterLensEffect;
	EffectRecord*			GodRaysEffect;
	EffectRecord*			DepthOfFieldEffect;
	EffectRecord*			AmbientOcclusionEffect;
	EffectRecord*			ColoringEffect;
	EffectRecord*			CinemaEffect;
	EffectRecord*			BloomEffect;
	EffectRecord*			SnowAccumulationEffect;
	EffectRecord*			BloodLensEffect;
	EffectRecord*			SMAAEffect;
	EffectRecord*			MotionBlurEffect;
	EffectRecord*			LowHFEffect;
	EffectRecord*			WetWorldEffect;
	EffectRecord*			SharpeningEffect;
	EffectRecord*			CustomEffect;
	CustomEffectRecordList	CustomEffectRecords;
};