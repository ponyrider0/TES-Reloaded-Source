#pragma once
#include <d3d9.h>
#include <map>

static const int SamplerStatesMax = 12;
static const char* WordSourceBuffer = "TESR_SourceBuffer";
static const char* WordRenderedBuffer = "TESR_RenderedBuffer";
static const char* WordDepthBuffer = "TESR_DepthBuffer";

enum TextureRecordType {
	
	TextureRecordType_None,
	TextureRecordType_PlanarBuffer,
	TextureRecordType_VolumeBuffer,
	TextureRecordType_CubeBuffer,
	TextureRecordType_SourceBuffer,
	TextureRecordType_RenderedBuffer,
	TextureRecordType_DepthBuffer,

};

class TextureRecord
{
public:
	TextureRecord();

	bool					LoadTexture(TextureRecordType Type, const char* Filename);
	void					SetSamplerState(D3DSAMPLERSTATETYPE SamplerType, DWORD Value);

	IDirect3DBaseTexture9*	Texture;
	DWORD					SamplerStates[SamplerStatesMax];

};

typedef std::map<std::string, TextureRecord*> TextureList;

class TextureManager // Never disposed
{
public:
	TextureManager();

	TextureRecord*			LoadTexture(const char* ShaderSource, UInt32 RegisterIndex);
	
	TextureList				Textures;
};