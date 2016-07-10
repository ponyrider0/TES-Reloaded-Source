#include <d3dx9tex.h>

TextureRecord::TextureRecord() {
  type = TR_PLANAR;
  texture = NULL;
  Filepath[0] = 0;
}

TextureRecord::~TextureRecord() {} // Never disposed

bool TextureRecord::LoadTexture(TextureRecordType type, const char *fp) {
	
  if (type == TR_PLANAR) {
    IDirect3DTexture9 *tex = NULL;
	D3DXCreateTextureFromFileA(TheRenderManager->device,fp,&tex);
	if (tex == NULL)
        return false;

    this->textureP = tex;
  }
  else if (type == TR_CUBIC) {
    IDirect3DCubeTexture9 *tex = NULL;
    D3DXCreateCubeTextureFromFileA(TheRenderManager->device, fp, &tex);
	if (tex == NULL)
		return false;

    this->textureC = tex;
  }
  else if (type == TR_VOLUMETRIC) {
    IDirect3DVolumeTexture9 *tex = NULL;
    D3DXCreateVolumeTextureFromFileA(TheRenderManager->device,fp,&tex);
	if (tex == NULL)
		return false;

	this->textureV = tex;
  }
  this->type = type;
  strcpy_s(this->Filepath, 256, fp);
  return true;
}

IDirect3DBaseTexture9 *TextureRecord::GetTexture() {
  return this->texture;
}

const char *TextureRecord::GetPath() {
  return this->Filepath;
}

TextureManager::TextureManager() {
  _MESSAGE("Starting the textures manager...");
  TextureIndex = 0;
}

int TextureManager::LoadTexture(const char *Filename, TextureRecordType type) {

  char NewPath[256];
  strcpy_s(NewPath, 256, "data\\textures\\");
  strcat_s(NewPath, 256, Filename);

  TextureList::iterator Texture = Textures.begin();

  while (Texture != Textures.end()) {
    if (!_stricmp(NewPath, Texture->second->GetPath())) {
      _MESSAGE("Texture linked: %s", NewPath);
      return Texture->first;
    }
    Texture++;
  }

  TextureRecord *NewTex = new TextureRecord();
  if (!NewTex->LoadTexture(type, NewPath)) {
	  _MESSAGE("ERROR: Cannot load texture %s", NewPath);
    delete NewTex;
    return -1;
  }

  _MESSAGE("Texture loaded: %s", NewPath);
  Textures[TextureIndex++] = NewTex;

  return TextureIndex - 1;
}

TextureRecord *TextureManager::GetTexture(int TextureNum) {

	 std::map<int, TextureRecord*>::iterator v = Textures.find(TextureNum);
	 if (v == Textures.end())
		 return NULL;
	 else
		 return v->second;

}

void TextureManager::OnReleaseDevice() {

	for (TextureList::iterator iter = Textures.begin(); iter != Textures.end(); ++iter) {
		iter->second->GetTexture()->Release();
	}
	Textures.clear();

}
