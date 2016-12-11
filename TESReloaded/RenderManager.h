#pragma once
#if defined(OBLIVION)
#include "obse\NiRenderer.h"
#include "obse\NiShader.h"
#elif defined(SKYRIM)
#include "skse\NiRenderer.h"
#include "skse\NiShader.h"
#endif

class NiD3DVertexShaderEx : public NiD3DVertexShader
{

public:
	ShaderRecord*			ShaderProg;
	char*					ShaderName;

};

class NiD3DPixelShaderEx : public NiD3DPixelShader
{

public:
	ShaderRecord*			ShaderProg;
	char*					ShaderName;

};

class RenderManager: public NiDX9Renderer
{

public:

	void				Initialize();
	void				ResolveDepthBuffer();
	void				GetCameraData();
	void				SetCameraData();
	void				SetSceneGraph();
	HRESULT				SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* Value, bool SetState);

	D3DXVECTOR4			CameraForward;
	D3DXVECTOR4			CameraPosition;
	IDirect3DSurface9*	RenderTarget;
	IDirect3DSurface9*	DepthSurface;
	IDirect3DTexture9*	DepthTexture;
	IDirect3DTexture9*	DepthTextureINTZ;
	RECT				SaveGameScreenShotRECT;
	bool				IsSaveGameScreenShot;
	bool				FirstPersonView;
	bool				RESZ;
	
};