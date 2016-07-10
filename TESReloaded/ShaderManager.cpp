#if defined(OBLIVION)
#include "obse\GameData.h"
#define fogNight_nearFog fogNight.nearFog
#define fogNight_farFog fogNight.farFog
#define fogDay_nearFog fogDay.nearFog
#define fogDay_farFog fogDay.farFog
#define isThirdPersonView (*g_thePlayer)->isThirdPerson
#define GetPlayerHealthPercent (float)(*g_thePlayer)->GetActorValue(kActorVal_Health) / (float)(*g_thePlayer)->GetBaseActorValue(kActorVal_Health)
#define GetPlayerFatiguePercent (float)(*g_thePlayer)->GetActorValue(kActorVal_Fatigue) / (float)(*g_thePlayer)->GetBaseActorValue(kActorVal_Fatigue)
#define CurrentBlend *WaterBlend
#define WaterLevel ShaderConst.currentCell->GetWaterHeight()
#elif defined(SKYRIM)
#include "skse\GameData.h"
#include "skse\GameCamera.h"
#define eColor_Fog kColorType_FogNear
#define eColor_Sun kColorType_Sun
#define fogNight_nearFog fogDistance.nearNight
#define fogNight_farFog fogDistance.farNight
#define fogDay_nearFog fogDistance.nearDay
#define fogDay_farFog fogDistance.farDay
#define sunGlare general.sunGlare
#define windSpeed general.windSpeed
#define precipType general.precipType
#define isThirdPersonView PlayerCamera::GetSingleton()->cameraState->stateId == 9
#define rotZ rot.z
#define rotX rot.x
#define GetPlayerHealthPercent (*g_thePlayer)->actorValueOwner.GetCurrent(24) / (*g_thePlayer)->actorValueOwner.GetBase(24)
#define GetPlayerFatiguePercent (*g_thePlayer)->actorValueOwner.GetCurrent(26) / (*g_thePlayer)->actorValueOwner.GetBase(26)
#define CurrentBlend 0.0f
#define LightingData InteriorData
#define lighting interiorData
#define WaterLevel (*g_thePlayer)->loadedState->waterHeight
#endif
#include <assert.h>
#include <fstream>
#include <ctime>

RuntimeShaderRecord::RuntimeShaderRecord() {

	Name[0] = '\0';
	pCustomCT = NULL;
	pBool     = NULL;
	pInt4     = NULL;
	pFloat4   = NULL;
	pTexture  = NULL;
	pCopyRT	  = NULL;
	pCopyDS   = NULL;
	pFunction = NULL;
	pSource   = NULL;
	pShader   = NULL;
	pTable    = NULL;
	pErrors   = NULL;

}

RuntimeShaderRecord::~RuntimeShaderRecord() {} // Never disposed

bool RuntimeShaderRecord::LoadShader(const char *Name) {
  
	char FileName[MAX_PATH];
	char FileNameBinary[MAX_PATH];

	strcpy(FileName, ShadersPath);
	if (!memcmp(Name,"WATER",5)) {
		if (TheSettingManager->SettingsMain.EnableWater == 0) return false;
		strcat(FileName, "Water\\");
	}
	else if (!memcmp(Name,"GRASS",5)) {
		if (TheSettingManager->SettingsMain.EnableGrass == 0) return false;
		strcat(FileName, "Grass\\");
	}
	else if (!memcmp(Name,"PRECIP",6)) {
		if (TheSettingManager->SettingsMain.EnablePrecipitations == 0) return false;
		strcat(FileName, "Precipitations\\");
	}
	else if (!memcmp(Name,"HDR",3)) {
		if (TheSettingManager->SettingsMain.EnableHDR == 0) return false;
		strcat(FileName, "HDR\\");
	}
	else if (!memcmp(Name,"PAR",3)) {
		if (TheSettingManager->SettingsMain.EnablePOM == 0) return false;
		strcat(FileName, "POM\\");
	}
	else if (!memcmp(Name,"SKIN",4)) {
		if (TheSettingManager->SettingsMain.EnableSkin == 0) return false;
		strcat(FileName, "Skin\\");
	}
	else if (!memcmp(Name,"SLS2001",7) || !memcmp(Name,"SLS2064.vso",11) || !memcmp(Name,"SLS2068.pso",11) || !memcmp(Name,"SLS2042.vso",11) || !memcmp(Name,"SLS2048.pso",11) || !memcmp(Name,"SLS2043.vso",11) || !memcmp(Name,"SLS2049.pso",11)) {
		if (TheSettingManager->SettingsMain.EnableTerrain == 0) return false;
		strcat(FileName, "Terrain\\");
	}
	else if (!memcmp(Name,"GDECALS.vso",11) || !memcmp(Name,"GDECAL.pso",10) || !memcmp(Name,"SLS2040.vso",11) || !memcmp(Name,"SLS2046.pso",11)) {
		if (TheSettingManager->SettingsMain.EnableBlood == 0) return false;
		strcat(FileName, "Blood\\");
	}
	else if (!memcmp(Name,"SLS2073.vso",11) || !memcmp(Name,"SLS2074.vso",11) || !memcmp(Name,"SLS2075.vso",11) || !memcmp(Name,"SLS2080.pso",11) || !memcmp(Name,"SLS2081.pso",11)) {
		if (TheSettingManager->SettingsMain.EnableShadows == 0) return false;
		strcat(FileName, "Shadows\\");
	}
	else if (TheSettingManager->SettingsMain.DevelopShadersFolder) {
		strcat(FileName, "Develop\\");
	}
	else
		FileName[0] = 0;

	if (FileName[0]) {
		strcpy(this->Name, Name);
		strcat(FileName, Name);
		strcpy(FileNameBinary, FileName);
		strcat(FileName, ".hlsl");
		std::ifstream FileSource(FileName, std::ios::in|std::ios::binary|std::ios::ate);
		if (FileSource.is_open()) {
			std::streampos size = FileSource.tellg();
			pSource = new char [size];
			FileSource.seekg(0, std::ios::beg);
			FileSource.read(pSource, size);
			FileSource.close();
			if (strstr(Name, ".vso"))
				ShaderType = Vertex;
			else if (strstr(Name, ".pso"))
				ShaderType = Pixel;
			if (strstr(FileName, "Develop")) {
				D3DXCompileShaderFromFileA(FileName, NULL, NULL, "main", (ShaderType == Vertex ? "vs_3_0" : "ps_3_0"), NULL, &pShader, &pErrors, &pTable);
				if (pErrors) _MESSAGE((char*)pErrors->GetBufferPointer());
			}
			else {
				if (TheSettingManager->SettingsMain.DevelopCompileShaders) {
					D3DXCompileShaderFromFileA(FileName, NULL, NULL, "main", (ShaderType == Vertex ? "vs_3_0" : "ps_3_0"), NULL, &pShader, &pErrors, &pTable);
					if (pErrors) _MESSAGE((char*)pErrors->GetBufferPointer());
					if (pShader) {
						std::ofstream FileBinary(FileNameBinary,std::ios::out|std::ios::binary);
						FileBinary.write((char*)pShader->GetBufferPointer(), pShader->GetBufferSize());
						FileBinary.flush();
						FileBinary.close();
						_MESSAGE("Shader compiled: %s", FileName);
					}
				}
				else {
					std::ifstream FileBinary(FileNameBinary,std::ios::in|std::ios::binary|std::ios::ate);
					if (FileBinary.is_open()) {
						size = FileBinary.tellg();
						D3DXCreateBuffer(size, &pShader);
						FileBinary.seekg(0, std::ios::beg);
						void* pShaderBuffer = pShader->GetBufferPointer();
						FileBinary.read((char*)pShaderBuffer, size);
						FileBinary.close();
						D3DXGetShaderConstantTable((const DWORD*)pShaderBuffer, &pTable);
					}
					else
						_MESSAGE("ERROR: Shader %s not found. Try to enable the CompileShader option to attempt to recompile the shaders!", FileNameBinary);
				}
			}
			if (pShader) {
				pFunction = pShader->GetBufferPointer();
				CreateCT();
				_MESSAGE("Shader loaded: %s", FileNameBinary);
				return true;
			}
		}
	}
	return false;
}

DWORD *RuntimeShaderRecord::LoadTexture(const char *sName, int *TexNum, DWORD *States) {

  if (pSource && strstr(sName, "sampler")) {
    const char *key = "sampler";
    std::string tName(sName);
    std::string tPath(sName);

    tName.replace(tName.find(key), strlen(key), "tex");
    tPath.replace(tPath.find(key), strlen(key), "path");

    const char *sampler = strstr(pSource, sName);
    const char *texture = strstr(pSource, tName.c_str());
    const char *texpath = strstr(pSource, tPath.c_str());

    /* all different position */
    if (((texture != sampler) || !texture) &&
        ((texture != texpath) || !texpath) &&
        ((sampler != texpath))) {
      const char *samplert = sampler;
      while (--samplert > pSource)
	if (*samplert == ';')
	  break;

      const char *samplerb = (sampler  ? strchr(sampler     , '{') : NULL);
      const char *textureb = (texture  ? strchr(texture     , '"') : NULL);
      const char *texpathb = (texpath  ? strchr(texpath     , '"') : NULL);
      const char *samplere = (samplerb ? strchr(samplerb + 1, '}') : NULL);
      const char *texturee = (textureb ? strchr(textureb + 1, '"') : NULL);
      const char *texpathe = (texpathb ? strchr(texpathb + 1, '"') : NULL);

      // check for the sampler-type
      // check there is no ";" between the sampler and it's beginning block "{"
      // check there is no ";" between the texture and it's ending block ";"
      const char *sampleri = (samplert ? strstr(samplert, "sampler") : NULL);
      const char *samplerc = (sampler  ? strchr(sampler , ';') : NULL);
      const char *texturec = (texture  ? strchr(texture , ';') : NULL);
      const char *texpathc = (texpath  ? strchr(texpath , ';') : NULL);

      if ((sampleri <  sampler ) && (textureb || texpathb) && ((texturec >= texturee) || (texpathc >= texpathe))) {
      	char buf[256];
      	int len;

		if (textureb) {
		  len = texturee - (textureb + 1); if (len > 0)
		  strncpy(buf, textureb + 1, len);
		}
		else if (texpathb) {
		  len = texpathe - (texpathb + 1); if (len > 0)
		  strncpy(buf, texpathb + 1, len);
		}

		if (len > 0) {
		  buf[len] = '\0';
		  if (sampleri[7] == 'C') *TexNum = TheTextureManager->LoadTexture(buf, TR_CUBIC);
		  if (sampleri[7] == '3') *TexNum = TheTextureManager->LoadTexture(buf, TR_VOLUMETRIC);
		  if (sampleri[7] == '2') *TexNum = TheTextureManager->LoadTexture(buf, TR_PLANAR);
		  if (sampleri[7] == ' ') *TexNum = TheTextureManager->LoadTexture(buf, TR_PLANAR);
		}
      }

      if ((sampleri <  sampler ) && (samplerc >= samplerb) && (samplere >  samplerb)) {
		char buf[1024];
		int len;

		len = samplere - (samplerb + 1); if (len > 0)
		strncpy(buf, samplerb + 1, len);

		if (len > 0) {
		  buf[len] = '\0';

		  const char *au = strstr(buf, "AddressU");
		  const char *av = strstr(buf, "AddressV");
		  const char *aw = strstr(buf, "AddressW");
		  const char *mp = strstr(buf, "MIPFILTER");
		  const char *mn = strstr(buf, "MINFILTER");
		  const char *mg = strstr(buf, "MAGFILTER");
		  const char *bc = strstr(buf, "Bordercolor");

		  if (au) {
			char *end = (char *)strchr(au, ';'); if (end) *end = '\0';
			/**/ if (strstr(au, "WRAP"      )) { *States++ = D3DSAMP_ADDRESSU; *States++ = D3DTADDRESS_WRAP; }
			else if (strstr(au, "MIRROR"    )) { *States++ = D3DSAMP_ADDRESSU; *States++ = D3DTADDRESS_MIRROR; }
			else if (strstr(au, "CLAMP"     )) { *States++ = D3DSAMP_ADDRESSU; *States++ = D3DTADDRESS_CLAMP; }
			else if (strstr(au, "BORDER"    )) { *States++ = D3DSAMP_ADDRESSU; *States++ = D3DTADDRESS_BORDER; }
			else if (strstr(au, "MIRRORONCE")) { *States++ = D3DSAMP_ADDRESSU; *States++ = D3DTADDRESS_MIRRORONCE; }
		  }
		  if (av) {
			char *end = (char *)strchr(av, ';'); if (end) *end = '\0';
			/**/ if (strstr(av, "WRAP"      )) { *States++ = D3DSAMP_ADDRESSV; *States++ = D3DTADDRESS_WRAP; }
			else if (strstr(av, "MIRROR"    )) { *States++ = D3DSAMP_ADDRESSV; *States++ = D3DTADDRESS_MIRROR; }
			else if (strstr(av, "CLAMP"     )) { *States++ = D3DSAMP_ADDRESSV; *States++ = D3DTADDRESS_CLAMP; }
			else if (strstr(av, "BORDER"    )) { *States++ = D3DSAMP_ADDRESSV; *States++ = D3DTADDRESS_BORDER; }
			else if (strstr(av, "MIRRORONCE")) { *States++ = D3DSAMP_ADDRESSV; *States++ = D3DTADDRESS_MIRRORONCE; }
		  }
		  if (aw) {
			char *end = (char *)strchr(aw, ';'); if (end) *end = '\0';
			/**/ if (strstr(aw, "WRAP"      )) { *States++ = D3DSAMP_ADDRESSW; *States++ = D3DTADDRESS_WRAP; }
			else if (strstr(aw, "MIRROR"    )) { *States++ = D3DSAMP_ADDRESSW; *States++ = D3DTADDRESS_MIRROR; }
			else if (strstr(aw, "CLAMP"     )) { *States++ = D3DSAMP_ADDRESSW; *States++ = D3DTADDRESS_CLAMP; }
			else if (strstr(aw, "BORDER"    )) { *States++ = D3DSAMP_ADDRESSW; *States++ = D3DTADDRESS_BORDER; }
			else if (strstr(aw, "MIRRORONCE")) { *States++ = D3DSAMP_ADDRESSW; *States++ = D3DTADDRESS_MIRRORONCE; }
		  }
		  if (mp) {
			char *end = (char *)strchr(mp, ';'); if (end) *end = '\0';
			/**/ if (strstr(mp, "NONE"         )) { *States++ = D3DSAMP_MIPFILTER; *States++ = D3DTEXF_NONE; }
			else if (strstr(mp, "POINT"        )) { *States++ = D3DSAMP_MIPFILTER; *States++ = D3DTEXF_POINT; }
			else if (strstr(mp, "LINEAR"       )) { *States++ = D3DSAMP_MIPFILTER; *States++ = D3DTEXF_LINEAR; }
			else if (strstr(mp, "ANISOTROPIC"  )) { *States++ = D3DSAMP_MIPFILTER; *States++ = D3DTEXF_ANISOTROPIC; }
			else if (strstr(mp, "PYRAMIDALQUAD")) { *States++ = D3DSAMP_MIPFILTER; *States++ = D3DTEXF_PYRAMIDALQUAD; }
			else if (strstr(mp, "GAUSSIANQUAD" )) { *States++ = D3DSAMP_MIPFILTER; *States++ = D3DTEXF_GAUSSIANQUAD; }
		  }
		  if (mn) {
			char *end = (char *)strchr(mn, ';'); if (end) *end = '\0';
			/**/ if (strstr(mn, "NONE"         )) { *States++ = D3DSAMP_MINFILTER; *States++ = D3DTEXF_NONE; }
			else if (strstr(mn, "POINT"        )) { *States++ = D3DSAMP_MINFILTER; *States++ = D3DTEXF_POINT; }
			else if (strstr(mn, "LINEAR"       )) { *States++ = D3DSAMP_MINFILTER; *States++ = D3DTEXF_LINEAR; }
			else if (strstr(mn, "ANISOTROPIC"  )) { *States++ = D3DSAMP_MINFILTER; *States++ = D3DTEXF_ANISOTROPIC; }
			else if (strstr(mn, "PYRAMIDALQUAD")) { *States++ = D3DSAMP_MINFILTER; *States++ = D3DTEXF_PYRAMIDALQUAD; }
			else if (strstr(mn, "GAUSSIANQUAD" )) { *States++ = D3DSAMP_MINFILTER; *States++ = D3DTEXF_GAUSSIANQUAD; }
		  }
		  if (mg) {
			char *end = (char *)strchr(mg, ';'); if (end) *end = '\0';
			/**/ if (strstr(mg, "NONE"         )) { *States++ = D3DSAMP_MAGFILTER; *States++ = D3DTEXF_NONE; }
			else if (strstr(mg, "POINT"        )) { *States++ = D3DSAMP_MAGFILTER; *States++ = D3DTEXF_POINT; }
			else if (strstr(mg, "LINEAR"       )) { *States++ = D3DSAMP_MAGFILTER; *States++ = D3DTEXF_LINEAR; }
			else if (strstr(mg, "ANISOTROPIC"  )) { *States++ = D3DSAMP_MAGFILTER; *States++ = D3DTEXF_ANISOTROPIC; }
			else if (strstr(mg, "PYRAMIDALQUAD")) { *States++ = D3DSAMP_MAGFILTER; *States++ = D3DTEXF_PYRAMIDALQUAD; }
			else if (strstr(mg, "GAUSSIANQUAD" )) { *States++ = D3DSAMP_MAGFILTER; *States++ = D3DTEXF_GAUSSIANQUAD; }
		  }
		  if (bc) {
			char *end = (char *)strchr(bc, ';'); if (end) *end = '\0';

			/* currently fixed */
			float col = 0.0; /*sscanf("Bordercolor = %f", &col);*/
			*States++ = D3DSAMP_BORDERCOLOR;
			*States++ = *((DWORD *)&col);
		  }
		}
      }
    }
  }

  return States;
}

void RuntimeShaderRecord::CreateCT() {

	D3DXCONSTANTTABLE_DESC desc;
	D3DXCONSTANT_DESC cnst;
	UINT count = 1;
    
    int lcls[4] = {0};
    int nums[4] = {0};
    int cnts[5] = {0};
    int size;
	
	pTable->GetDesc(&desc);
    for (int c = 0; c < desc.Constants; c++) {
		D3DXHANDLE handle = pTable->GetConstant(NULL, c);
		pTable->GetConstantDesc(handle, &cnst, &count);

		if (cnst.RegisterSet <= 4) {
			if ((cnst.Name == strstr(cnst.Name, "cust_")) || (cnst.Name == strstr(cnst.Name, "glob_")) || (cnst.Name == strstr(cnst.Name, "oblv_")))
				lcls[cnst.RegisterSet] += cnst.RegisterCount;
			if ((cnst.Name == strstr(cnst.Name, "obge_")) || (cnst.Name == strstr(cnst.Name, "oblv_")) || (cnst.Name == strstr(cnst.Name, "glob_")) || (cnst.Name == strstr(cnst.Name, "cust_")))
				nums[cnst.RegisterSet] += 1;
		}
    }

    /* check if there is anything we have to do by ourself or
    * if all the variables are handled by Oblivion
    */
    if (nums[D3DXRS_BOOL   ]) nums[D3DXRS_BOOL   ]++;
    if (nums[D3DXRS_INT4   ]) nums[D3DXRS_INT4   ]++;
    if (nums[D3DXRS_FLOAT4 ]) nums[D3DXRS_FLOAT4 ]++;
    if (nums[D3DXRS_SAMPLER]) nums[D3DXRS_SAMPLER]++;

    if (nums[D3DXRS_BOOL] + nums[D3DXRS_INT4] + nums[D3DXRS_FLOAT4] + nums[D3DXRS_SAMPLER]) {

      /* we allocate all resources in one big contiguous block */
      size = ((nums[D3DXRS_BOOL   ] + nums[D3DXRS_INT4   ] + nums[D3DXRS_FLOAT4 ] + nums[D3DXRS_SAMPLER] + nums[D3DXRS_SAMPLER]) * sizeof(struct RuntimeVariable)) +
			 ((lcls[D3DXRS_INT4   ] * sizeof(RuntimeVariable::mem::iv)) + (lcls[D3DXRS_FLOAT4 ] * sizeof(RuntimeVariable::mem::fv)) + (lcls[D3DXRS_SAMPLER] * sizeof(RuntimeVariable::mem::tv) * 16));

      pCustomCT = calloc(size, 1);
      pBool    = (RuntimeVariable *)pCustomCT;
      pInt4    = pBool    + nums[D3DXRS_BOOL   ];
      pFloat4  = pInt4    + nums[D3DXRS_INT4   ];
      pTexture = pFloat4  + nums[D3DXRS_FLOAT4 ];
      pSampler = pTexture + nums[D3DXRS_SAMPLER];
      RuntimeVariable::mem::iv *ivs = (RuntimeVariable::mem::iv *)(pSampler + nums[D3DXRS_SAMPLER]);
      RuntimeVariable::mem::fv *fvs = (RuntimeVariable::mem::fv *)(ivs      + lcls[D3DXRS_INT4   ]);
      RuntimeVariable::mem::tv *tvs = (RuntimeVariable::mem::tv *)(fvs      + lcls[D3DXRS_FLOAT4 ]);
      assert((void *)((char *)pCustomCT + size) == (void *)(tvs      + lcls[D3DXRS_SAMPLER] * 16));

      for (int c = 0; c < desc.Constants; c++) {
		D3DXHANDLE handle = pTable->GetConstant(NULL, c);
		pTable->GetConstantDesc(handle, &cnst, &count);

		if (cnst.RegisterSet > 4)
		  continue;
		if ((cnst.Name != strstr(cnst.Name, "obge_")) && (cnst.Name != strstr(cnst.Name, "oblv_")) && (cnst.Name != strstr(cnst.Name, "glob_")) && (cnst.Name != strstr(cnst.Name, "cust_")))
		  continue;

		switch (cnst.RegisterSet) {
		  case D3DXRS_BOOL:
			/**/ if (cnst.Name == strstr(cnst.Name, "obge_"))
			  break;
			else if (cnst.Name == strstr(cnst.Name, "oblv_"))
			  break;
			else if (cnst.Name == strstr(cnst.Name, "glob_"))
			  break;
			else if (cnst.Name == strstr(cnst.Name, "cust_")) {
			  if (cnst.DefaultValue)
				pBool[cnts[D3DXRS_BOOL]].vals.condition = *((bool *)cnst.DefaultValue);
			}

			pBool[cnts[D3DXRS_BOOL]].offset = cnst.RegisterIndex;
			pBool[cnts[D3DXRS_BOOL]].length = cnst.RegisterCount;
			pBool[cnts[D3DXRS_BOOL]].name = cnst.Name;
				  cnts[D3DXRS_BOOL]++;
			break;
		  case D3DXRS_INT4:
			/**/ if (cnst.Name == strstr(cnst.Name, "obge_"))
			  break;
			else if (cnst.Name == strstr(cnst.Name, "oblv_"))
			  break;
			else if (cnst.Name == strstr(cnst.Name, "glob_"))
			  break;
			else if (cnst.Name == strstr(cnst.Name, "cust_")) {
			  if (cnst.DefaultValue)
				memcpy(ivs, cnst.DefaultValue, cnst.RegisterCount * sizeof(int) * 4);
			  pInt4[cnts[D3DXRS_INT4]].vals.integer = ivs; ivs += cnst.RegisterCount;
			}

			pInt4[cnts[D3DXRS_INT4]].offset = cnst.RegisterIndex;
			pInt4[cnts[D3DXRS_INT4]].length = cnst.RegisterCount;
			pInt4[cnts[D3DXRS_INT4]].name = cnst.Name;
				  cnts[D3DXRS_INT4]++;
			break;
		  case D3DXRS_FLOAT4:
			if (cnst.Name == strstr(cnst.Name, "obge_")) {
			  if (cnst.Name == strstr(cnst.Name, "obge_Tick"))
				pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.TikTiming;
			  else if (cnst.Name == strstr(cnst.Name, "obge_ToneMapping"))
				pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.HDR_ToneMapping;
			  else if (cnst.Name == strstr(cnst.Name, "obge_ParallaxData"))
				pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.POM_ParallaxData;
			  else if (cnst.Name == strstr(cnst.Name, "obge_GrassScale"))
				pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Grass_Scale;
			  else if (cnst.Name == strstr(cnst.Name, "obge_TerrainData"))
				pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Terrain_Data;
			  else if (cnst.Name == strstr(cnst.Name, "obge_SkinData"))
				pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Skin_SkinData;
			  else if (cnst.Name == strstr(cnst.Name, "obge_SkinColor"))
				pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Skin_SkinColor;
			  else if (cnst.Name == strstr(cnst.Name, "obge_ShadowsData"))
				  pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Shadows_Data;
			  else if (cnst.Name == strstr(cnst.Name, "obge_PrecipitationsData"))
				  pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Precipitations_Data;
			  else
				break;
			}
			else if (cnst.Name == strstr(cnst.Name, "oblv_")) {
				  if (cnst.Name == strstr(cnst.Name, "oblv_WorldTransform_CURRENTPASS"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheRenderManager->worldMatrix;
				  else if (cnst.Name == strstr(cnst.Name, "oblv_ViewTransform_CURRENTPASS"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheRenderManager->viewMatrix;
				  else if (cnst.Name == strstr(cnst.Name, "oblv_ProjectionTransform_CURRENTPASS"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheRenderManager->projMatrix;
				  else if (cnst.Name == strstr(cnst.Name, "oblv_ReciprocalResolution_CURRENTPASS"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.rcpres;
				  else if (cnst.Name == strstr(cnst.Name, "oblv_ReciprocalResolution_WATERHEIGHTMAPPASS"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.rcpresh;
				  else if (cnst.Name == strstr(cnst.Name, "oblv_ReciprocalResolution_WATERDISPLACEMENTPASS"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.rcpresd;
				  else if (cnst.Name == strstr(cnst.Name, "oblv_CameraForward"))
					  pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheRenderManager->CameraForward;
				  else if (cnst.Name == strstr(cnst.Name, "oblv_CameraPosition"))
					  pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheRenderManager->CameraPosition;
				  else if (cnst.Name == strstr(cnst.Name, "oblv_SunDirection"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.SunDir;
				  else if (cnst.Name == strstr(cnst.Name, "oblv_SunTiming"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.SunTiming;
				  else if  (cnst.Name == strstr(cnst.Name, "oblv_GameTime"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.GameTime;
				  else if  (cnst.Name == strstr(cnst.Name, "oblv_TexData0")) {
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.TextureData;
				  }
				  else
					break;
			}
			else if (cnst.Name == strstr(cnst.Name, "glob_")) {
				if (cnst.Name == strstr(cnst.Name, "glob_waveSpeed"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Water_waveSpeed;
				else if (cnst.Name == strstr(cnst.Name, "glob_waterCoefficients"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Water_waterCoefficients;
				else if (cnst.Name == strstr(cnst.Name, "glob_waveParams"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Water_waveParams;
				else if (cnst.Name == strstr(cnst.Name, "glob_waterVolume"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Water_waterVolume;
				else if (cnst.Name == strstr(cnst.Name, "glob_foamAndLODSettings"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.Water_foamAndLODSettings;
				else if (cnst.Name == strstr(cnst.Name, "glob_fogColor"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.fogColor;
				else if (cnst.Name == strstr(cnst.Name, "glob_sunColor"))
					pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = (RuntimeVariable::mem::fv *)&TheShaderManager->ShaderConst.sunColor;
				else
					break;
			}
			else if (cnst.Name == strstr(cnst.Name, "cust_")) {
			  if (cnst.DefaultValue)
				memcpy(fvs, cnst.DefaultValue, cnst.RegisterCount * sizeof(float) * 4);
			  pFloat4[cnts[D3DXRS_FLOAT4]].vals.floating = fvs; fvs += cnst.RegisterCount;
			}

			pFloat4[cnts[D3DXRS_FLOAT4]].offset = cnst.RegisterIndex;
			pFloat4[cnts[D3DXRS_FLOAT4]].length = cnst.RegisterCount;
			pFloat4[cnts[D3DXRS_FLOAT4]].name = cnst.Name;
					cnts[D3DXRS_FLOAT4]++;
 			break;
		  case D3DXRS_SAMPLER:
			if (cnst.Name == strstr(cnst.Name, "oblv_")) {
			  if (cnst.Name == strstr(cnst.Name, "oblv_CurrRendertarget0_CURRENTPASS")) {
				pCopyRT = (IDirect3DTexture9 **)&pTexture[cnts[D3DXRS_SAMPLER]].vals.texture;
				(*pCopyRT) = TheShaderManager->RenderTexture;
			  }
			  else if (cnst.Name == strstr(cnst.Name, "oblv_CurrDepthStenzilZ_CURRENTPASS")) {
				pCopyDS = (IDirect3DTexture9 **)&pTexture[cnts[D3DXRS_SAMPLER]].vals.texture;
				(*pCopyDS) = TheRenderManager->DepthTexture;
			  }
			  else
			  break;
			}
			else if ((cnst.Name == strstr(cnst.Name, "glob_")) || (cnst.Name == strstr(cnst.Name, "cust_"))) {
				int tnm = 0;
				int sts = (RuntimeVariable::mem::tv *)LoadTexture(cnst.Name,(int *)&tnm,(DWORD *)tvs) - tvs;

				  /* specific sampler-states have been defined */
				  if (tvs->Type) {
					assert(sts < 16);

					pSampler[cnts[D3DXRS_SAMPLER + 1]].vals.state = tvs; tvs += sts + 1;
					pSampler[cnts[D3DXRS_SAMPLER + 1]].offset = cnst.RegisterIndex;
					pSampler[cnts[D3DXRS_SAMPLER + 1]].length = cnst.RegisterCount;
					pSampler[cnts[D3DXRS_SAMPLER + 1]].name = cnst.Name;
					cnts[D3DXRS_SAMPLER + 1]++;
				  }

				  pTexture[cnts[D3DXRS_SAMPLER]].vals.texture = TheTextureManager->GetTexture(tnm)->GetTexture();
			}

			pTexture[cnts[D3DXRS_SAMPLER]].offset = cnst.RegisterIndex;
			pTexture[cnts[D3DXRS_SAMPLER]].length = cnst.RegisterCount;
			pTexture[cnts[D3DXRS_SAMPLER]].name = cnst.Name;
			cnts[D3DXRS_SAMPLER]++;
			break;
		}
      }

      assert(cnts[D3DXRS_BOOL	    ] <= nums[D3DXRS_BOOL	]);
      assert(cnts[D3DXRS_INT4	    ] <= nums[D3DXRS_INT4	]);
      assert(cnts[D3DXRS_FLOAT4	    ] <= nums[D3DXRS_FLOAT4	]);
      assert(cnts[D3DXRS_SAMPLER    ] <= nums[D3DXRS_SAMPLER    ]);
      assert(cnts[D3DXRS_SAMPLER + 1] <= nums[D3DXRS_SAMPLER    ]);

      if (!cnts[D3DXRS_BOOL	  ]) pBool    = NULL;
      if (!cnts[D3DXRS_INT4	  ]) pInt4    = NULL;
      if (!cnts[D3DXRS_FLOAT4	  ]) pFloat4  = NULL;
      if (!cnts[D3DXRS_SAMPLER	  ]) pTexture = NULL;
      if (!cnts[D3DXRS_SAMPLER + 1]) pSampler = NULL;
  }

}

void RuntimeShaderRecord::SetCT() {

	if (pCustomCT) {
		RuntimeVariable *rV;

		if (pCopyRT && !TheShaderManager->RenderSurfaceFilled) {
			TheRenderManager->device->StretchRect(TheRenderManager->currentRTGroup->targets[0]->data->Surface, NULL, TheShaderManager->RenderSurface, NULL, D3DTEXF_NONE);
			TheShaderManager->RenderSurfaceFilled = true;
		}
		if (pCopyDS && !TheShaderManager->DepthBufferFilled) {
			TheRenderManager->ResolveDepthBuffer();
			TheShaderManager->DepthBufferFilled = true;
		}
		if (rV = pTexture) do {
				TheRenderManager->device->SetTexture(rV->offset, rV->vals.texture);
			}
			while ((++rV)->length);
		if (rV = pSampler) do {
				RuntimeVariable::mem::tv *rT;
				if (rT = rV->vals.state) do {
						TheRenderManager->device->SetSamplerState(rV->offset, rT->Type, rT->Value);
					}
					while ((++rT)->Type);
			}
			while ((++rV)->length);
		if (rV = pBool) do {
				if (ShaderType == Vertex)
					TheRenderManager->device->SetVertexShaderConstantB(rV->offset, (const BOOL *)&rV->vals.condition, rV->length);
				else
					TheRenderManager->device->SetPixelShaderConstantB(rV->offset, (const BOOL *)&rV->vals.condition, rV->length);
			}
			while ((++rV)->length);
		if (rV = pInt4) do {
				if (ShaderType == Vertex)
					TheRenderManager->device->SetVertexShaderConstantI(rV->offset, (const int *)rV->vals.integer, rV->length);
				else
					TheRenderManager->device->SetPixelShaderConstantI(rV->offset, (const int *)rV->vals.integer, rV->length);
			}
			while ((++rV)->length);
		if (rV = pFloat4) do {
				if (ShaderType == Vertex)
					TheRenderManager->device->SetVertexShaderConstantF(rV->offset, (const float *)rV->vals.floating, rV->length);
				else
					TheRenderManager->device->SetPixelShaderConstantF(rV->offset, (const float *)rV->vals.floating, rV->length);
			}
			while ((++rV)->length);
	}

}

ShaderManager::ShaderManager() {

	_MESSAGE("Starting the shaders manager...");
	LARGE_INTEGER freq;

	QueryPerformanceFrequency(&freq);
	ShaderConst.TikTiming.w = (freq.QuadPart);
	ShaderConst.rcpres.x = 1.0f / TheRenderManager->width;
	ShaderConst.rcpres.y = 1.0f / TheRenderManager->height;
	ShaderConst.rcpres.z = TheRenderManager->width / TheRenderManager->height;
	ShaderConst.rcpres.w = TheRenderManager->width * TheRenderManager->height;
	ShaderConst.rcpresh.x = 1.0f / WaterHeightMapSize;
	ShaderConst.rcpresh.y = 1.0f / WaterHeightMapSize;
	ShaderConst.rcpresh.z = WaterHeightMapSize / WaterHeightMapSize;
	ShaderConst.rcpresh.w = WaterHeightMapSize * WaterHeightMapSize;
	ShaderConst.rcpresd.x = 1.0f / WaterDisplacementMapSize;
	ShaderConst.rcpresd.y = 1.0f / WaterDisplacementMapSize;
	ShaderConst.rcpresd.z = WaterDisplacementMapSize / WaterDisplacementMapSize;
	ShaderConst.rcpresd.w = WaterDisplacementMapSize * WaterDisplacementMapSize;
	InitializeConstants();
	RenderSurfaceFilled = false;
	DepthBufferFilled = false;
	RenderTexture = NULL;
	RenderSurface = NULL;
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &RenderTexture, NULL);
	RenderTexture->GetSurfaceLevel(0, &RenderSurface);

}

void ShaderManager::InitializeConstants()
{
	ShaderConst.pWeather = NULL;
	ShaderConst.WaterLens_Percent = 0;
	ShaderConst.BloodLens_Percent = 0;
	ShaderConst.SnowAccumulation_Params.w = 0;
	ShaderConst.WetWorld_RainAmount = 0;
	ShaderConst.FrameTime = 0;
	ShaderConst.LowHF_HealthCoeff = 0;
	ShaderConst.LowHF_FatigueCoeff = 0;
}

void ShaderManager::UpdateFrameConstants() {

	LARGE_INTEGER tick;
	SettingsWaterStruct *sws = NULL;
	Sky* pSky = *(Sky**)kSky;
	NiAVObject* SunRoot = (NiAVObject*)pSky->sun->SunBillboard->m_parent;
	TESClimate* currentClimate = pSky->firstClimate;
	TESWeather* currentWeather = pSky->firstWeather;
	float weatherPercent = pSky->weatherPercent;
	bool IsThirdPersonView;

	QueryPerformanceCounter(&tick);
	time(&currentTime);
	if (TheSettingManager->SettingsMain.CameraMode)
		IsThirdPersonView = !TheRenderManager->FirstPersonView;
	else
		IsThirdPersonView = isThirdPersonView;
	TheRenderManager->GetCameraData();

	ShaderConst.currentCell = (*g_thePlayer)->parentCell;
	if (ShaderConst.currentCell) {
		ShaderConst.currentWorldSpace = ShaderConst.currentCell->worldSpace;

		ShaderConst.TikTiming.x = (float)(tick.QuadPart) * 1    * 1    / ShaderConst.TikTiming.w;
		ShaderConst.TikTiming.y = (float)(tick.QuadPart) * 1000 * 1    / ShaderConst.TikTiming.w;
		ShaderConst.TikTiming.z = (float)(tick.QuadPart) * 1000 * 1000 / ShaderConst.TikTiming.w;
	
		ShaderConst.SunTiming.x = currentClimate->sunriseBegin / 6.0f - 1.0f;
		ShaderConst.SunTiming.y = currentClimate->sunriseEnd / 6.0f;
		ShaderConst.SunTiming.z = currentClimate->sunsetBegin / 6.0f;
		ShaderConst.SunTiming.w = currentClimate->sunsetEnd / 6.0f + 1.0f;

		ShaderConst.GameTime.x = TheUtilityManager->GetGameTime();
		ShaderConst.GameTime.y = ShaderConst.GameTime.x / 3600.0f;
		ShaderConst.GameTime.z = ((int)ShaderConst.GameTime.x / 60) % 60;
		ShaderConst.GameTime.w = ((int)ShaderConst.GameTime.x) % 60;
	
		float deltaz = ShaderConst.SunDir.z;
		ShaderConst.SunDir.x = SunRoot->m_localTransform.pos.x;
		ShaderConst.SunDir.y = SunRoot->m_localTransform.pos.y;
		ShaderConst.SunDir.z = SunRoot->m_localTransform.pos.z;
		TheUtilityManager->NormalizeVector3(ShaderConst.SunDir);

		if (ShaderConst.GameTime.y > ShaderConst.SunTiming.w || ShaderConst.GameTime.y < ShaderConst.SunTiming.x)
			ShaderConst.SunDir.z = -ShaderConst.SunDir.z;
		else if (ShaderConst.GameTime.y > ShaderConst.SunTiming.z && fabs(deltaz) - ShaderConst.SunDir.z <= 0.0f)
			ShaderConst.SunDir.z = -ShaderConst.SunDir.z;
		else if (ShaderConst.GameTime.y < ShaderConst.SunTiming.y && fabs(deltaz) - ShaderConst.SunDir.z >= 0.0f)
			ShaderConst.SunDir.z = -ShaderConst.SunDir.z;

		if (ShaderConst.currentWorldSpace) {
			if (currentWeather) {
				ShaderConst.SunDir.w = 1.0f;
				if (ShaderConst.GameTime.y >= ShaderConst.SunTiming.y && ShaderConst.GameTime.y <= ShaderConst.SunTiming.z) {
					ShaderConst.SunAmount.x = 0.0f;
					ShaderConst.SunAmount.y = 1.0f;
					ShaderConst.SunAmount.z = 0.0f;
					ShaderConst.SunAmount.w = 0.0f;
				}
				else if ((ShaderConst.GameTime.y >= ShaderConst.SunTiming.w && ShaderConst.GameTime.y <= 23.59) || (ShaderConst.GameTime.y >= 0 && ShaderConst.GameTime.y <= ShaderConst.SunTiming.x)) {
					ShaderConst.SunAmount.x = 0.0f;
					ShaderConst.SunAmount.y = 0.0f;
					ShaderConst.SunAmount.z = 0.0f;
					ShaderConst.SunAmount.w = 1.0f;
				}
				else if (ShaderConst.GameTime.y >= ShaderConst.SunTiming.x && ShaderConst.GameTime.y <= ShaderConst.SunTiming.y) {
					if ((ShaderConst.GameTime.y - ShaderConst.SunTiming.x) * 2 <= ShaderConst.SunTiming.y - ShaderConst.SunTiming.x) { 
						ShaderConst.SunAmount.x = (ShaderConst.GameTime.y - ShaderConst.SunTiming.x ) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x);
						ShaderConst.SunAmount.y = 0.0f;
						ShaderConst.SunAmount.z = 0.0f;
						ShaderConst.SunAmount.w = 1.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.x ) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x);
					}
					else {
						ShaderConst.SunAmount.x = 2.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.x) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x);
						ShaderConst.SunAmount.y = (ShaderConst.GameTime.y - ShaderConst.SunTiming.x) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x) - 1.0f;
						ShaderConst.SunAmount.z = 0.0f;
						ShaderConst.SunAmount.w = 0.0f;
					}
				}
				else if (ShaderConst.GameTime.y >= ShaderConst.SunTiming.z && ShaderConst.GameTime.y <= ShaderConst.SunTiming.w) {
					if ((ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 <= ShaderConst.SunTiming.w - ShaderConst.SunTiming.z) {
						ShaderConst.SunAmount.x = 0.0f;
						ShaderConst.SunAmount.y = 1.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z);
						ShaderConst.SunAmount.z = (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z);
						ShaderConst.SunAmount.w = 0.0f;
					}
					else {
						ShaderConst.SunAmount.x = 0.0f;
						ShaderConst.SunAmount.y = 0.0f;
						ShaderConst.SunAmount.z = 2.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z);
						ShaderConst.SunAmount.w = (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z) - 1.0f;
					}
				}

				if (ShaderConst.pWeather == NULL) ShaderConst.pWeather = currentWeather;

				for (int t = TESWeather::eTime_Sunrise; t <= TESWeather::eTime_Night ; t++) {
					TESWeather::RGBA color = currentWeather->colors[TESWeather::eColor_Fog].colors[t];
					switch (t)
					{
						case TESWeather::eTime_Sunrise:
							ShaderConst.fogColor.x = color.r / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.fogColor.y = color.g / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.fogColor.z = color.b / 255.0f * ShaderConst.SunAmount.x;
							break;
						case TESWeather::eTime_Day:
							ShaderConst.fogColor.x += color.r / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.fogColor.y += color.g / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.fogColor.z += color.b / 255.0f * ShaderConst.SunAmount.y;
							break;
						case TESWeather::eTime_Sunset:
							ShaderConst.fogColor.x += color.r / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.fogColor.y += color.g / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.fogColor.z += color.b / 255.0f * ShaderConst.SunAmount.z;
							break;
						case TESWeather::eTime_Night:
							ShaderConst.fogColor.x += color.r / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.fogColor.y += color.g / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.fogColor.z += color.b / 255.0f * ShaderConst.SunAmount.w;
							break;
					}
				}
				for (int t = TESWeather::eTime_Sunrise; t <= TESWeather::eTime_Night ; t++) {
					TESWeather::RGBA color = ShaderConst.pWeather->colors[TESWeather::eColor_Fog].colors[t];
					switch (t)
					{
						case TESWeather::eTime_Sunrise:
							ShaderConst.oldfogColor.x = color.r / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.oldfogColor.y = color.g / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.oldfogColor.z = color.b / 255.0f * ShaderConst.SunAmount.x;
							break;
						case TESWeather::eTime_Day:
							ShaderConst.oldfogColor.x += color.r / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.oldfogColor.y += color.g / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.oldfogColor.z += color.b / 255.0f * ShaderConst.SunAmount.y;
							break;
						case TESWeather::eTime_Sunset:
							ShaderConst.oldfogColor.x += color.r / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.oldfogColor.y += color.g / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.oldfogColor.z += color.b / 255.0f * ShaderConst.SunAmount.z;
							break;
						case TESWeather::eTime_Night:
							ShaderConst.oldfogColor.x += color.r / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.oldfogColor.y += color.g / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.oldfogColor.z += color.b / 255.0f * ShaderConst.SunAmount.w;
							break;
					}
				}

				for (int t = TESWeather::eTime_Sunrise; t <= TESWeather::eTime_Night ; t++) {
					TESWeather::RGBA color = currentWeather->colors[TESWeather::eColor_Sun].colors[t];
					switch (t)
					{
						case TESWeather::eTime_Sunrise:
							ShaderConst.sunColor.x = color.r / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.sunColor.y = color.g / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.sunColor.z = color.b / 255.0f * ShaderConst.SunAmount.x;
							break;
						case TESWeather::eTime_Day:
							ShaderConst.sunColor.x += color.r / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.sunColor.y += color.g / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.sunColor.z += color.b / 255.0f * ShaderConst.SunAmount.y;
							break;
						case TESWeather::eTime_Sunset:
							ShaderConst.sunColor.x += color.r / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.sunColor.y += color.g / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.sunColor.z += color.b / 255.0f * ShaderConst.SunAmount.z;
							break;
						case TESWeather::eTime_Night:
							ShaderConst.sunColor.x += color.r / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.sunColor.y += color.g / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.sunColor.z += color.b / 255.0f * ShaderConst.SunAmount.w;
							break;
					}
				}
				for (int t = TESWeather::eTime_Sunrise; t <= TESWeather::eTime_Night ; t++) {
					TESWeather::RGBA color = ShaderConst.pWeather->colors[TESWeather::eColor_Sun].colors[t];
					switch (t)
					{
						case TESWeather::eTime_Sunrise:
							ShaderConst.oldsunColor.x = color.r / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.oldsunColor.y = color.g / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.oldsunColor.z = color.b / 255.0f * ShaderConst.SunAmount.x;
							break;
						case TESWeather::eTime_Day:
							ShaderConst.oldsunColor.x += color.r / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.oldsunColor.y += color.g / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.oldsunColor.z += color.b / 255.0f * ShaderConst.SunAmount.y;
							break;
						case TESWeather::eTime_Sunset:
							ShaderConst.oldsunColor.x += color.r / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.oldsunColor.y += color.g / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.oldsunColor.z += color.b / 255.0f * ShaderConst.SunAmount.z;
							break;
						case TESWeather::eTime_Night:
							ShaderConst.oldsunColor.x += color.r / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.oldsunColor.y += color.g / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.oldsunColor.z += color.b / 255.0f * ShaderConst.SunAmount.w;
							break;
					}
				}

				if (ShaderConst.SunAmount.w == 1.0f) {
					ShaderConst.currentfogStart = currentWeather->fogNight_nearFog;
					ShaderConst.currentfogEnd = currentWeather->fogNight_farFog;
					ShaderConst.oldfogStart = ShaderConst.pWeather->fogNight_nearFog;
					ShaderConst.oldfogEnd = ShaderConst.pWeather->fogNight_farFog;
				}
				else {
					ShaderConst.currentfogStart = currentWeather->fogDay_nearFog;
					ShaderConst.currentfogEnd = currentWeather->fogDay_farFog;
					ShaderConst.oldfogStart = ShaderConst.pWeather->fogDay_nearFog;
					ShaderConst.oldfogEnd = ShaderConst.pWeather->fogDay_farFog;
				}

				ShaderConst.oldsunGlare = ShaderConst.pWeather->sunGlare;
				ShaderConst.oldwindSpeed = ShaderConst.pWeather->windSpeed;
				ShaderConst.currentsunGlare = (ShaderConst.oldsunGlare - ((ShaderConst.oldsunGlare - currentWeather->sunGlare) * weatherPercent)) / 255.0f;
				ShaderConst.currentwindSpeed = (ShaderConst.oldwindSpeed - ((ShaderConst.oldwindSpeed - currentWeather->windSpeed) * weatherPercent)) / 255.0f;

				ShaderConst.fogColor.x = ShaderConst.oldfogColor.x - ((ShaderConst.oldfogColor.x - ShaderConst.fogColor.x) * weatherPercent);
				ShaderConst.fogColor.y = ShaderConst.oldfogColor.y - ((ShaderConst.oldfogColor.y - ShaderConst.fogColor.y) * weatherPercent);
				ShaderConst.fogColor.z = ShaderConst.oldfogColor.z - ((ShaderConst.oldfogColor.z - ShaderConst.fogColor.z) * weatherPercent);
				ShaderConst.fogColor.w = 1.0f;

				ShaderConst.sunColor.x = ShaderConst.oldsunColor.x - ((ShaderConst.oldsunColor.x - ShaderConst.sunColor.x) * weatherPercent);
				ShaderConst.sunColor.y = ShaderConst.oldsunColor.y - ((ShaderConst.oldsunColor.y - ShaderConst.sunColor.y) * weatherPercent);
				ShaderConst.sunColor.z = ShaderConst.oldsunColor.z - ((ShaderConst.oldsunColor.z - ShaderConst.sunColor.z) * weatherPercent);
				ShaderConst.sunColor.w = ShaderConst.SunAmount.w;

				ShaderConst.fogStart = ShaderConst.oldfogStart - ((ShaderConst.oldfogStart - ShaderConst.currentfogStart) * weatherPercent);
				ShaderConst.fogEnd = ShaderConst.oldfogEnd - ((ShaderConst.oldfogEnd - ShaderConst.currentfogEnd) * weatherPercent);

				if (weatherPercent == 1.0f) ShaderConst.pWeather = currentWeather;
			}
		}
		else {
			ShaderConst.SunDir.w = 0.0f;
			TESObjectCELL::LightingData* LightData = ShaderConst.currentCell->lighting;
			ShaderConst.fogColor.x = LightData->fog.r / 255.0f;
			ShaderConst.fogColor.y = LightData->fog.g / 255.0f;
			ShaderConst.fogColor.z = LightData->fog.b / 255.0f;
			ShaderConst.fogColor.w = 1.0f;

			ShaderConst.sunColor.x = LightData->ambient.r / 255.0f;
			ShaderConst.sunColor.y = LightData->ambient.g / 255.0f;
			ShaderConst.sunColor.z = LightData->ambient.b / 255.0f;
			ShaderConst.sunColor.w = ShaderConst.SunAmount.w;

			ShaderConst.fogStart = LightData->fogNear;
			ShaderConst.fogEnd = LightData->fogFar;

			ShaderConst.currentsunGlare = 0.5f;
		}
		
		if (CurrentBlend == 0.25f)
			sws = TheSettingManager->GetSettingsWater("Blood");
		else if (CurrentBlend == 0.50f)
			sws = TheSettingManager->GetSettingsWater("Lava");
		else
			if (!(sws = TheSettingManager->GetSettingsWater(ShaderConst.currentCell->GetEditorName())))
				if (ShaderConst.currentWorldSpace) sws = TheSettingManager->GetSettingsWater(ShaderConst.currentWorldSpace->GetEditorName());

		if (sws == NULL) sws = TheSettingManager->GetSettingsWater("Default");

		if (TheSettingManager->SettingsMain.EnableWater || TheSettingManager->SettingsMain.EnableUnderwater) {
			ShaderConst.Water_waterCoefficients.x = sws->inExtCoeff_R;
			ShaderConst.Water_waterCoefficients.y = sws->inExtCoeff_G;
			ShaderConst.Water_waterCoefficients.z = sws->inExtCoeff_B;
			ShaderConst.Water_waterCoefficients.w = sws->inScattCoeff;

			ShaderConst.Water_waveParams.x = sws->choppiness;
			ShaderConst.Water_waveParams.y = sws->waveWidth;
			ShaderConst.Water_waveParams.w = sws->reflectivity;

			ShaderConst.Water_foamAndLODSettings.z = sws->LODdistance;
			ShaderConst.Water_foamAndLODSettings.w = sws->MinLOD;

			ShaderConst.Water_waterVolume.x = sws->causticsStrength * ShaderConst.currentsunGlare;
			ShaderConst.Water_waterVolume.y = sws->shoreFactor;
			ShaderConst.Water_waterVolume.z = sws->turbidity;
			ShaderConst.Water_waterVolume.w = sws->causticsStrengthS;

			ShaderConst.Water_depthDarkness = sws->depthDarkness;
			ShaderConst.HasWater = ShaderConst.currentCell->flags0 & TESObjectCELL::kFlags0_HasWater;
			ShaderConst.WaterHeight = WaterLevel;
		}		

		if (TheSettingManager->SettingsMain.EnableSnowAccumulation) {
			if (currentWeather->precipType == 8) {
				if (ShaderConst.SnowAccumulation_Params.w < TheSettingManager->SettingsPrecipitations.SnowAmount)
					ShaderConst.SnowAccumulation_Params.w = ShaderConst.SnowAccumulation_Params.w + TheSettingManager->SettingsPrecipitations.SnowIncrease;
				if (sws->IceEnabled)
					if (ShaderConst.currentWorldSpace && ShaderConst.SnowAccumulation_Params.w >= TheSettingManager->SettingsPrecipitations.SnowAmount)
						ShaderConst.Water_waveSpeed.x = 0.0f;
			}
			else {
				if (ShaderConst.SnowAccumulation_Params.w > 0.0f)
					ShaderConst.SnowAccumulation_Params.w = ShaderConst.SnowAccumulation_Params.w - TheSettingManager->SettingsPrecipitations.SnowDecrease;
				if (ShaderConst.SnowAccumulation_Params.w < 0.0f)
					ShaderConst.SnowAccumulation_Params.w = 0.0f;
				if (ShaderConst.SnowAccumulation_Params.w == 0.0f)
					ShaderConst.Water_waveSpeed.x = sws->waveSpeed;
			}
			if (ShaderConst.Water_waveSpeed.x == 0.0f) {
				ShaderConst.Water_waterCoefficients.x = sws->IceinExtCoeff_R;
				ShaderConst.Water_waterCoefficients.y = sws->IceinExtCoeff_G;
				ShaderConst.Water_waterCoefficients.z = sws->IceinExtCoeff_B;
				ShaderConst.Water_waveParams.x = sws->Icechoppiness;
				ShaderConst.Water_waveParams.y = sws->IcewaveWidth;
				ShaderConst.Water_waveParams.w = sws->Icereflectivity;
				ShaderConst.Water_waterVolume.y = sws->IceshoreFactor;
				ShaderConst.Water_waterVolume.z = sws->Iceturbidity;
			}
			ShaderConst.SnowAccumulation_Params.x = TheSettingManager->SettingsPrecipitations.SnowBlurNormDropThreshhold;
			ShaderConst.SnowAccumulation_Params.y = TheSettingManager->SettingsPrecipitations.SnowBlurRadiusMultiplier;
			ShaderConst.SnowAccumulation_Params.z = TheSettingManager->SettingsPrecipitations.SnowSunPower;
		}
		else
			ShaderConst.Water_waveSpeed.x = sws->waveSpeed;

		if (TheSettingManager->SettingsMain.EnableWetWorld) {
			if (currentWeather->precipType == 4) {
				ShaderConst.WetWorld_RippleWeight = 1.0f;
				if (ShaderConst.WetWorld_RainAmount < TheSettingManager->SettingsPrecipitations.RainAmount)
					ShaderConst.WetWorld_RainAmount = ShaderConst.WetWorld_RainAmount + TheSettingManager->SettingsPrecipitations.RainIncrease;
			}
			else {
				ShaderConst.WetWorld_RippleWeight = 0.3f - weatherPercent;
				if (ShaderConst.WetWorld_RippleWeight <= 0.0f)
					ShaderConst.WetWorld_RippleWeight = 0.05f;
				if (ShaderConst.WetWorld_RainAmount > 0.0f)
					ShaderConst.WetWorld_RainAmount = ShaderConst.WetWorld_RainAmount - TheSettingManager->SettingsPrecipitations.RainDecrease;
				if (ShaderConst.WetWorld_RainAmount < 0.0f)
					ShaderConst.WetWorld_RainAmount = 0.0f;
			}
			ShaderConst.WetWorld_Coeffs.x = TheSettingManager->SettingsPrecipitations.PuddleCoeff_R;
			ShaderConst.WetWorld_Coeffs.y = TheSettingManager->SettingsPrecipitations.PuddleCoeff_G;
			ShaderConst.WetWorld_Coeffs.z = TheSettingManager->SettingsPrecipitations.PuddleCoeff_B;
			ShaderConst.WetWorld_Coeffs.w = TheSettingManager->SettingsPrecipitations.PuddleSpecularMultiplier;
		}

		if (TheSettingManager->SettingsMain.EnableWaterLens) {
			ShaderConst.WaterLens_Time.x = sws->LensTimeMultA;
			ShaderConst.WaterLens_Time.y = sws->LensTimeMultB;
			ShaderConst.WaterLens_Time.z = sws->LensTime;
			if (ShaderConst.WaterLens_Percent == -1.0f) {
				ShaderConst.WaterLens_Time.w = 0.0f;
				ShaderConst.WaterLens_Amount = sws->LensAmount;
			}
			else if (ShaderConst.WaterLens_Percent > 0.0f) {
				ShaderConst.WaterLens_Time.w += 1.0f;
				ShaderConst.WaterLens_Percent = 1.0f - ShaderConst.WaterLens_Time.w / ShaderConst.WaterLens_Time.z;
				if (ShaderConst.WaterLens_Percent < 0.0f)
					ShaderConst.WaterLens_Percent = 0.0f;
				ShaderConst.WaterLens_Amount = sws->LensAmount * ShaderConst.WaterLens_Percent;
			}
			ShaderConst.WaterLens_Viscosity = sws->LensViscosity;
		}

		if (TheSettingManager->SettingsMain.EnableGrass) {
			ShaderConst.Grass_Scale.x = TheSettingManager->SettingsGrass.ScaleX;
			ShaderConst.Grass_Scale.y = TheSettingManager->SettingsGrass.ScaleY;
			ShaderConst.Grass_Scale.z = TheSettingManager->SettingsGrass.ScaleZ;
			ShaderConst.Grass_Scale.w = TheSettingManager->SettingsGrass.MinHeight;
			switch (TheSettingManager->SettingsGrass.GrassDensity)
			{
				case 1:
					*SettingMinGrassSize = 240;
					*SettingTexturePctThreshold = 0.3f;
					break;
				case 2:
					*SettingMinGrassSize = 240;
					*SettingTexturePctThreshold = 0.2f;
					break;
				case 3:
					*SettingMinGrassSize = 240;
					*SettingTexturePctThreshold = 0.1f;
					break;
				case 4:
					*SettingMinGrassSize = 240;
					*SettingTexturePctThreshold = 0.0f;
					break;
				case 5:
					*SettingMinGrassSize = 120;
					*SettingTexturePctThreshold = 0.3f;
					break;
				case 6:
					*SettingMinGrassSize = 120;
					*SettingTexturePctThreshold = 0.2f;
					break;
				case 7:
					*SettingMinGrassSize = 120;
					*SettingTexturePctThreshold = 0.1f;
					break;
				case 8:
					*SettingMinGrassSize = 120;
					*SettingTexturePctThreshold = 0.0f;
					break;
				case 9:
					*SettingMinGrassSize = 80;
					*SettingTexturePctThreshold = 0.3f;
					break;
				case 10:
					*SettingMinGrassSize = 80;
					*SettingTexturePctThreshold = 0.2f;
					break;
				case 11:
					*SettingMinGrassSize = 80;
					*SettingTexturePctThreshold = 0.1f;
					break;
				case 12:
					*SettingMinGrassSize = 80;
					*SettingTexturePctThreshold = 0.0f;
					break;
				case 13:
					*SettingMinGrassSize = 20;
					*SettingTexturePctThreshold = 0.3f;
					break;
				case 14:
					*SettingMinGrassSize = 20;
					*SettingTexturePctThreshold = 0.2f;
					break;
				case 15:
					*SettingMinGrassSize = 20;
					*SettingTexturePctThreshold = 0.1f;
					break;
				case 16:
					*SettingMinGrassSize = 20;
					*SettingTexturePctThreshold = 0.0f;
					break;
				default:
					break;
			}
			*SettingGrassStartFadeDistance = TheSettingManager->SettingsGrass.MinDistance;
			*SettingGrassEndDistance = TheSettingManager->SettingsGrass.MaxDistance;
			if (TheSettingManager->SettingsGrass.WindEnabled) {
				*SettingGrassWindMagnitudeMax = *LocalGrassWindMagnitudeMax = TheSettingManager->SettingsGrass.WindCoefficient * ShaderConst.currentwindSpeed;
				*SettingGrassWindMagnitudeMin = *LocalGrassWindMagnitudeMin = *SettingGrassWindMagnitudeMax * 0.5f;
			}
		}

		if (TheSettingManager->SettingsMain.EnableHDR) {
			ShaderConst.HDR_ToneMapping.x = TheSettingManager->SettingsHDR.ToneMapping;
			ShaderConst.HDR_ToneMapping.y = TheSettingManager->SettingsHDR.ToneMappingBlur;
			ShaderConst.HDR_ToneMapping.z = TheSettingManager->SettingsHDR.ToneMappingColor;
		}

		if (TheSettingManager->SettingsMain.EnablePOM) {
			ShaderConst.POM_ParallaxData.x = TheSettingManager->SettingsPOM.HeightMapScale;
			ShaderConst.POM_ParallaxData.y = TheSettingManager->SettingsPOM.ShadowSoftening;
			ShaderConst.POM_ParallaxData.z = TheSettingManager->SettingsPOM.MinSamples;
			ShaderConst.POM_ParallaxData.w = TheSettingManager->SettingsPOM.MaxSamples;
		}
	
		if (TheSettingManager->SettingsMain.EnableTerrain) {
			ShaderConst.Terrain_Data.x = TheSettingManager->SettingsTerrain.DistantSpecular;
			ShaderConst.Terrain_Data.y = TheSettingManager->SettingsTerrain.DistantNoise;
			ShaderConst.Terrain_Data.z = TheSettingManager->SettingsTerrain.NearSpecular;
			ShaderConst.Terrain_Data.w = TheSettingManager->SettingsTerrain.MiddleSpecular;
		}
	
		if (TheSettingManager->SettingsMain.EnableSkin) {
			ShaderConst.Skin_SkinData.x = TheSettingManager->SettingsSkin.Attenuation;
			ShaderConst.Skin_SkinData.y = TheSettingManager->SettingsSkin.SpecularPower;
			ShaderConst.Skin_SkinData.z = TheSettingManager->SettingsSkin.MaterialThickness;
			ShaderConst.Skin_SkinData.w = TheSettingManager->SettingsSkin.RimScalar;
			ShaderConst.Skin_SkinColor.x = TheSettingManager->SettingsSkin.ExtCoeffRed;
			ShaderConst.Skin_SkinColor.y = TheSettingManager->SettingsSkin.ExtCoeffGreen;
			ShaderConst.Skin_SkinColor.z = TheSettingManager->SettingsSkin.ExtCoeffBlue;
		}
		
		if (TheSettingManager->SettingsMain.EnableShadows) {
			ShaderConst.Shadows_Data.x = TheSettingManager->SettingsShadows.ShadowIntensity;
			ShaderConst.Shadows_Data.y = TheSettingManager->SettingsShadows.SelfShadowIntensity;
		}
		
		if (TheSettingManager->SettingsMain.EnablePrecipitations) {
			ShaderConst.Precipitations_Data.x = TheSettingManager->SettingsPrecipitations.Intensity;
			ShaderConst.Precipitations_Data.y = TheSettingManager->SettingsPrecipitations.Velocity;
			ShaderConst.Precipitations_Data.z = TheSettingManager->SettingsPrecipitations.Size;
		}

		if (TheSettingManager->SettingsMain.EnableGodRays) {
			ShaderConst.GodRays_LightShaftPasses = TheSettingManager->SettingsGodRays.LightShaftPasses;
			ShaderConst.GodRays_Ray.x = TheSettingManager->SettingsGodRays.RayIntensity;
			ShaderConst.GodRays_Ray.y = TheSettingManager->SettingsGodRays.RayLength;
			if (TheSettingManager->SettingsGodRays.SunGlareEnabled) {
				ShaderConst.GodRays_Ray.z = TheSettingManager->SettingsGodRays.RayDensity * ShaderConst.currentsunGlare;
				ShaderConst.GodRays_Ray.w = TheSettingManager->SettingsGodRays.RayVisibility * ShaderConst.currentsunGlare;
			}
			else {
				ShaderConst.GodRays_Ray.z = TheSettingManager->SettingsGodRays.RayDensity;
				ShaderConst.GodRays_Ray.w = TheSettingManager->SettingsGodRays.RayVisibility;
			}
			ShaderConst.GodRays_RayColor.x = TheSettingManager->SettingsGodRays.RayR;
			ShaderConst.GodRays_RayColor.y = TheSettingManager->SettingsGodRays.RayG;
			ShaderConst.GodRays_RayColor.z = TheSettingManager->SettingsGodRays.RayB;
			ShaderConst.GodRays_RayColor.w = TheSettingManager->SettingsGodRays.Saturate;
			ShaderConst.GodRays_Luminance = TheSettingManager->SettingsGodRays.Luminance;
			ShaderConst.GodRays_GlobalMultiplier = TheSettingManager->SettingsGodRays.GlobalMultiplier;
		}

		if (TheSettingManager->SettingsMain.EnableAmbientOcclusion) {
			SettingsAmbientOcclusionStruct *sas = NULL;

			if (ShaderConst.currentWorldSpace)
				sas = TheSettingManager->GetSettingsAmbientOcclusion("Exterior");
			else
				sas = TheSettingManager->GetSettingsAmbientOcclusion("Interior");

			ShaderConst.AmbientOcclusion_Enabled = sas->Enabled;
			if (ShaderConst.AmbientOcclusion_Enabled) {
				ShaderConst.AmbientOcclusion_RadiusMultiplier = sas->RadiusMultiplier;
				ShaderConst.AmbientOcclusion_StrengthMultiplier = sas->StrengthMultiplier;
				ShaderConst.AmbientOcclusion_ClampStrength = sas->ClampStrength;
				ShaderConst.AmbientOcclusion_AngleBias = sas->AngleBias;
				ShaderConst.AmbientOcclusion_Range = sas->Range;
				ShaderConst.AmbientOcclusion_LumThreshold = sas->LumThreshold * ShaderConst.SunAmount.y;
				ShaderConst.AmbientOcclusion_BlurDropThreshold = sas->BlurDropThreshold;
				ShaderConst.AmbientOcclusion_BlurRadiusMultiplier = sas->BlurRadiusMultiplier;
			}
		}

		if (TheSettingManager->SettingsMain.EnableBloom) {
			SettingsBloomStruct *sbs = NULL;
			if (!(sbs = TheSettingManager->GetSettingsBloom(ShaderConst.currentCell->GetEditorName())))
				if (ShaderConst.currentWorldSpace)
					sbs = TheSettingManager->GetSettingsBloom(ShaderConst.currentWorldSpace->GetEditorName());

			if (sbs == NULL)
				sbs = TheSettingManager->GetSettingsBloom("Default");

			ShaderConst.Bloom_BloomValues.x = sbs->BloomIntensity;
			ShaderConst.Bloom_BloomValues.y = sbs->OriginalIntensity;
			ShaderConst.Bloom_BloomValues.z = sbs->BloomSaturation;
			ShaderConst.Bloom_BloomValues.w = sbs->OriginalSaturation;
			ShaderConst.Bloom_Luminance = sbs->Luminance;
			ShaderConst.Bloom_MiddleGray = sbs->MiddleGray;
			ShaderConst.Bloom_WhiteCutOff = sbs->WhiteCutOff;
		}

		if (TheSettingManager->SettingsMain.EnableColoring) {
			SettingsColoringStruct *scs = NULL;
			if (!(scs = TheSettingManager->GetSettingsColoring(ShaderConst.currentCell->GetEditorName())))
				if (ShaderConst.currentWorldSpace)
					scs = TheSettingManager->GetSettingsColoring(ShaderConst.currentWorldSpace->GetEditorName());

			if (scs == NULL)
				scs = TheSettingManager->GetSettingsColoring("Default");

			ShaderConst.Coloring_Strength = scs->Strength;
			ShaderConst.Coloring_BaseGamma = scs->BaseGamma;
			ShaderConst.Coloring_Fade = scs->Fade;
			ShaderConst.Coloring_Contrast = scs->Contrast;
			ShaderConst.Coloring_Saturation = scs->Saturation;
			ShaderConst.Coloring_Bleach = scs->Bleach;
			ShaderConst.Coloring_BleachLuma = scs->BleachLuma;
			ShaderConst.Coloring_Linearization = scs->Linearization;
			ShaderConst.Coloring_ColorCurve.x = scs->ColorCurve;
			ShaderConst.Coloring_ColorCurve.y = scs->ColorCurveR;
			ShaderConst.Coloring_ColorCurve.z = scs->ColorCurveG;
			ShaderConst.Coloring_ColorCurve.w = scs->ColorCurveB;
			ShaderConst.Coloring_EffectGamma.x = scs->EffectGamma;
			ShaderConst.Coloring_EffectGamma.y = scs->EffectGammaR;
			ShaderConst.Coloring_EffectGamma.z = scs->EffectGammaG;
			ShaderConst.Coloring_EffectGamma.w = scs->EffectGammaB;
		}

		if (TheSettingManager->SettingsMain.EnableBloodLens) {
			if (ShaderConst.BloodLens_Percent > 0.0f) {
				ShaderConst.BloodLens_Time.z = TheSettingManager->SettingsBlood.LensTime;
				if (ShaderConst.BloodLens_Percent == 1.0f) {
					ShaderConst.BloodLens_Time.w = 0.0f;
					srand(time(NULL));
					ShaderConst.BloodLens_Params.x = (double)rand() / (RAND_MAX + 1) * (0.75f - 0.25f) + 0.25f; //from 0.25 to 0.75
					ShaderConst.BloodLens_Params.y = (double)rand() / (RAND_MAX + 1) * (0.5f + 0.1f) - 0.1f; //from -0.1 to 0.5
					ShaderConst.BloodLens_Params.z = (double)rand() / (RAND_MAX + 1) * (2.0f + 2.0f) - 2.0f; //from -2 to 2
					ShaderConst.BloodLens_Params.w = TheSettingManager->SettingsBlood.LensIntensity;
				}
				ShaderConst.BloodLens_Time.w += 1.0f;
				ShaderConst.BloodLens_Percent = 1.0f - ShaderConst.BloodLens_Time.w / ShaderConst.BloodLens_Time.z;
				if (ShaderConst.BloodLens_Percent < 0.0f)
					ShaderConst.BloodLens_Percent = 0.0f;
				ShaderConst.BloodLens_Params.w = TheSettingManager->SettingsBlood.LensIntensity * ShaderConst.BloodLens_Percent;
				ShaderConst.BloodLens_BloodColor.x = TheSettingManager->SettingsBlood.LensColorR;
				ShaderConst.BloodLens_BloodColor.y = TheSettingManager->SettingsBlood.LensColorG;
				ShaderConst.BloodLens_BloodColor.z = TheSettingManager->SettingsBlood.LensColorB;
			}
		}

		if (TheSettingManager->SettingsMain.EnableLowHF) {
			float PlayerHealthPercent = GetPlayerHealthPercent;
			float PlayerFatiguePercent = GetPlayerFatiguePercent;

			ShaderConst.LowHF_Params.x = 0;
			ShaderConst.LowHF_Params.y = 0;
			ShaderConst.LowHF_Params.z = 0;
			ShaderConst.LowHF_Params.w = 0;
			if (PlayerHealthPercent < TheSettingManager->SettingsLowHF.HealthLimit) {
				ShaderConst.LowHF_HealthCoeff = 1 - PlayerHealthPercent / TheSettingManager->SettingsLowHF.HealthLimit;
				ShaderConst.LowHF_Params.x = ShaderConst.LowHF_HealthCoeff * TheSettingManager->SettingsLowHF.LumaMultiplier;
				ShaderConst.LowHF_Params.y = ShaderConst.LowHF_HealthCoeff * 0.01f * TheSettingManager->SettingsLowHF.BlurMultiplier;
				ShaderConst.LowHF_Params.z = ShaderConst.LowHF_HealthCoeff * 20.0f * TheSettingManager->SettingsLowHF.VignetteMultiplier;
				ShaderConst.LowHF_Params.w = (1.0f - ShaderConst.LowHF_HealthCoeff) * TheSettingManager->SettingsLowHF.DarknessMultiplier;
			}
			if (!ShaderConst.LowHF_Params.x && PlayerFatiguePercent < TheSettingManager->SettingsLowHF.FatigueLimit) {
				ShaderConst.LowHF_FatigueCoeff = 1.0f - PlayerFatiguePercent / TheSettingManager->SettingsLowHF.FatigueLimit;
				ShaderConst.LowHF_Params.x = ShaderConst.LowHF_FatigueCoeff * TheSettingManager->SettingsLowHF.LumaMultiplier;
			}
		}

		if (TheSettingManager->SettingsMain.EnableDepthOfField) {
			SettingsDepthOfFieldStruct *sds = NULL;

			if (TheUtilityManager->IsVanityView())
				sds = TheSettingManager->GetSettingsDepthOfField("VanityView");
			else if (IsThirdPersonView)
				sds = TheSettingManager->GetSettingsDepthOfField("ThirdPersonView");
			else
				sds = TheSettingManager->GetSettingsDepthOfField("FirstPersonView");

			ShaderConst.DepthOfField_Enabled = sds->Enabled;
			if (ShaderConst.DepthOfField_Enabled) {
				ShaderConst.DepthOfField_DistantBlur = sds->DistantBlur;
				ShaderConst.DepthOfField_DistantBlurStartRange = sds->DistantBlurStartRange;
				ShaderConst.DepthOfField_DistantBlurEndRange = sds->DistantBlurEndRange;
				ShaderConst.DepthOfField_BaseBlurRadius = sds->BaseBlurRadius;
				ShaderConst.DepthOfField_BlurFallOff = sds->BlurFallOff;
				ShaderConst.DepthOfField_Radius = sds->Radius;
				ShaderConst.DepthOfField_DiameterRange = sds->DiameterRange;
				ShaderConst.DepthOfField_NearBlurCutOff = sds->NearBlurCutOff;
				if (sds->DialogMode) {
					if (!TheUtilityManager->IsMenuMode(1009) && !TheUtilityManager->IsMenuMode(1034))
						ShaderConst.DepthOfField_Enabled = 0;
				}
			}
		}

		if (TheSettingManager->SettingsMain.EnableCinema) {
			ShaderConst.Cinema_AspectRatio = TheSettingManager->SettingsCinema.AspectRatio;
			ShaderConst.Cinema_VignetteRadius = TheSettingManager->SettingsCinema.VignetteRadius;
			if (ShaderConst.Cinema_AspectRatio != 0.0f && TheSettingManager->SettingsCinema.DialogMode) {
				if (!TheUtilityManager->IsMenuMode(1009) && !TheUtilityManager->IsMenuMode(1034)) {
					ShaderConst.Cinema_AspectRatio = 0.0f;
					ShaderConst.Cinema_VignetteRadius = 0.0f;
				}
			}
			ShaderConst.Cinema_VignetteDarkness = TheSettingManager->SettingsCinema.VignetteDarkness;
		}

		if (TheSettingManager->SettingsMain.EnableMotionBlur) {
			SettingsMotionBlurStruct *sms = NULL;

			if (IsThirdPersonView)
				sms = TheSettingManager->GetSettingsMotionBlur("ThirdPersonView");
			else
				sms = TheSettingManager->GetSettingsMotionBlur("FirstPersonView");

			float AngleZ = TheUtilityManager->RadiansToDegrees((*g_thePlayer)->rotZ);
			float AngleX = TheUtilityManager->RadiansToDegrees((*g_thePlayer)->rotX);
			float fMotionBlurAmtX = ShaderConst.MotionBlur_oldAngleZ - AngleZ;
			float fMotionBlurAmtY = ShaderConst.MotionBlur_oldAngleX - AngleX;
			float fBlurDistScratchpad = fMotionBlurAmtX + 360.0f;
			float fBlurDistScratchpad2 = (AngleZ - ShaderConst.MotionBlur_oldAngleZ + 360.0f) * -1.0f;

			if (abs(fMotionBlurAmtX) > abs(fBlurDistScratchpad))
				fMotionBlurAmtX = fBlurDistScratchpad;
			else if (abs(fMotionBlurAmtX) > abs(fBlurDistScratchpad2))
				fMotionBlurAmtX = fBlurDistScratchpad2;

			if (pow(fMotionBlurAmtX, 2) + pow(fMotionBlurAmtY, 2) < sms->BlurCutOff) {
				fMotionBlurAmtX = 0.0f;
				fMotionBlurAmtY = 0.0f;
			}

			if (sms->Enabled) {
				ShaderConst.MotionBlur_AmountX = (ShaderConst.MotionBlur_oldoldAmountX + ShaderConst.MotionBlur_oldAmountX + fMotionBlurAmtX) / 3.0f;
				ShaderConst.MotionBlur_AmountY = (ShaderConst.MotionBlur_oldoldAmountY + ShaderConst.MotionBlur_oldAmountY + fMotionBlurAmtY) / 3.0f;
			}
			else {
				ShaderConst.MotionBlur_AmountX = 0.0f;
				ShaderConst.MotionBlur_AmountY = 0.0f;
			}
			ShaderConst.MotionBlur_oldAngleZ = AngleZ;
			ShaderConst.MotionBlur_oldAngleX = AngleX;
			ShaderConst.MotionBlur_oldoldAmountX = ShaderConst.MotionBlur_oldAmountX;
			ShaderConst.MotionBlur_oldoldAmountY = ShaderConst.MotionBlur_oldAmountY;
			ShaderConst.MotionBlur_oldAmountX = fMotionBlurAmtX;
			ShaderConst.MotionBlur_oldAmountY = fMotionBlurAmtY;
			ShaderConst.MotionBlur_BlurParams.x = sms->GaussianWeight;
			ShaderConst.MotionBlur_BlurParams.y = sms->BlurScale;
			ShaderConst.MotionBlur_BlurParams.z = sms->BlurOffsetMax;
		}

		if (TheSettingManager->SettingsMain.FrameRate) {
			int FrameRate = 1.0f / (ShaderConst.TikTiming.x - ShaderConst.FrameTime);
			TheFrameRateManager->SetFrame(FrameRate);
			ShaderConst.FrameTime = ShaderConst.TikTiming.x;
		}
	}
}

float ShaderManager::GetShaderConst(const char* Name, const char* Const) {
	// To finish
	float Value = -999999;

	if (!strcmp(Name, "AmbientOcclusion")) {

	}
	else if (!strcmp(Name, "BloodLens")) {
		if (!strcmp(Const, "BloodLens_Percent"))
			Value = ShaderConst.BloodLens_Percent;
	}
	else if (!strcmp(Name, "Bloom")) {

	}
	else if (!strcmp(Name, "Cinema")) {

	}
	else if (!strcmp(Name, "Coloring")) {

	}
	else if (!strcmp(Name, "DepthOfField")) {

	}
	else if (!strcmp(Name, "GodRays")) {

	}
	else if (!strcmp(Name, "Grass")) {

	}
	else if (!strcmp(Name, "HDR")) {

	}
	else if (!strcmp(Name, "LowHF")) {
		if (!strcmp(Const, "LowHF_HealthCoeff"))
			Value = ShaderConst.LowHF_HealthCoeff;
		else if (!strcmp(Const, "LowHF_FatigueCoeff"))
			Value = ShaderConst.LowHF_FatigueCoeff;
		else if (!strcmp(Const, "LowHF_Params.x"))
			Value = ShaderConst.LowHF_Params.x;
		else if (!strcmp(Const, "LowHF_Params.y"))
			Value = ShaderConst.LowHF_Params.y;
		else if (!strcmp(Const, "LowHF_Params.z"))
			Value = ShaderConst.LowHF_Params.z;
		else if (!strcmp(Const, "LowHF_Params.w"))
			Value = ShaderConst.LowHF_Params.w;
	}
	else if (!strcmp(Name, "MotionBlur")) {

	}
	else if (!strcmp(Name, "POM")) {

	}
	else if (!strcmp(Name, "Precipitations")) {

	}
	else if (!strcmp(Name, "Skin")) {

	}
	else if (!strcmp(Name, "SnowAccumulation")) {

	}
	else if (!strcmp(Name, "Terrain")) {

	}
	else if (!strcmp(Name, "Underwater")) {

	}
	else if (!strcmp(Name, "Water")) {
		if (!strcmp(Const, "Water_waveSpeed.x"))
			Value = ShaderConst.Water_waveSpeed.x;
		else if (!strcmp(Const, "WaterHeight"))
			Value = ShaderConst.WaterHeight;
	}
	else if (!strcmp(Name, "WaterLens")) {

	}
	else if (!strcmp(Name, "WetWorld")) {

	}
	return Value;

}

RuntimeShaderRecord* ShaderManager::LoadShader(const char* Name) {

	RuntimeShaderRecord *NewShader = new RuntimeShaderRecord();

	if (!NewShader->LoadShader(Name)) {
		delete NewShader;
		return NULL;
	}
	return NewShader;

}

void ShaderManager::BeginScene() {

	RenderSurfaceFilled = false;
	DepthBufferFilled = false;

}
