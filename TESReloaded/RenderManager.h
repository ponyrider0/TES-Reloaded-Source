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
	RuntimeShaderRecord*	ShaderRecord;
	char*					ShaderName;

};

class NiD3DPixelShaderEx : public NiD3DPixelShader
{

public:
	RuntimeShaderRecord*	ShaderRecord;
	char*					ShaderName;

};

class RenderManager: public NiDX9Renderer
{

public:
	RenderManager();
	~RenderManager();

	void				Initialize();
	void				ResolveDepthBuffer();
	void				GetCameraData();
	void				SetCameraData();
	void				SetSceneGraph();
	
	D3DXVECTOR4			CameraForward;
	D3DXVECTOR4			CameraPosition;
	IDirect3DSurface9*	LastFrame;
	IDirect3DSurface9*	DepthSurface;
	IDirect3DTexture9*	DepthTexture;
	IDirect3DTexture9*	DepthTextureINTZ;
	RECT				SaveGameScreenShotRECT;
	bool				IsSaveGameScreenShot;
	bool				FirstPersonView;
	bool				RESZ;
	
};