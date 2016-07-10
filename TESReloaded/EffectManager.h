#pragma once

#include <d3d9.h>
#include <d3dx9effect.h>
#include <map>

#define EFFECTQUADFORMAT D3DFVF_XYZ | D3DFVF_TEX1

enum EffectTypeEnum
{
	Underwater,
	WaterLens,
	GodRays,
	DepthOfField,
	AmbientOcclusion,
	Coloring,
	Cinema,
	Bloom,
	SnowAccumulation,
	BloodLens,
	SMAA,
	MotionBlur,
	LowHF,
	WetWorld,
	Sharpening,
	Custom,
};

class EffectRecord
{

public:
	EffectRecord();
	~EffectRecord();
	
	bool						LoadEffect(char *Filename);
	void						ApplyCompileDirectives();
	void						ApplyPermanents(IDirect3DTexture9* RenderTexture, IDirect3DTexture9* SourceTexture);
	void						ApplyConstants(EffectTypeEnum EffectType);
	void						Render(IDirect3DDevice9 *D3DDevice, IDirect3DSurface9 *RenderTo, IDirect3DSurface9 *RenderSurface, bool ClearRenderTo);
	void						GetValue(const char* Const, float *Value);
	void						SetValue(const char* Const, float Value);

	bool						Enabled;

private:
	ID3DXBuffer*				pErrors;
	ID3DXEffect*				pEffect;

};

typedef std::map<std::string, EffectRecord*> CustomEffectRecordList;

class EffectManager // Never disposed
{

public:
	EffectManager();
	
	void						Initialize();
	void						Render();
	void						CreateEffect(EffectTypeEnum EffectType);
	void						DisposeEffect(EffectRecord* TheEffect);
	bool						LoadEffect(EffectRecord* TheEffect, char* Filename, char* CustomEffectName);
	float						GetCustomEffectConst(const char* Name, const char* Const);
	void						SetCustomEffectConst(const char* Name, const char* Const, float Value);
	void						SetEffectEnabled(const char* Name, bool Value);

	bool						IsInitialized;

private:
	struct						EffectQuad { float x,y,z; float u,v; };
	CustomEffectRecordList		CustomEffectRecords;
	EffectRecord*				UnderwaterEffect;
	EffectRecord*				WaterLensEffect;
	EffectRecord*				GodRaysEffect;
	EffectRecord*				DepthOfFieldEffect;
	EffectRecord*				AmbientOcclusionEffect;
	EffectRecord*				ColoringEffect;
	EffectRecord*				CinemaEffect;
	EffectRecord*				BloomEffect;
	EffectRecord*				SnowAccumulationEffect;
	EffectRecord*				BloodLensEffect;
	EffectRecord*				SMAAEffect;
	EffectRecord*				MotionBlurEffect;
	EffectRecord*				LowHFEffect;
	EffectRecord*				WetWorldEffect;
	EffectRecord*				SharpeningEffect;
	EffectRecord*				CustomEffect;
	IDirect3DVertexBuffer9*		EffectVertex;
	IDirect3DTexture9*			RenderToTexture;
	IDirect3DSurface9*			RenderToSurface;
	IDirect3DTexture9*			RenderTexture;
	IDirect3DSurface9*			RenderSurface;
	IDirect3DTexture9*			SourceTexture;
	IDirect3DSurface9*			SourceSurface;

};