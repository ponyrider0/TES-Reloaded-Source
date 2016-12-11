#if defined(OBLIVION)
#include "obse\GameData.h"
#define SetCurrentVertexStream TheRenderManager->renderState->SetFVF(EFFECTQUADFORMAT, false)
#define kSky 0x00B365C4
#define fogNight_nearFog fogNight.nearFog
#define fogNight_farFog fogNight.farFog
#define fogDay_nearFog fogDay.nearFog
#define fogDay_farFog fogDay.farFog
#define isThirdPersonView (*g_thePlayer)->isThirdPerson
#define GetPlayerHealthPercent (float)(*g_thePlayer)->GetActorValue(kActorVal_Health) / (float)(*g_thePlayer)->GetBaseActorValue(kActorVal_Health)
#define GetPlayerFatiguePercent (float)(*g_thePlayer)->GetActorValue(kActorVal_Fatigue) / (float)(*g_thePlayer)->GetBaseActorValue(kActorVal_Fatigue)
#define CurrentBlend *WaterBlend
#define WaterLevel TheUtilityManager->GetWaterHeight(ShaderConst.currentCell)
#define DeepColorR deepColorR
#define DeepColorG deepColorG
#define DeepColorB deepColorB
#define DeepColorA deepColorA
#define ShallowColorR shallowColorR
#define ShallowColorG shallowColorG
#define ShallowColorB shallowColorB
#define ShallowColorA shallowColorA
#elif defined(SKYRIM)
#include "skse\GameData.h"
#include "skse\GameCamera.h"
#define SetCurrentVertexStream D3DDevice->SetFVF(EFFECTQUADFORMAT)
#define kSky 0x01B1160C
#define eColor_Fog kColorType_FogNear
#define eColor_Sun kColorType_Sun
#define fogNight_nearFog fogDistance.nearNight
#define fogNight_farFog fogDistance.farNight
#define fogDay_nearFog fogDistance.nearDay
#define fogDay_farFog fogDistance.farDay
#define sunGlare general.sunGlare
#define windSpeed general.windSpeed
#define precipType general.precipType
#define isThirdPersonView PlayerCamera::GetSingleton()->cameraState->stateId == PlayerCamera::kCameraState_ThirdPerson2
#define rotZ rot.z
#define rotX rot.x
#define GetPlayerHealthPercent (*g_thePlayer)->actorValueOwner.GetCurrent(24) / (*g_thePlayer)->actorValueOwner.GetBase(24)
#define GetPlayerFatiguePercent (*g_thePlayer)->actorValueOwner.GetCurrent(26) / (*g_thePlayer)->actorValueOwner.GetBase(26)
#define CurrentBlend 0.0f
#define LightingData InteriorData
#define lighting interiorData
#define WaterLevel TheUtilityManager->GetWaterHeight(*g_thePlayer)
#define DeepColorR properties.deepColorR
#define DeepColorG properties.deepColorG
#define DeepColorB properties.deepColorB
#define DeepColorA properties.deepColorA
#define ShallowColorR properties.shallowColorR
#define ShallowColorG properties.shallowColorG
#define ShallowColorB properties.shallowColorB
#define ShallowColorA properties.shallowColorA
#endif
#include <fstream>
#include <ctime>

void ShaderProgram::SetConstantTableValue(LPCSTR Name, UInt32 Index)
{
	if (!strcmp(Name, "TESR_Tick"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.TikTiming;
	else if (!strcmp(Name, "TESR_ToneMapping"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.HDR_ToneMapping;
	else if (!strcmp(Name, "TESR_ParallaxData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.POM_ParallaxData;
	else if (!strcmp(Name, "TESR_GrassScale"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Grass_Scale;
	else if (!strcmp(Name, "TESR_TerrainData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Terrain_Data;
	else if (!strcmp(Name, "TESR_SkinData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Skin_SkinData;
	else if (!strcmp(Name, "TESR_SkinColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Skin_SkinColor;
	else if (!strcmp(Name, "TESR_ShadowsData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Shadows_Data;
	else if (!strcmp(Name, "TESR_PrecipitationsData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Precipitations_Data;
	else if (!strcmp(Name, "TESR_WorldTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheRenderManager->worldMatrix;
	else if (!strcmp(Name, "TESR_ViewTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheRenderManager->viewMatrix;
	else if (!strcmp(Name, "TESR_ProjectionTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheRenderManager->projMatrix;
	else if (!strcmp(Name, "TESR_ReciprocalResolution"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ReciprocalResolution;
	else if (!strcmp(Name, "TESR_ReciprocalResolutionWater"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ReciprocalResolutionWater;
	else if (!strcmp(Name, "TESR_CameraForward"))
		FloatShaderValues[Index].Value = &TheRenderManager->CameraForward;
	else if (!strcmp(Name, "TESR_CameraPosition"))
		FloatShaderValues[Index].Value = &TheRenderManager->CameraPosition;
	else if (!strcmp(Name, "TESR_SunDirection"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SunDir;
	else if (!strcmp(Name, "TESR_SunTiming"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SunTiming;
	else if (!strcmp(Name, "TESR_SunAmount"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SunAmount;
	else if (!strcmp(Name, "TESR_GameTime"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.GameTime;
	else if (!strcmp(Name, "TESR_TextureData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.TextureData;
	else if (!strcmp(Name, "TESR_WaterCoefficients"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water_waterCoefficients;
	else if (!strcmp(Name, "TESR_WaveParams"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water_waveParams;
	else if (!strcmp(Name, "TESR_WaterVolume"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water_waterVolume;
	else if (!strcmp(Name, "TESR_WaterSettings"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water_waterSettings;
	else if (!strcmp(Name, "TESR_WaterDeepColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water_deepColor;
	else if (!strcmp(Name, "TESR_WaterShallowColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water_shallowColor;
	else if (!strcmp(Name, "TESR_FogColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.fogColor;
	else if (!strcmp(Name, "TESR_SunColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.sunColor;
	else if (!strcmp(Name, "TESR_FogData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.fogData;
	else if (!strcmp(Name, "TESR_AmbientOcclusionAOData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.AmbientOcclusion_AOData;
	else if (!strcmp(Name, "TESR_AmbientOcclusionData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.AmbientOcclusion_Data;
	else if (!strcmp(Name, "TESR_BloodLensParams"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.BloodLens_Params;
	else if (!strcmp(Name, "TESR_BloodLensColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.BloodLens_BloodColor;
	else if (!strcmp(Name, "TESR_BloomData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Bloom_BloomData;
	else if (!strcmp(Name, "TESR_BloomValues"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Bloom_BloomValues;
	else if (!strcmp(Name, "TESR_CinemaData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Cinema_Data;
	else if (!strcmp(Name, "TESR_ColoringColorCurve"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Coloring_ColorCurve;
	else if (!strcmp(Name, "TESR_ColoringEffectGamma"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Coloring_EffectGamma;
	else if (!strcmp(Name, "TESR_ColoringData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Coloring_Data;
	else if (!strcmp(Name, "TESR_ColoringValues"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Coloring_Values;
	else if (!strcmp(Name, "TESR_DepthOfFieldBlur"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.DepthOfField_Blur;
	else if (!strcmp(Name, "TESR_DepthOfFieldData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.DepthOfField_Data;
	else if (!strcmp(Name, "TESR_GodRaysRay"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.GodRays_Ray;
	else if (!strcmp(Name, "TESR_GodRaysRayColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.GodRays_RayColor;
	else if (!strcmp(Name, "TESR_GodRaysData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.GodRays_Data;
	else if (!strcmp(Name, "TESR_LowHFData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.LowHF_Data;
	else if (!strcmp(Name, "TESR_MotionBlurParams"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.MotionBlur_BlurParams;
	else if (!strcmp(Name, "TESR_MotionBlurData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.MotionBlur_Data;
	else if (!strcmp(Name, "TESR_SharpeningData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Sharpening_Data;
	else if (!strcmp(Name, "TESR_SnowAccumulationParams"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SnowAccumulation_Params;
	else if (!strcmp(Name, "TESR_WaterLensData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.WaterLens_Time;
	else if (!strcmp(Name, "TESR_WetWorldCoeffs"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.WetWorld_Coeffs;
	else if (!strcmp(Name, "TESR_WetWorldData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.WetWorld_Data;
	else {
		_MESSAGE("Custom constant found: %s", Name);
		D3DXVECTOR4 v; v.x = v.y = v.z = v.w = 0.0f;
		TheShaderManager->CustomConst[Name] = v;
		FloatShaderValues[Index].Value = &TheShaderManager->CustomConst[Name];
	}
}

ShaderRecord::ShaderRecord() {

	HasCT		= false;
	HasRB		= false;
	HasDB		= false;
	pFunction	= NULL;
	pSource		= NULL;
	pShader		= NULL;
	pTable		= NULL;
	pErrors		= NULL;
	FloatShaderValuesCount = 0;
	TextureShaderValuesCount = 0;

}

bool ShaderRecord::LoadShader(const char* Name) {
  
	char FileName[MAX_PATH];
	char FileNameBinary[MAX_PATH];

	strcpy(FileName, ShadersPath);
	if (!memcmp(Name, "WATER", 5)) {
		if (!TheSettingManager->SettingsMain.EnableWater) return false;
		strcat(FileName, "Water\\");
	}
	else if (!memcmp(Name, "GRASS", 5)) {
		if (!TheSettingManager->SettingsMain.EnableGrass) return false;
		strcat(FileName, "Grass\\");
	}
	else if (!memcmp(Name, "PRECIP", 6)) {
		if (!TheSettingManager->SettingsMain.EnablePrecipitations) return false;
		strcat(FileName, "Precipitations\\");
	}
	else if (!memcmp(Name, "HDR", 3)) {
		if (!TheSettingManager->SettingsMain.EnableHDR) return false;
		strcat(FileName, "HDR\\");
	}
	else if (!memcmp(Name, "PAR", 3)) {
		if (!TheSettingManager->SettingsMain.EnablePOM) return false;
		strcat(FileName, "POM\\");
	}
	else if (!memcmp(Name, "SKIN", 4)) {
		if (!TheSettingManager->SettingsMain.EnableSkin) return false;
		strcat(FileName, "Skin\\");
	}
	else if (!memcmp(Name, "SLS2001", 7) || !memcmp(Name, "SLS2064.vso", 11) || !memcmp(Name, "SLS2068.pso", 11) || !memcmp(Name, "SLS2042.vso", 11) || !memcmp(Name, "SLS2048.pso", 11) || !memcmp(Name, "SLS2043.vso", 11) || !memcmp(Name, "SLS2049.pso", 11)) {
		if (!TheSettingManager->SettingsMain.EnableTerrain) return false;
		strcat(FileName, "Terrain\\");
	}
	else if (!memcmp(Name, "GDECALS.vso", 11) || !memcmp(Name, "GDECAL.pso", 10) || !memcmp(Name, "SLS2040.vso", 11) || !memcmp(Name, "SLS2046.pso", 11)) {
		if (!TheSettingManager->SettingsMain.EnableBlood) return false;
		strcat(FileName, "Blood\\");
	}
	else if (!memcmp(Name, "SLS2073.vso", 11) || !memcmp(Name, "SLS2074.vso", 11) || !memcmp(Name, "SLS2075.vso", 11) || !memcmp(Name, "SLS2080.pso", 11) || !memcmp(Name, "SLS2081.pso", 11)) {
		if (!TheSettingManager->SettingsMain.EnableShadows) return false;
		strcat(FileName, "Shadows\\");
	}
	else if (!memcmp(Name, "NIGHTEYE", 8)) {
		if (!TheSettingManager->SettingsMain.EnableNightEye) return false;
		strcat(FileName, "NightEye\\");
	}
	else
		FileName[0] = NULL;

	if (FileName[0]) {
		strcat(FileName, Name);
		strcpy(FileNameBinary, FileName);
		strcat(FileName, ".hlsl");
		std::ifstream FileSource(FileName, std::ios::in | std::ios::binary | std::ios::ate);
		if (FileSource.is_open()) {
			std::streampos size = FileSource.tellg();
			pSource = new char [size];
			FileSource.seekg(0, std::ios::beg);
			FileSource.read(pSource, size);
			FileSource.close();
			if (strstr(Name, ".vso"))
				Type = ShaderType_Vertex;
			else if (strstr(Name, ".pso"))
				Type = ShaderType_Pixel;
			if (TheSettingManager->SettingsMain.DevelopCompileShaders) {
				D3DXCompileShaderFromFileA(FileName, NULL, NULL, "main", (Type == ShaderType_Vertex ? "vs_3_0" : "ps_3_0"), NULL, &pShader, &pErrors, &pTable);
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
				std::ifstream FileBinary(FileNameBinary, std::ios::in | std::ios::binary | std::ios::ate);
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
					_MESSAGE("ERROR: Shader %s not found. Try to enable the CompileShader option to recompile the shaders.", FileNameBinary);
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

void ShaderRecord::CreateCT() {

	D3DXCONSTANTTABLE_DESC ConstantTableDesc;
	D3DXCONSTANT_DESC ConstantDesc;
	D3DXHANDLE Handle;
	UINT ConstantCount = 1;
	UInt32 FloatIndex = 0;
	UInt32 TextureIndex = 0;
	
	pTable->GetDesc(&ConstantTableDesc);
    for (int c = 0; c < ConstantTableDesc.Constants; c++) {
		Handle = pTable->GetConstant(NULL, c);
		pTable->GetConstantDesc(Handle, &ConstantDesc, &ConstantCount);
		if (ConstantDesc.RegisterSet == D3DXRS_FLOAT4 && !memcmp(ConstantDesc.Name, "TESR_", 5)) FloatShaderValuesCount += 1;
		if (ConstantDesc.RegisterSet == D3DXRS_SAMPLER && !memcmp(ConstantDesc.Name, "TESR_", 5)) TextureShaderValuesCount += 1;
    }
	HasCT = FloatShaderValuesCount + TextureShaderValuesCount;
    if (HasCT) {
		FloatShaderValues = (ShaderValue*)malloc(FloatShaderValuesCount * sizeof(ShaderValue));
		TextureShaderValues = (ShaderValue*)malloc(TextureShaderValuesCount * sizeof(ShaderValue));
		for (int c = 0; c < ConstantTableDesc.Constants; c++) {
			Handle = pTable->GetConstant(NULL, c);
			pTable->GetConstantDesc(Handle, &ConstantDesc, &ConstantCount);
			if (!memcmp(ConstantDesc.Name, "TESR_", 5)) {
				switch (ConstantDesc.RegisterSet)
				{
					case D3DXRS_FLOAT4:
						SetConstantTableValue(ConstantDesc.Name, FloatIndex);
						FloatShaderValues[FloatIndex].RegisterIndex = ConstantDesc.RegisterIndex;
						FloatShaderValues[FloatIndex].RegisterCount = ConstantDesc.RegisterCount;
						FloatIndex++;
 						break;
					case D3DXRS_SAMPLER:
						if (!strcmp(ConstantDesc.Name, WordRenderedBuffer)) HasRB = true;
						if (!strcmp(ConstantDesc.Name, WordDepthBuffer)) HasDB = true;
						TextureShaderValues[TextureIndex].Texture = TheTextureManager->LoadTexture(pSource, ConstantDesc.RegisterIndex);
						TextureShaderValues[TextureIndex].RegisterIndex = ConstantDesc.RegisterIndex;
						TextureShaderValues[TextureIndex].RegisterCount = 1;
						TextureIndex++;
						break;
				}
			}
		}
	}

}

void ShaderRecord::SetCT() {
	
	ShaderValue* Value;

	if (HasCT) {
		if (HasRB && !TheShaderManager->RenderedBufferFilled) {
			TheRenderManager->device->StretchRect(TheRenderManager->currentRTGroup->RenderTargets[0]->data->Surface, NULL, TheShaderManager->RenderedSurface, NULL, D3DTEXF_NONE);
			TheShaderManager->RenderedBufferFilled = true;
		}
		if (HasDB && !TheShaderManager->DepthBufferFilled) {
			TheRenderManager->ResolveDepthBuffer();
			TheShaderManager->DepthBufferFilled = true;
		}
		for (int c = 0; c < TextureShaderValuesCount; c++) {
			Value = &TextureShaderValues[c];
			if (Value->Texture->Texture) TheRenderManager->device->SetTexture(Value->RegisterIndex, Value->Texture->Texture);
			if (Value->Texture->SamplerStates[0]) {
				for (int t = 1; t < SamplerStatesMax; t++) {
					if (Value->Texture->SamplerStates[t]) TheRenderManager->SetSamplerState(Value->RegisterIndex, (D3DSAMPLERSTATETYPE)(t), &Value->Texture->SamplerStates[t], true);
				}
			}
		}
		for (int c = 0; c < FloatShaderValuesCount; c++) {
			Value = &FloatShaderValues[c];
			if (Type == ShaderType_Vertex)
				TheRenderManager->device->SetVertexShaderConstantF(Value->RegisterIndex, (const float *)Value->Value, Value->RegisterCount);
			else
				TheRenderManager->device->SetPixelShaderConstantF(Value->RegisterIndex, (const float *)Value->Value, Value->RegisterCount);
		}
	}

}

EffectRecord::EffectRecord() {

	pSource = NULL;
	pEffect = NULL;
	pErrors = NULL;
	Enabled = false;
	FloatShaderValuesCount = 0;
	TextureShaderValuesCount = 0;

}

EffectRecord::~EffectRecord() {

	if (pEffect) { pEffect->Release(); pEffect = NULL; }
	if (pErrors) { pErrors->Release(); pErrors = NULL; }

}

bool EffectRecord::LoadEffect(const char* Name) {

	char FileName[MAX_PATH];
	bool Compiled = true;

	strcpy(FileName, Name);
	strcat(FileName, ".hlsl");
	std::ifstream FileSource(FileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (FileSource.is_open()) {
		std::streampos size = FileSource.tellg();
		pSource = new char[size];
		FileSource.seekg(0, std::ios::beg);
		FileSource.read(pSource, size);
		FileSource.close();
		if (TheSettingManager->SettingsMain.DevelopCompileEffects) {
			Compiled = false;
			ID3DXEffectCompiler* pCompiler = NULL;
			ID3DXBuffer* pEffectBuffer = NULL;
			D3DXCreateEffectCompilerFromFileA(FileName, NULL, NULL, NULL, &pCompiler, &pErrors);
			if (pErrors) _MESSAGE((char*)pErrors->GetBufferPointer());
			if (pCompiler) {
				pCompiler->CompileEffect(NULL, &pEffectBuffer, &pErrors);
				if (pErrors) _MESSAGE((char*)pErrors->GetBufferPointer());
			}
			if (pEffectBuffer) {
				std::ofstream FileBinary(Name, std::ios::out | std::ios::binary);
				FileBinary.write((char*)pEffectBuffer->GetBufferPointer(), pEffectBuffer->GetBufferSize());
				FileBinary.flush();
				FileBinary.close();
				Compiled = true;
				_MESSAGE("Effect compiled: %s", FileName);
			}
			if (pEffectBuffer) pEffectBuffer->Release();
			if (pCompiler) pCompiler->Release();
		}
		if (Compiled) {
			D3DXCreateEffectFromFileA(TheRenderManager->device, Name, NULL, NULL, NULL, NULL, &pEffect, &pErrors);
			if (pErrors) _MESSAGE((char*)pErrors->GetBufferPointer());
			if (pEffect) {
				CreateCT();
				_MESSAGE("Effect loaded: %s", Name);
				return true;
			}
		}
	}
	return false;

}

void EffectRecord::CreateCT() {

	D3DXEFFECT_DESC ConstantTableDesc;
	D3DXPARAMETER_DESC ConstantDesc;
	D3DXHANDLE Handle;
	UINT ConstantCount = 1;
	UInt32 FloatIndex = 0;
	UInt32 TextureIndex = 0;

	pEffect->GetDesc(&ConstantTableDesc);
	for (int c = 0; c < ConstantTableDesc.Parameters; c++) {
		Handle = pEffect->GetParameter(NULL, c);
		pEffect->GetParameterDesc(Handle, &ConstantDesc);
		if ((ConstantDesc.Class == D3DXPC_VECTOR || ConstantDesc.Class == D3DXPC_MATRIX_ROWS) && !memcmp(ConstantDesc.Name, "TESR_", 5)) FloatShaderValuesCount += 1;
		if (ConstantDesc.Class == D3DXPC_OBJECT && ConstantDesc.Type >= D3DXPT_SAMPLER && ConstantDesc.Type <= D3DXPT_SAMPLERCUBE && !memcmp(ConstantDesc.Name, "TESR_", 5)) TextureShaderValuesCount += 1;
	}
	FloatShaderValues = (ShaderValue*)malloc(FloatShaderValuesCount * sizeof(ShaderValue));
	TextureShaderValues = (ShaderValue*)malloc(TextureShaderValuesCount * sizeof(ShaderValue));
	for (int c = 0; c < ConstantTableDesc.Parameters; c++) {
		Handle = pEffect->GetParameter(NULL, c);
		pEffect->GetParameterDesc(Handle, &ConstantDesc);
		if (!memcmp(ConstantDesc.Name, "TESR_", 5)) {
			switch (ConstantDesc.Class) {
				case D3DXPC_VECTOR:
				case D3DXPC_MATRIX_ROWS:
					SetConstantTableValue(ConstantDesc.Name, FloatIndex);
					FloatShaderValues[FloatIndex].RegisterIndex = (UInt32)Handle;
					FloatShaderValues[FloatIndex].RegisterCount = ConstantDesc.Rows;
					FloatIndex++;
					break;
				case D3DXPC_OBJECT:
					if (ConstantDesc.Class == D3DXPC_OBJECT && ConstantDesc.Type >= D3DXPT_SAMPLER && ConstantDesc.Type <= D3DXPT_SAMPLERCUBE) {
						TextureShaderValues[TextureIndex].Texture = TheTextureManager->LoadTexture(pSource, TextureIndex);
						TextureShaderValues[TextureIndex].RegisterIndex = TextureIndex;
						TextureShaderValues[TextureIndex].RegisterCount = 1;
						TextureIndex++;
					}
					break;
			}
		}
	}

}

void EffectRecord::SetCT() {

	ShaderValue* Value;

	for (int c = 0; c < TextureShaderValuesCount; c++) {
		Value = &TextureShaderValues[c];
		if (Value->Texture->Texture) TheRenderManager->device->SetTexture(Value->RegisterIndex, Value->Texture->Texture);
	}
	for (int c = 0; c < FloatShaderValuesCount; c++) {
		Value = &FloatShaderValues[c];
		if (Value->RegisterCount == 1)
			pEffect->SetVector((D3DXHANDLE)Value->RegisterIndex, Value->Value);
		else
			pEffect->SetMatrix((D3DXHANDLE)Value->RegisterIndex, (D3DXMATRIX*)Value->Value);
	}

}

void EffectRecord::Render(IDirect3DDevice9* D3DDevice, IDirect3DSurface9* RenderTarget, IDirect3DSurface9* RenderedSurface, bool ClearRenderTarget) {

	UINT Passes;

	pEffect->Begin(&Passes, NULL);
	for (int p = 0; p < Passes; p++) {
		if (ClearRenderTarget) D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1, 0);
		pEffect->BeginPass(p);
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		pEffect->EndPass();
		D3DDevice->StretchRect(RenderTarget, NULL, RenderedSurface, NULL, D3DTEXF_NONE);
	}
	pEffect->End();

}

ShaderManager::ShaderManager() {

	_MESSAGE("Starting the shaders manager...");
	TheShaderManager = this;

	LARGE_INTEGER freq;
	float uadj, vadj;
	void* VertexPointer;

	SourceTexture = NULL;
	SourceSurface = NULL;
	RenderedTexture = NULL;
	RenderedSurface = NULL;
	RenderTextureSMAA = NULL;
	RenderSurfaceSMAA = NULL;
	RenderedBufferFilled = false;
	DepthBufferFilled = false;
	EffectVertex = NULL;
	UnderwaterEffect = NULL;
	WaterLensEffect = NULL;
	GodRaysEffect = NULL;
	DepthOfFieldEffect = NULL;
	AmbientOcclusionEffect = NULL;
	ColoringEffect = NULL;
	CinemaEffect = NULL;
	BloomEffect = NULL;
	SnowAccumulationEffect = NULL;
	BloodLensEffect = NULL;
	SMAAEffect = NULL;
	MotionBlurEffect = NULL;
	LowHFEffect = NULL;
	WetWorldEffect = NULL;
	SharpeningEffect = NULL;
	CustomEffect = NULL;
	InitializeConstants();

	QueryPerformanceFrequency(&freq);
	ShaderConst.TikTiming.w = (freq.QuadPart);
	ShaderConst.ReciprocalResolution.x = 1.0f / TheRenderManager->width;
	ShaderConst.ReciprocalResolution.y = 1.0f / TheRenderManager->height;
	ShaderConst.ReciprocalResolution.z = TheRenderManager->width / TheRenderManager->height;
	ShaderConst.ReciprocalResolution.w = 0.0f; // Reserved to store the FoV
	ShaderConst.ReciprocalResolutionWater.x = 1.0f / WaterHeightMapSize;
	ShaderConst.ReciprocalResolutionWater.y = 1.0f / WaterHeightMapSize;
	ShaderConst.ReciprocalResolutionWater.z = 1.0f / WaterDisplacementMapSize;
	ShaderConst.ReciprocalResolutionWater.w = 1.0f / WaterDisplacementMapSize;

	uadj = (1.0f / (float)TheRenderManager->width) * 0.5f;
	vadj = (1.0f / (float)TheRenderManager->height) * 0.5f;
	EffectQuad ShaderVertices[] = {
		{ -1.0f,  1.0f, 1.0f, 0.0f + uadj, 0.0f + vadj },
		{ -1.0f, -1.0f, 1.0f, 0.0f + uadj, 1.0f + vadj },
		{  1.0f,  1.0f, 1.0f, 1.0f + uadj, 0.0f + vadj },
		{  1.0f, -1.0f, 1.0f, 1.0f + uadj, 1.0f + vadj }
	};
	TheRenderManager->device->CreateVertexBuffer(4 * sizeof(EffectQuad), D3DUSAGE_WRITEONLY, EFFECTQUADFORMAT, D3DPOOL_DEFAULT, &EffectVertex, 0);
	EffectVertex->Lock(0, 0, &VertexPointer, 0);
	CopyMemory(VertexPointer, ShaderVertices, sizeof(ShaderVertices));
	EffectVertex->Unlock();
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SourceTexture, NULL);
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &RenderedTexture, NULL);
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &RenderTextureSMAA, NULL);
	SourceTexture->GetSurfaceLevel(0, &SourceSurface);
	RenderedTexture->GetSurfaceLevel(0, &RenderedSurface);
	RenderTextureSMAA->GetSurfaceLevel(0, &RenderSurfaceSMAA);
	
	if (TheSettingManager->SettingsMain.EnableUnderwater) CreateEffect(EffectRecordType_Underwater);
	if (TheSettingManager->SettingsMain.EnableWaterLens) CreateEffect(EffectRecordType_WaterLens);
	if (TheSettingManager->SettingsMain.EnableGodRays) CreateEffect(EffectRecordType_GodRays);
	if (TheSettingManager->SettingsMain.EnableDepthOfField) CreateEffect(EffectRecordType_DepthOfField);
	if (TheSettingManager->SettingsMain.EnableAmbientOcclusion) CreateEffect(EffectRecordType_AmbientOcclusion);
	if (TheSettingManager->SettingsMain.EnableColoring) CreateEffect(EffectRecordType_Coloring);
	if (TheSettingManager->SettingsMain.EnableCinema) CreateEffect(EffectRecordType_Cinema);
	if (TheSettingManager->SettingsMain.EnableBloom) CreateEffect(EffectRecordType_Bloom);
	if (TheSettingManager->SettingsMain.EnableSnowAccumulation) CreateEffect(EffectRecordType_SnowAccumulation);
	if (TheSettingManager->SettingsMain.EnableBloodLens) CreateEffect(EffectRecordType_BloodLens);
	if (TheSettingManager->SettingsMain.EnableSMAA) CreateEffect(EffectRecordType_SMAA);
	if (TheSettingManager->SettingsMain.EnableMotionBlur) CreateEffect(EffectRecordType_MotionBlur);
	if (TheSettingManager->SettingsMain.EnableLowHF) CreateEffect(EffectRecordType_LowHF);
	if (TheSettingManager->SettingsMain.EnableWetWorld) CreateEffect(EffectRecordType_WetWorld);
	if (TheSettingManager->SettingsMain.EnableSharpening) CreateEffect(EffectRecordType_Sharpening);
	if (TheSettingManager->SettingsMain.CustomEffects) CreateEffect(EffectRecordType_Custom);

}

void ShaderManager::InitializeConstants()
{
	ShaderConst.pWeather = NULL;
	ShaderConst.WaterLens_Percent = 0.0f;
	ShaderConst.BloodLens_Percent = 0.0f;
	ShaderConst.SnowAccumulation_Params.w = 0.0f;
	ShaderConst.WetWorld_Data.x = 0.0f;
}

void ShaderManager::UpdateConstants() {

	LARGE_INTEGER tick;
	SettingsWaterStruct *sws = NULL;
	Sky* pSky = *(Sky**)kSky;
	NiAVObject* SunRoot = (NiAVObject*)pSky->sun->SunBillboard->m_parent;
	TESClimate* currentClimate = pSky->firstClimate;
	TESWeather* currentWeather = pSky->firstWeather;
	float weatherPercent = pSky->weatherPercent;
	bool IsThirdPersonView;

	QueryPerformanceCounter(&tick);
	TheFrameRateManager->CurrentTime = time(NULL);
	ShaderConst.TikTiming.x = (float)(tick.QuadPart) / ShaderConst.TikTiming.w;
	ShaderConst.TikTiming.y = (float)(tick.QuadPart) * 1000 / ShaderConst.TikTiming.w;
	ShaderConst.TikTiming.z = (float)(tick.QuadPart) * 1000000 / ShaderConst.TikTiming.w;
	TheFrameRateManager->SetFrameTime(ShaderConst.TikTiming.x);

	if (TheSettingManager->SettingsMain.CameraMode)
		IsThirdPersonView = !TheRenderManager->FirstPersonView;
	else
		IsThirdPersonView = isThirdPersonView;
	TheRenderManager->GetCameraData();

	ShaderConst.currentCell = (*g_thePlayer)->parentCell;
	if (ShaderConst.currentCell) {
		ShaderConst.currentWorldSpace = ShaderConst.currentCell->worldSpace;
	
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
		TheUtilityManager->NormalizeVector3(&ShaderConst.SunDir);

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

				ShaderConst.fogData.x = ShaderConst.oldfogStart - ((ShaderConst.oldfogStart - ShaderConst.currentfogStart) * weatherPercent);
				ShaderConst.fogData.y = ShaderConst.oldfogEnd - ((ShaderConst.oldfogEnd - ShaderConst.currentfogEnd) * weatherPercent);
				ShaderConst.fogData.z = ShaderConst.currentsunGlare;

				if (weatherPercent == 1.0f) ShaderConst.pWeather = currentWeather;
			}
		}
		else {
			ShaderConst.SunDir.w = 0.0f;
			ShaderConst.SunAmount.x = 0.0f;
			ShaderConst.SunAmount.y = 1.0f;
			ShaderConst.SunAmount.z = 0.0f;
			ShaderConst.SunAmount.w = 0.0f;
			ShaderConst.currentsunGlare = 0.5f;
			TESObjectCELL::LightingData* LightData = ShaderConst.currentCell->lighting;
			ShaderConst.fogColor.x = LightData->fog.r / 255.0f;
			ShaderConst.fogColor.y = LightData->fog.g / 255.0f;
			ShaderConst.fogColor.z = LightData->fog.b / 255.0f;
			ShaderConst.fogColor.w = 1.0f;

			ShaderConst.sunColor.x = LightData->ambient.r / 255.0f;
			ShaderConst.sunColor.y = LightData->ambient.g / 255.0f;
			ShaderConst.sunColor.z = LightData->ambient.b / 255.0f;
			ShaderConst.sunColor.w = ShaderConst.SunAmount.w;

			ShaderConst.fogData.x = LightData->fogNear;
			ShaderConst.fogData.y = LightData->fogFar;
			ShaderConst.fogData.z = ShaderConst.currentsunGlare;
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
			TESWaterForm* currentWater = TheUtilityManager->GetWaterForm(ShaderConst.currentCell);
			
			if (currentWater) {
				ShaderConst.Water_deepColor.x = currentWater->DeepColorR / 255.0f;
				ShaderConst.Water_deepColor.y = currentWater->DeepColorG / 255.0f;
				ShaderConst.Water_deepColor.z = currentWater->DeepColorB / 255.0f;
				ShaderConst.Water_deepColor.w = currentWater->DeepColorA / 255.0f;
			
				ShaderConst.Water_shallowColor.x = currentWater->ShallowColorR / 255.0f;
				ShaderConst.Water_shallowColor.y = currentWater->ShallowColorG / 255.0f;
				ShaderConst.Water_shallowColor.z = currentWater->ShallowColorB / 255.0f;
				ShaderConst.Water_shallowColor.w = currentWater->ShallowColorA / 255.0f;
			}

			ShaderConst.Water_waterCoefficients.x = sws->inExtCoeff_R;
			ShaderConst.Water_waterCoefficients.y = sws->inExtCoeff_G;
			ShaderConst.Water_waterCoefficients.z = sws->inExtCoeff_B;
			ShaderConst.Water_waterCoefficients.w = sws->inScattCoeff;

			ShaderConst.Water_waveParams.x = sws->choppiness;
			ShaderConst.Water_waveParams.y = sws->waveWidth;
			ShaderConst.Water_waveParams.z = sws->waveSpeed;
			ShaderConst.Water_waveParams.w = sws->reflectivity;

			ShaderConst.Water_waterSettings.x = WaterLevel;
			ShaderConst.Water_waterSettings.y = sws->depthDarkness;
			ShaderConst.Water_waterSettings.z = sws->LODdistance;
			ShaderConst.Water_waterSettings.w = sws->MinLOD;

			ShaderConst.Water_waterVolume.x = sws->causticsStrength * ShaderConst.currentsunGlare;
			ShaderConst.Water_waterVolume.y = sws->shoreFactor;
			ShaderConst.Water_waterVolume.z = sws->turbidity;
			ShaderConst.Water_waterVolume.w = sws->causticsStrengthS;
			
			ShaderConst.HasWater = ShaderConst.currentCell->flags0 & TESObjectCELL::kFlags0_HasWater;
		}		

		if (TheSettingManager->SettingsMain.EnableSnowAccumulation) {
			if (currentWeather->precipType == 8) {
				if (ShaderConst.SnowAccumulation_Params.w < TheSettingManager->SettingsPrecipitations.SnowAmount) ShaderConst.SnowAccumulation_Params.w = ShaderConst.SnowAccumulation_Params.w + TheSettingManager->SettingsPrecipitations.SnowIncrease;
			}
			else {
				if (ShaderConst.SnowAccumulation_Params.w > 0.0f)
					ShaderConst.SnowAccumulation_Params.w = ShaderConst.SnowAccumulation_Params.w - TheSettingManager->SettingsPrecipitations.SnowDecrease;
				else if (ShaderConst.SnowAccumulation_Params.w < 0.0f)
					ShaderConst.SnowAccumulation_Params.w = 0.0f;
			}
			ShaderConst.SnowAccumulation_Params.x = TheSettingManager->SettingsPrecipitations.SnowBlurNormDropThreshhold;
			ShaderConst.SnowAccumulation_Params.y = TheSettingManager->SettingsPrecipitations.SnowBlurRadiusMultiplier;
			ShaderConst.SnowAccumulation_Params.z = TheSettingManager->SettingsPrecipitations.SnowSunPower;
		}
			
		if (TheSettingManager->SettingsMain.EnableWetWorld) {
			if (currentWeather->precipType == 4) {
				ShaderConst.WetWorld_Data.y = 1.0f;
				if (ShaderConst.WetWorld_Data.x < TheSettingManager->SettingsPrecipitations.RainAmount) ShaderConst.WetWorld_Data.x = ShaderConst.WetWorld_Data.x + TheSettingManager->SettingsPrecipitations.RainIncrease;
			}
			else {
				ShaderConst.WetWorld_Data.y = 0.3f - weatherPercent;
				if (ShaderConst.WetWorld_Data.y <= 0.0f) ShaderConst.WetWorld_Data.y = 0.05f;
				if (ShaderConst.WetWorld_Data.x > 0.0f)
					ShaderConst.WetWorld_Data.x = ShaderConst.WetWorld_Data.x - TheSettingManager->SettingsPrecipitations.RainDecrease;
				else if (ShaderConst.WetWorld_Data.x < 0.0f)
					ShaderConst.WetWorld_Data.x = 0.0f;
			}
			ShaderConst.WetWorld_Coeffs.x = TheSettingManager->SettingsPrecipitations.PuddleCoeff_R;
			ShaderConst.WetWorld_Coeffs.y = TheSettingManager->SettingsPrecipitations.PuddleCoeff_G;
			ShaderConst.WetWorld_Coeffs.z = TheSettingManager->SettingsPrecipitations.PuddleCoeff_B;
			ShaderConst.WetWorld_Coeffs.w = TheSettingManager->SettingsPrecipitations.PuddleSpecularMultiplier;
		}

		if (TheSettingManager->SettingsMain.EnableWaterLens) {
			ShaderConst.WaterLens_Time.x = sws->LensTimeMultA;
			ShaderConst.WaterLens_Time.y = sws->LensTimeMultB;
			ShaderConst.WaterLens_Time.z = sws->LensViscosity;
			if (ShaderConst.WaterLens_Percent == -1.0f) {
				ShaderConst.WaterLens_TimeAmount = 0.0f;
				ShaderConst.WaterLens_Time.w = sws->LensAmount;
			}
			else if (ShaderConst.WaterLens_Percent > 0.0f) {
				ShaderConst.WaterLens_TimeAmount += 1.0f;
				ShaderConst.WaterLens_Percent = 1.0f - ShaderConst.WaterLens_TimeAmount / sws->LensTime;
				if (ShaderConst.WaterLens_Percent < 0.0f) ShaderConst.WaterLens_Percent = 0.0f;
				ShaderConst.WaterLens_Time.w = sws->LensAmount * ShaderConst.WaterLens_Percent;
			}
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
			ShaderConst.Skin_SkinColor.x = TheSettingManager->SettingsSkin.CoeffRed;
			ShaderConst.Skin_SkinColor.y = TheSettingManager->SettingsSkin.CoeffGreen;
			ShaderConst.Skin_SkinColor.z = TheSettingManager->SettingsSkin.CoeffBlue;
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
			ShaderConst.GodRays_Data.x = TheSettingManager->SettingsGodRays.LightShaftPasses;
			ShaderConst.GodRays_Data.y = TheSettingManager->SettingsGodRays.Luminance;
			ShaderConst.GodRays_Data.z = TheSettingManager->SettingsGodRays.GlobalMultiplier;
		}

		if (TheSettingManager->SettingsMain.EnableAmbientOcclusion) {
			SettingsAmbientOcclusionStruct *sas = NULL;

			if (ShaderConst.currentWorldSpace)
				sas = TheSettingManager->GetSettingsAmbientOcclusion("Exterior");
			else
				sas = TheSettingManager->GetSettingsAmbientOcclusion("Interior");

			ShaderConst.AmbientOcclusion_Enabled = sas->Enabled;
			if (ShaderConst.AmbientOcclusion_Enabled) {
				ShaderConst.AmbientOcclusion_AOData.x = sas->RadiusMultiplier;
				ShaderConst.AmbientOcclusion_AOData.y = sas->StrengthMultiplier;
				ShaderConst.AmbientOcclusion_AOData.z = sas->ClampStrength;
				ShaderConst.AmbientOcclusion_AOData.w = sas->Range;
				ShaderConst.AmbientOcclusion_Data.x = sas->AngleBias;
				ShaderConst.AmbientOcclusion_Data.y = sas->LumThreshold * ShaderConst.SunAmount.y;
				ShaderConst.AmbientOcclusion_Data.z = sas->BlurDropThreshold;
				ShaderConst.AmbientOcclusion_Data.w = sas->BlurRadiusMultiplier;
			}
		}

		if (TheSettingManager->SettingsMain.EnableBloom) {
			SettingsBloomStruct *sbs = NULL;
			if (!(sbs = TheSettingManager->GetSettingsBloom(ShaderConst.currentCell->GetEditorName())))
				if (ShaderConst.currentWorldSpace)
					sbs = TheSettingManager->GetSettingsBloom(ShaderConst.currentWorldSpace->GetEditorName());

			if (sbs == NULL)
				sbs = TheSettingManager->GetSettingsBloom("Default");

			ShaderConst.Bloom_BloomData.x = sbs->Luminance;
			ShaderConst.Bloom_BloomData.y = sbs->MiddleGray;
			ShaderConst.Bloom_BloomData.z = sbs->WhiteCutOff;
			ShaderConst.Bloom_BloomValues.x = sbs->BloomIntensity;
			ShaderConst.Bloom_BloomValues.y = sbs->OriginalIntensity;
			ShaderConst.Bloom_BloomValues.z = sbs->BloomSaturation;
			ShaderConst.Bloom_BloomValues.w = sbs->OriginalSaturation;
		}

		if (TheSettingManager->SettingsMain.EnableColoring) {
			SettingsColoringStruct *scs = NULL;
			if (!(scs = TheSettingManager->GetSettingsColoring(ShaderConst.currentCell->GetEditorName())))
				if (ShaderConst.currentWorldSpace)
					scs = TheSettingManager->GetSettingsColoring(ShaderConst.currentWorldSpace->GetEditorName());

			if (scs == NULL)
				scs = TheSettingManager->GetSettingsColoring("Default");

			ShaderConst.Coloring_Data.x = scs->Strength;
			ShaderConst.Coloring_Data.y = scs->BaseGamma;
			ShaderConst.Coloring_Data.z = scs->Fade;
			ShaderConst.Coloring_Data.w = scs->Contrast;
			ShaderConst.Coloring_Values.x = scs->Saturation;
			ShaderConst.Coloring_Values.y = scs->Bleach;
			ShaderConst.Coloring_Values.z = scs->BleachLuma;
			ShaderConst.Coloring_Values.w = scs->Linearization;
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

		ShaderConst.LowHF_Data.x = 0.0f;
		if (TheSettingManager->SettingsMain.EnableLowHF) {
			float PlayerHealthPercent = GetPlayerHealthPercent;
			float PlayerFatiguePercent = GetPlayerFatiguePercent;

			if (PlayerHealthPercent < TheSettingManager->SettingsLowHF.HealthLimit) {
				ShaderConst.LowHF_HealthCoeff = 1.0f - PlayerHealthPercent / TheSettingManager->SettingsLowHF.HealthLimit;
				ShaderConst.LowHF_Data.x = ShaderConst.LowHF_HealthCoeff * TheSettingManager->SettingsLowHF.LumaMultiplier;
				ShaderConst.LowHF_Data.y = ShaderConst.LowHF_HealthCoeff * 0.01f * TheSettingManager->SettingsLowHF.BlurMultiplier;
				ShaderConst.LowHF_Data.z = ShaderConst.LowHF_HealthCoeff * 20.0f * TheSettingManager->SettingsLowHF.VignetteMultiplier;
				ShaderConst.LowHF_Data.w = (1.0f - ShaderConst.LowHF_HealthCoeff) * TheSettingManager->SettingsLowHF.DarknessMultiplier;
			}
			if (!ShaderConst.LowHF_Data.x && PlayerFatiguePercent < TheSettingManager->SettingsLowHF.FatigueLimit) {
				ShaderConst.LowHF_FatigueCoeff = 1.0f - PlayerFatiguePercent / TheSettingManager->SettingsLowHF.FatigueLimit;
				ShaderConst.LowHF_Data.x = ShaderConst.LowHF_FatigueCoeff * TheSettingManager->SettingsLowHF.LumaMultiplier;
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
				ShaderConst.DepthOfField_Blur.x = sds->DistantBlur;
				ShaderConst.DepthOfField_Blur.y = sds->DistantBlurStartRange;
				ShaderConst.DepthOfField_Blur.z = sds->DistantBlurEndRange;
				ShaderConst.DepthOfField_Blur.w = sds->BaseBlurRadius;
				ShaderConst.DepthOfField_Data.x = sds->BlurFallOff;
				ShaderConst.DepthOfField_Data.y = sds->Radius;
				ShaderConst.DepthOfField_Data.z = sds->DiameterRange;
				ShaderConst.DepthOfField_Data.w = sds->NearBlurCutOff;
				if (sds->DialogMode) {
					if (!TheUtilityManager->IsMenu(1009) && !TheUtilityManager->IsMenu(1034)) ShaderConst.DepthOfField_Enabled = 0;
				}
			}
		}

		if (TheSettingManager->SettingsMain.EnableCinema) {
			ShaderConst.Cinema_Data.x = TheSettingManager->SettingsCinema.AspectRatio;
			ShaderConst.Cinema_Data.y = TheSettingManager->SettingsCinema.VignetteRadius;
			if (ShaderConst.Cinema_Data.x != 0.0f && TheSettingManager->SettingsCinema.DialogMode) {
				if (!TheUtilityManager->IsMenu(1009) && !TheUtilityManager->IsMenu(1034)) {
					ShaderConst.Cinema_Data.x = 0.0f;
					ShaderConst.Cinema_Data.y = 0.0f;
				}
			}
			ShaderConst.Cinema_Data.z = TheSettingManager->SettingsCinema.VignetteDarkness;
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
				ShaderConst.MotionBlur_Data.x = (ShaderConst.MotionBlur_oldoldAmountX + ShaderConst.MotionBlur_oldAmountX + fMotionBlurAmtX) / 3.0f;
				ShaderConst.MotionBlur_Data.y = (ShaderConst.MotionBlur_oldoldAmountY + ShaderConst.MotionBlur_oldAmountY + fMotionBlurAmtY) / 3.0f;
			}
			else {
				ShaderConst.MotionBlur_Data.x = 0.0f;
				ShaderConst.MotionBlur_Data.y = 0.0f;
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

		if (TheSettingManager->SettingsMain.EnableSharpening) {
			ShaderConst.Sharpening_Data.x = TheSettingManager->SettingsSharpening.Strength;
			ShaderConst.Sharpening_Data.y = TheSettingManager->SettingsSharpening.Clamp;
			ShaderConst.Sharpening_Data.z = TheSettingManager->SettingsSharpening.Offset;
		}

		if (TheSettingManager->SettingsMain.FrameRate) TheFrameRateManager->Set();
	}
}

void ShaderManager::BeginScene() {

	RenderedBufferFilled = false;
	DepthBufferFilled = false;

}

ShaderRecord* ShaderManager::LoadShader(const char* Name) {

	ShaderRecord *NewShader = new ShaderRecord();

	if (!NewShader->LoadShader(Name)) {
		delete NewShader;
		return NULL;
	}
	return NewShader;

}

void ShaderManager::CreateEffect(EffectRecordType EffectType) {

	char Filename[MAX_PATH];

	strcpy(Filename, EffectsPath);
	switch (EffectType) {
	case EffectRecordType_Underwater:
		strcat(Filename, "Water\\Underwater.fx");
		UnderwaterEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableUnderwater = LoadEffect(UnderwaterEffect, Filename, NULL);
		break;
	case EffectRecordType_WaterLens:
		strcat(Filename, "Water\\WaterLens.fx");
		WaterLensEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableWaterLens = LoadEffect(WaterLensEffect, Filename, NULL);
		break;
	case EffectRecordType_GodRays:
		strcat(Filename, "GodRays\\GodRays.fx");
		GodRaysEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableGodRays = LoadEffect(GodRaysEffect, Filename, NULL);
		break;
	case EffectRecordType_DepthOfField:
		strcat(Filename, "DepthOfField\\DepthOfField.fx");
		DepthOfFieldEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableDepthOfField = LoadEffect(DepthOfFieldEffect, Filename, NULL);
		break;
	case EffectRecordType_AmbientOcclusion:
		strcat(Filename, "AmbientOcclusion\\AmbientOcclusion.fx");
		AmbientOcclusionEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableAmbientOcclusion = LoadEffect(AmbientOcclusionEffect, Filename, NULL);
		break;
	case EffectRecordType_Coloring:
		strcat(Filename, "Coloring\\Coloring.fx");
		ColoringEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableColoring = LoadEffect(ColoringEffect, Filename, NULL);
		break;
	case EffectRecordType_Cinema:
		strcat(Filename, "Cinema\\Cinema.fx");
		CinemaEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableCinema = LoadEffect(CinemaEffect, Filename, NULL);
		break;
	case EffectRecordType_Bloom:
		strcat(Filename, "Bloom\\Bloom.fx");
		BloomEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableBloom = LoadEffect(BloomEffect, Filename, NULL);
		break;
	case EffectRecordType_SnowAccumulation:
		strcat(Filename, "Precipitations\\SnowAccumulation.fx");
		SnowAccumulationEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableSnowAccumulation = LoadEffect(SnowAccumulationEffect, Filename, NULL);
		break;
	case EffectRecordType_BloodLens:
		strcat(Filename, "Blood\\BloodLens.fx");
		BloodLensEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableBloodLens = LoadEffect(BloodLensEffect, Filename, NULL);
		break;
	case EffectRecordType_SMAA:
		strcat(Filename, "SMAA\\SMAA.fx");
		SMAAEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableSMAA = LoadEffect(SMAAEffect, Filename, NULL);
		break;
	case EffectRecordType_MotionBlur:
		strcat(Filename, "MotionBlur\\MotionBlur.fx");
		MotionBlurEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableMotionBlur = LoadEffect(MotionBlurEffect, Filename, NULL);
		break;
	case EffectRecordType_LowHF:
		strcat(Filename, "LowHF\\LowHF.fx");
		LowHFEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableLowHF = LoadEffect(LowHFEffect, Filename, NULL);
		break;
	case EffectRecordType_WetWorld:
		strcat(Filename, "Precipitations\\WetWorld.fx");
		WetWorldEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableWetWorld = LoadEffect(WetWorldEffect, Filename, NULL);
		break;
	case EffectRecordType_Sharpening:
		strcat(Filename, "Sharpening\\Sharpening.fx");
		SharpeningEffect = new EffectRecord();
		TheSettingManager->SettingsMain.EnableSharpening = LoadEffect(SharpeningEffect, Filename, NULL);
		break;
	case EffectRecordType_Custom:
		WIN32_FIND_DATA File;
		HANDLE H;

		if (TheSettingManager->SettingsMain.DevelopCompileEffects)
			strcat(Filename, "Custom\\*.hlsl");
		else
			strcat(Filename, "Custom\\*.fx");
		H = FindFirstFileA(Filename, &File);
		if (H != INVALID_HANDLE_VALUE) {
			if (TheSettingManager->SettingsMain.DevelopCompileEffects) File.cFileName[strlen(File.cFileName) - 5] = NULL;
			strcpy(Filename, EffectsPath);
			strcat(Filename, "Custom\\");
			strcat(Filename, File.cFileName);
			CustomEffect = new EffectRecord();
			LoadEffect(CustomEffect, Filename, File.cFileName);
			while (FindNextFileA(H, &File)) {
				if (TheSettingManager->SettingsMain.DevelopCompileEffects) File.cFileName[strlen(File.cFileName) - 5] = NULL;
				strcpy(Filename, EffectsPath);
				strcat(Filename, "Custom\\");
				strcat(Filename, File.cFileName);
				CustomEffect = new EffectRecord();
				LoadEffect(CustomEffect, Filename, File.cFileName);
			}
			FindClose(H);
		}
		break;
	}

}

bool ShaderManager::LoadEffect(EffectRecord* TheEffect, char* Filename, char* CustomEffectName) {

	bool IsLoaded = TheEffect->LoadEffect(Filename);

	if (IsLoaded) {
		if (CustomEffectName) CustomEffectRecords[std::string(CustomEffectName).substr(0, strlen(CustomEffectName) - 3)] = TheEffect;
	}
	else
		DisposeEffect(TheEffect);
	return IsLoaded;

}

void ShaderManager::DisposeEffect(EffectRecord* TheEffect)
{

	if (TheEffect == AmbientOcclusionEffect) AmbientOcclusionEffect = NULL;
	else if (TheEffect == BloodLensEffect) BloodLensEffect = NULL;
	else if (TheEffect == BloomEffect) BloomEffect = NULL;
	else if (TheEffect == CinemaEffect) CinemaEffect = NULL;
	else if (TheEffect == ColoringEffect) ColoringEffect = NULL;
	else if (TheEffect == DepthOfFieldEffect) DepthOfFieldEffect = NULL;
	else if (TheEffect == GodRaysEffect) GodRaysEffect = NULL;
	else if (TheEffect == LowHFEffect) LowHFEffect = NULL;
	else if (TheEffect == MotionBlurEffect) MotionBlurEffect = NULL;
	else if (TheEffect == SMAAEffect) SMAAEffect = NULL;
	else if (TheEffect == SnowAccumulationEffect) SnowAccumulationEffect = NULL;
	else if (TheEffect == UnderwaterEffect) UnderwaterEffect = NULL;
	else if (TheEffect == WaterLensEffect) WaterLensEffect = NULL;
	else if (TheEffect == WetWorldEffect) WetWorldEffect = NULL;
	else if (TheEffect == SharpeningEffect) SharpeningEffect = NULL;

	if (TheEffect) delete TheEffect;

}

void ShaderManager::RenderEffects() {

	IDirect3DDevice9 *D3DDevice = TheRenderManager->device;
	IDirect3DSurface9* RenderTarget = TheRenderManager->RenderTarget;

	TheRenderManager->SetCameraData();
	D3DDevice->SetStreamSource(0, EffectVertex, 0, sizeof(EffectQuad));
	SetCurrentVertexStream;
	D3DDevice->StretchRect(RenderTarget, NULL, RenderedSurface, NULL, D3DTEXF_NONE);

	if (TheSettingManager->SettingsMain.EnableWetWorld && TheShaderManager->ShaderConst.currentWorldSpace && TheShaderManager->ShaderConst.WetWorld_Data.x > 0) {
		D3DDevice->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		WetWorldEffect->SetCT();
		WetWorldEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}
	else if (TheSettingManager->SettingsMain.EnableSnowAccumulation && TheShaderManager->ShaderConst.currentWorldSpace && TheShaderManager->ShaderConst.SnowAccumulation_Params.w > 0) {
		D3DDevice->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		SnowAccumulationEffect->SetCT();
		SnowAccumulationEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableColoring) {
		ColoringEffect->SetCT();
		ColoringEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableBloom) {
		D3DDevice->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		BloomEffect->SetCT();
		BloomEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableUnderwater && TheShaderManager->ShaderConst.HasWater && TheRenderManager->CameraPosition.z < TheShaderManager->ShaderConst.Water_waterSettings.x + 20) {
		if (TheRenderManager->CameraPosition.z < TheShaderManager->ShaderConst.Water_waterSettings.x) {
			if (TheShaderManager->ShaderConst.BloodLens_Percent) TheShaderManager->ShaderConst.BloodLens_Percent = 0.0f;
			TheShaderManager->ShaderConst.WaterLens_Percent = -1.0f;
		}
		UnderwaterEffect->SetCT();
		UnderwaterEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}
	else {
		if (TheShaderManager->ShaderConst.WaterLens_Percent == -1.0f) TheShaderManager->ShaderConst.WaterLens_Percent = 1.0f;

		if (TheSettingManager->SettingsMain.EnableAmbientOcclusion && TheShaderManager->ShaderConst.AmbientOcclusion_Enabled) {
			D3DDevice->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
			AmbientOcclusionEffect->SetCT();
			AmbientOcclusionEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
		}

		if (TheSettingManager->SettingsMain.EnableGodRays && TheShaderManager->ShaderConst.currentWorldSpace) {
			D3DDevice->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
			GodRaysEffect->SetCT();
			GodRaysEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
		}
	}

	if (TheSettingManager->SettingsMain.EnableDepthOfField && TheShaderManager->ShaderConst.DepthOfField_Enabled) {
		D3DDevice->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		DepthOfFieldEffect->SetCT();
		DepthOfFieldEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableBloodLens && TheShaderManager->ShaderConst.BloodLens_Percent > 0.0f) {
		BloodLensEffect->SetCT();
		BloodLensEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableWaterLens && TheShaderManager->ShaderConst.WaterLens_Percent > 0.0f) {
		WaterLensEffect->SetCT();
		WaterLensEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableMotionBlur && (TheShaderManager->ShaderConst.MotionBlur_Data.x || TheShaderManager->ShaderConst.MotionBlur_Data.y)) {
		MotionBlurEffect->SetCT();
		MotionBlurEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableLowHF && TheShaderManager->ShaderConst.LowHF_Data.x) {
		LowHFEffect->SetCT();
		LowHFEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableSharpening) {
		SharpeningEffect->SetCT();
		SharpeningEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableCinema && (TheShaderManager->ShaderConst.Cinema_Data.x != 0.0f || TheShaderManager->ShaderConst.Cinema_Data.y != 0.0f)) {
		CinemaEffect->SetCT();
		CinemaEffect->Render(D3DDevice, RenderTarget, RenderedSurface, false);
	}

	if (TheSettingManager->SettingsMain.CustomEffects) {
		for (CustomEffectRecordList::iterator iter = CustomEffectRecords.begin(); iter != CustomEffectRecords.end(); ++iter) {
			if (iter->second->Enabled) {
				D3DDevice->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
				iter->second->SetCT();
				iter->second->Render(D3DDevice, RenderTarget, RenderedSurface, false);
			}
		}
	}

	if (TheSettingManager->SettingsMain.EnableSMAA) {
		D3DDevice->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		D3DDevice->SetRenderTarget(0, RenderSurfaceSMAA);
		SMAAEffect->SetCT();
		SMAAEffect->Render(D3DDevice, RenderSurfaceSMAA, RenderedSurface, true);
		D3DDevice->StretchRect(RenderSurfaceSMAA, NULL, RenderTarget, NULL, D3DTEXF_NONE);
		D3DDevice->SetRenderTarget(0, RenderTarget);
	}

	if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.ScreenshotKey)) {
		char Filename[MAX_PATH];
		char Name[80];

		strcpy(Filename, TheSettingManager->SettingsMain.ScreenshotPath);
		strcat(Filename, ScreenshotFilenamePrefix);
		strftime(Name, 80, "%Y%m%d %H.%M.%S", localtime(&TheFrameRateManager->CurrentTime));
		strcat(Filename, Name);
		if (TheSettingManager->SettingsMain.ScreenshotType == 0)
			strcat(Filename, ".bmp");
		else
			strcat(Filename, ".jpg");
		if (GetFileAttributesA(TheSettingManager->SettingsMain.ScreenshotPath) == INVALID_FILE_ATTRIBUTES) CreateDirectoryA(TheSettingManager->SettingsMain.ScreenshotPath, NULL);
		D3DXSaveSurfaceToFileA(Filename, (D3DXIMAGE_FILEFORMAT)TheSettingManager->SettingsMain.ScreenshotType, RenderTarget, NULL, NULL);
		TheUtilityManager->ShowMessage("Screenshot taken!");
	}

}

void ShaderManager::EnableEffect(const char* Name, bool Value)
{

	if (!memcmp(Name, "AmbientOcclusion", 16)) {
		TheSettingManager->SettingsMain.EnableAmbientOcclusion = Value;
		DisposeEffect(AmbientOcclusionEffect);
		if (Value) CreateEffect(EffectRecordType_AmbientOcclusion);
	}
	else if (!memcmp(Name, "BloodLens", 9)) {
		TheSettingManager->SettingsMain.EnableBloodLens = Value;
		DisposeEffect(BloodLensEffect);
		if (Value) CreateEffect(EffectRecordType_BloodLens);
	}
	else if (!memcmp(Name, "Bloom", 5)) {
		TheSettingManager->SettingsMain.EnableBloom = Value;
		DisposeEffect(BloomEffect);
		if (Value) CreateEffect(EffectRecordType_Bloom);
	}
	else if (!memcmp(Name, "Cinema", 6)) {
		TheSettingManager->SettingsMain.EnableCinema = Value;
		DisposeEffect(CinemaEffect);
		if (Value) CreateEffect(EffectRecordType_Cinema);
	}
	else if (!memcmp(Name, "Coloring", 8)) {
		TheSettingManager->SettingsMain.EnableColoring = Value;
		DisposeEffect(ColoringEffect);
		if (Value) CreateEffect(EffectRecordType_Coloring);
	}
	else if (!memcmp(Name, "DepthOfField", 12)) {
		TheSettingManager->SettingsMain.EnableDepthOfField = Value;
		DisposeEffect(DepthOfFieldEffect);
		if (Value) CreateEffect(EffectRecordType_DepthOfField);
	}
	else if (!memcmp(Name, "GodRays", 7)) {
		TheSettingManager->SettingsMain.EnableGodRays = Value;
		DisposeEffect(GodRaysEffect);
		if (Value) CreateEffect(EffectRecordType_GodRays);
	}
	else if (!memcmp(Name, "LowHF", 5)) {
		TheSettingManager->SettingsMain.EnableLowHF = Value;
		DisposeEffect(LowHFEffect);
		if (Value) CreateEffect(EffectRecordType_LowHF);
	}
	else if (!memcmp(Name, "MotionBlur", 10)) {
		TheSettingManager->SettingsMain.EnableMotionBlur = Value;
		DisposeEffect(MotionBlurEffect);
		if (Value) CreateEffect(EffectRecordType_MotionBlur);
	}
	else if (!memcmp(Name, "SMAA", 4)) {
		TheSettingManager->SettingsMain.EnableSMAA = Value;
		DisposeEffect(SMAAEffect);
		if (Value) CreateEffect(EffectRecordType_SMAA);
	}
	else if (!memcmp(Name, "SnowAccumulation", 16)) {
		TheSettingManager->SettingsMain.EnableSnowAccumulation = Value;
		DisposeEffect(SnowAccumulationEffect);
		if (Value) CreateEffect(EffectRecordType_SnowAccumulation);
	}
	else if (!memcmp(Name, "Underwater", 10)) {
		TheSettingManager->SettingsMain.EnableUnderwater = Value;
		DisposeEffect(UnderwaterEffect);
		if (Value) CreateEffect(EffectRecordType_Underwater);
	}
	else if (!memcmp(Name, "WaterLens", 9)) {
		TheSettingManager->SettingsMain.EnableWaterLens = Value;
		DisposeEffect(WaterLensEffect);
		if (Value) CreateEffect(EffectRecordType_WaterLens);
	}
	else if (!memcmp(Name, "WetWorld", 8)) {
		TheSettingManager->SettingsMain.EnableWetWorld = Value;
		DisposeEffect(WetWorldEffect);
		if (Value) CreateEffect(EffectRecordType_WetWorld);
	}
	else if (!memcmp(Name, "Sharpening", 10)) {
		TheSettingManager->SettingsMain.EnableSharpening = Value;
		DisposeEffect(SharpeningEffect);
		if (Value) CreateEffect(EffectRecordType_Sharpening);
	}

}

void ShaderManager::SetShaderValue(const char* Name, const char* ConstantName, D3DXVECTOR4 Value)
{
	
	CustomConstants::iterator v = CustomConst.find(std::string(Name));
	if (v != CustomConst.end()) v->second = Value;

}

void ShaderManager::SetCustomShaderValue(const char* Name, const char* ConstantName, D3DXVECTOR4 Value)
{

	CustomEffectRecordList::iterator v = CustomEffectRecords.find(std::string(Name));
	if (v != CustomEffectRecords.end()) {
		if (!memcmp(ConstantName, "Enabled", 7))
			v->second->Enabled = Value;
		else
			v->second->pEffect->SetVector(ConstantName, &Value);
	}

}