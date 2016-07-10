#pragma once
#include <d3d9.h>
#include <map>

typedef enum {
  TR_PLANAR = 0,
  TR_CUBIC = 1,
  TR_VOLUMETRIC = 2
} TextureRecordType;

class TextureRecord
{
public:
	TextureRecord();
	~TextureRecord();

	bool LoadTexture(TextureRecordType type, const char *fp);
	const char *GetPath();
	IDirect3DBaseTexture9* GetTexture();

private:
	char				Filepath[MAX_PATH];
	TextureRecordType		type;
	union {
	  IDirect3DBaseTexture9*	texture;
	  IDirect3DTexture9*		textureP;
	  IDirect3DCubeTexture9*	textureC;
	  IDirect3DVolumeTexture9*	textureV;
	};
};

typedef std::map<int, TextureRecord*> TextureList;

class TextureManager // Never disposed
{
public:
	TextureManager();

	int								TextureIndex;
	int								LoadTexture(const char *Filename, TextureRecordType type);
	TextureRecord*					GetTexture(int TextureNum);
	void							OnReleaseDevice();

	TextureList						Textures;
};