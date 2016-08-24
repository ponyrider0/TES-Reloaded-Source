#include <ctime>
#include <string>
#include <fstream>

#if defined(OBLIVION)
#define SetCurrentVertexStream TheRenderManager->renderState->SetFVF(EFFECTQUADFORMAT, false)
#elif defined(SKYRIM)
#define SetCurrentVertexStream D3DDevice->SetFVF(EFFECTQUADFORMAT)
#endif

EffectRecord::EffectRecord() {

  pEffect = NULL;
  pErrors = NULL;
  Enabled = false;

}

EffectRecord::~EffectRecord() {

	if (pEffect) { pEffect->Release(); pEffect = NULL; }
	if (pErrors) { pErrors->Release(); pErrors = NULL; }

}

bool EffectRecord::LoadEffect(char *Filename) {
	
	bool Compiled = true;

	if (TheSettingManager->SettingsMain.DevelopCompileEffects) {
		Compiled = false;
		char FilenameSource[MAX_PATH];
		strcpy(FilenameSource, Filename);
		strcat(FilenameSource,".hlsl");
		ID3DXEffectCompiler* pCompiler = NULL;
		ID3DXBuffer* pEffectBuffer = NULL;
		D3DXCreateEffectCompilerFromFileA(FilenameSource, NULL, NULL, NULL, &pCompiler, &pErrors);
		if (pErrors) _MESSAGE((char*)pErrors->GetBufferPointer());
		if (pCompiler) {
			pCompiler->CompileEffect(NULL, &pEffectBuffer, &pErrors);
			if (pErrors) _MESSAGE((char*)pErrors->GetBufferPointer());
		}
		if (pEffectBuffer) {
			std::ofstream FileBinary(Filename, std::ios::out|std::ios::binary);
			FileBinary.write((char*)pEffectBuffer->GetBufferPointer(), pEffectBuffer->GetBufferSize());
			FileBinary.flush();
			FileBinary.close();
			Compiled = true;
			_MESSAGE("Effect compiled: %s", FilenameSource);
		}
		if (pEffectBuffer) pEffectBuffer->Release();
		if (pCompiler) pCompiler->Release();
	}
	if (Compiled) {
		D3DXCreateEffectFromFileA(TheRenderManager->device, Filename, NULL, NULL, NULL, NULL, &pEffect, &pErrors);
		if (pErrors) _MESSAGE((char*)pErrors->GetBufferPointer());
		if (pEffect) {
			ApplyCompileDirectives();
			_MESSAGE("Effect loaded: %s", Filename);
			return true;
		}
	}
	return false;

}

void EffectRecord::ApplyCompileDirectives() {

  D3DXEFFECT_DESC Description;
  pEffect->GetDesc(&Description);

  for (int par = 0; par < Description.Parameters; par++) {
    D3DXHANDLE handle;

    if (handle = pEffect->GetParameter(NULL, par)) {
      D3DXPARAMETER_DESC Description;
      pEffect->GetParameterDesc(handle, &Description);

      if (Description.Type == D3DXPT_TEXTURECUBE) {
		D3DXHANDLE handle2;

		if ((handle2 = pEffect->GetAnnotationByName(handle, "filename"))) {
		  LPCSTR pString = NULL; pEffect->GetString(handle2, &pString);

		  int TexNum = TheTextureManager->LoadTexture((char *)pString, TR_CUBIC);
		  if (TexNum != -1)
			pEffect->SetTexture(handle, TheTextureManager->GetTexture(TexNum)->GetTexture());
		}
      }
      else if (Description.Type == D3DXPT_TEXTURE3D) {
		D3DXHANDLE handle2;

		if ((handle2 = pEffect->GetAnnotationByName(handle, "filename"))) {
		  LPCSTR pString = NULL; pEffect->GetString(handle2, &pString);

		  int TexNum = TheTextureManager->LoadTexture((char *)pString, TR_VOLUMETRIC);
		  if (TexNum != -1)
			pEffect->SetTexture(handle, TheTextureManager->GetTexture(TexNum)->GetTexture());
		}
      }
      else if ((Description.Type == D3DXPT_TEXTURE) || (Description.Type == D3DXPT_TEXTURE1D) || (Description.Type == D3DXPT_TEXTURE2D)) {
		  D3DXHANDLE handle2;

		  if ((handle2 = pEffect->GetAnnotationByName(handle, "filename"))) {
			LPCSTR pString = NULL; pEffect->GetString(handle2, &pString);

			int TexNum = TheTextureManager->LoadTexture((char *)pString, TR_PLANAR);
			if (TexNum != -1)
			  pEffect->SetTexture(handle, TheTextureManager->GetTexture(TexNum)->GetTexture());
		  }
      }
    }
  }
}

void EffectRecord::ApplyPermanents(IDirect3DTexture9* RenderTexture, IDirect3DTexture9* SourceTexture) {
	
	ShaderConstants *ShaderConst = &TheShaderManager->ShaderConst;

	pEffect->SetTexture("renderTexture", RenderTexture);
	pEffect->SetTexture("sourceTexture", SourceTexture);
	pEffect->SetTexture("depthTexture", TheRenderManager->DepthTexture);
	pEffect->SetVector("rcpres", &ShaderConst->rcpres);
	
}

void EffectRecord::ApplyConstants(EffectTypeEnum EffectType) {

	ShaderConstants *ShaderConst = &TheShaderManager->ShaderConst;

	switch (EffectType)
	{
		case Underwater:
			pEffect->SetMatrix("m44view", &(D3DXMATRIX)TheRenderManager->viewMatrix);
			pEffect->SetMatrix("m44proj", &(D3DXMATRIX)TheRenderManager->projMatrix);
			pEffect->SetVector("f4CameraPosition", &TheRenderManager->CameraPosition);
			pEffect->SetVector("TikTiming", &ShaderConst->TikTiming);
			pEffect->SetVector("f4SunDir", &ShaderConst->SunDir);
			pEffect->SetVector("waterCoefficients", &ShaderConst->Water_waterCoefficients);
			pEffect->SetVector("sunColor", &ShaderConst->sunColor);
			pEffect->SetVector("fogColor", &ShaderConst->fogColor);
			pEffect->SetVector("depthColor", &ShaderConst->Water_depthColor);
			pEffect->SetVector("shallowColor", &ShaderConst->Water_shallowColor);
			pEffect->SetFloat("waveSpeed", ShaderConst->Water_waveSpeed.x);
			pEffect->SetFloat("causticsStrength", ShaderConst->Water_waterVolume.x);
			pEffect->SetFloat("turbidity", ShaderConst->Water_waterVolume.z);
			pEffect->SetFloat("depthDarkness", ShaderConst->Water_depthDarkness);
			pEffect->SetFloat("fogStart", ShaderConst->fogStart);
			pEffect->SetFloat("fogEnd", ShaderConst->fogEnd);
			pEffect->SetFloat("waveWidth", ShaderConst->Water_waveParams.y);
			pEffect->SetFloat("waterHeight", ShaderConst->WaterHeight);
			break;
		case WaterLens:
			pEffect->SetVector("TikTiming", &ShaderConst->TikTiming);
			pEffect->SetVector("Time", &ShaderConst->WaterLens_Time);
			pEffect->SetFloat("Amount", ShaderConst->WaterLens_Amount);
			pEffect->SetFloat("Viscosity", ShaderConst->WaterLens_Viscosity);
			break;
		case GodRays:
			pEffect->SetInt("ShaftPasses", ShaderConst->GodRays_LightShaftPasses);
			pEffect->SetMatrix("m44view", &(D3DXMATRIX)TheRenderManager->viewMatrix);
			pEffect->SetMatrix("m44proj", &(D3DXMATRIX)TheRenderManager->projMatrix);
			pEffect->SetVector("f4CameraForward", &TheRenderManager->CameraForward);
			pEffect->SetVector("f4SunDir", &ShaderConst->SunDir);
			pEffect->SetVector("f4Time", &ShaderConst->GameTime);
			pEffect->SetVector("Ray", &ShaderConst->GodRays_Ray);
			pEffect->SetVector("sunColor", &ShaderConst->sunColor);
			pEffect->SetVector("sunAmount", &ShaderConst->SunAmount);
			pEffect->SetVector("RayColor", &ShaderConst->GodRays_RayColor);
			pEffect->SetFloat("Luminance", ShaderConst->GodRays_Luminance);
			pEffect->SetFloat("FoV", ShaderConst->FoV);
			pEffect->SetFloat("GlobalMul", ShaderConst->GodRays_GlobalMultiplier);
			break;
		case DepthOfField:
			pEffect->SetMatrix("m44proj", &(D3DXMATRIX)TheRenderManager->projMatrix);
			pEffect->SetBool("DistantBlur", ShaderConst->DepthOfField_DistantBlur);
			pEffect->SetFloat("DistantBlurStartRange", ShaderConst->DepthOfField_DistantBlurStartRange);
			pEffect->SetFloat("DistantBlurEndRange", ShaderConst->DepthOfField_DistantBlurEndRange);
			pEffect->SetFloat("BaseBlurRadius", ShaderConst->DepthOfField_BaseBlurRadius);
			pEffect->SetFloat("BlurFallOff", ShaderConst->DepthOfField_BlurFallOff);
			pEffect->SetFloat("Radius", ShaderConst->DepthOfField_Radius);
			pEffect->SetFloat("DiameterRange", ShaderConst->DepthOfField_DiameterRange);
			pEffect->SetFloat("NearBlurCutOff", ShaderConst->DepthOfField_NearBlurCutOff);
			break;
		case AmbientOcclusion:
			pEffect->SetMatrix("m44proj", &(D3DXMATRIX)TheRenderManager->projMatrix);
			pEffect->SetFloat("aoRadiusMultiplier", ShaderConst->AmbientOcclusion_RadiusMultiplier);
			pEffect->SetFloat("aoStrengthMultiplier", ShaderConst->AmbientOcclusion_StrengthMultiplier);
			pEffect->SetFloat("aoClamp", ShaderConst->AmbientOcclusion_ClampStrength);
			pEffect->SetFloat("angleBias", ShaderConst->AmbientOcclusion_AngleBias);
			pEffect->SetFloat("aoRange", ShaderConst->AmbientOcclusion_Range);
			pEffect->SetFloat("luminosity_threshold", ShaderConst->AmbientOcclusion_LumThreshold);
			pEffect->SetFloat("Blur_DropThreshhold", ShaderConst->AmbientOcclusion_BlurDropThreshold);
			pEffect->SetFloat("Blur_RadiusMultiplier", ShaderConst->AmbientOcclusion_BlurRadiusMultiplier);
			pEffect->SetFloat("fogStart", ShaderConst->fogStart);
			pEffect->SetFloat("fogEnd", ShaderConst->fogEnd);
			pEffect->SetFloat("FoV", ShaderConst->FoV);
			break;
		case Coloring:
			pEffect->SetVector("ColorCurve", &ShaderConst->Coloring_ColorCurve);
			pEffect->SetVector("EffectGamma", &ShaderConst->Coloring_EffectGamma);
			pEffect->SetFloat("Strength", ShaderConst->Coloring_Strength);
			pEffect->SetFloat("BaseGamma", ShaderConst->Coloring_BaseGamma);
			pEffect->SetFloat("Fade", ShaderConst->Coloring_Fade);
			pEffect->SetFloat("Contrast", ShaderConst->Coloring_Contrast);
			pEffect->SetFloat("Saturation", ShaderConst->Coloring_Saturation);
			pEffect->SetFloat("Bleach", ShaderConst->Coloring_Bleach);
			pEffect->SetFloat("BleachLuma", ShaderConst->Coloring_BleachLuma);
			pEffect->SetFloat("Linearization", ShaderConst->Coloring_Linearization);
			break;
		case Cinema:
			pEffect->SetFloat("AspectRatio", ShaderConst->Cinema_AspectRatio);
			pEffect->SetFloat("VignetteRadius", ShaderConst->Cinema_VignetteRadius);
			pEffect->SetFloat("VignetteDarkness", ShaderConst->Cinema_VignetteDarkness);
			break;
		case Bloom:
			pEffect->SetVector("BloomValues", &ShaderConst->Bloom_BloomValues);
			pEffect->SetFloat("Luminance", ShaderConst->Bloom_Luminance);
			pEffect->SetFloat("MiddleGray", ShaderConst->Bloom_MiddleGray);
			pEffect->SetFloat("WhiteCutOff", ShaderConst->Bloom_WhiteCutOff);
			break;
		case SnowAccumulation:
			pEffect->SetMatrix("m44world", &(D3DXMATRIX)TheRenderManager->worldMatrix);
			pEffect->SetMatrix("m44view", &(D3DXMATRIX)TheRenderManager->viewMatrix);
			pEffect->SetMatrix("m44proj", &(D3DXMATRIX)TheRenderManager->projMatrix);
			pEffect->SetVector("f4SunDir", &ShaderConst->SunDir);
			pEffect->SetVector("sunColor", &ShaderConst->sunColor);
			pEffect->SetVector("Params", &ShaderConst->SnowAccumulation_Params);
			pEffect->SetFloat("waterHeight", ShaderConst->WaterHeight);
			pEffect->SetFloat("fogStart", ShaderConst->fogStart);
			pEffect->SetFloat("fogEnd", ShaderConst->fogEnd);
			pEffect->SetFloat("sunGlare", ShaderConst->currentsunGlare);
			break;
		case BloodLens:
			pEffect->SetVector("BloodParams", &ShaderConst->BloodLens_Params);
			pEffect->SetVector("BloodColor", &ShaderConst->BloodLens_BloodColor);
			break;
		case MotionBlur:
			pEffect->SetVector("BlurParams", &ShaderConst->MotionBlur_BlurParams);
			pEffect->SetFloat("Blurx", ShaderConst->MotionBlur_AmountX);
			pEffect->SetFloat("Blury", ShaderConst->MotionBlur_AmountY);
			break;
		case LowHF:
			pEffect->SetVector("Params", &ShaderConst->LowHF_Params);
			break;
		case WetWorld:
			pEffect->SetMatrix("m44world", &(D3DXMATRIX)TheRenderManager->worldMatrix);
			pEffect->SetMatrix("m44view", &(D3DXMATRIX)TheRenderManager->viewMatrix);
			pEffect->SetMatrix("m44proj", &(D3DXMATRIX)TheRenderManager->projMatrix);
			pEffect->SetVector("TikTiming", &ShaderConst->TikTiming);
			pEffect->SetVector("f4SunDir", &ShaderConst->SunDir);
			pEffect->SetVector("fogColor", &ShaderConst->fogColor);
			pEffect->SetVector("sunColor", &ShaderConst->sunColor);
			pEffect->SetVector("Coeffs", &ShaderConst->WetWorld_Coeffs);
			pEffect->SetFloat("waterHeight", ShaderConst->WaterHeight);
			pEffect->SetFloat("fogStart", ShaderConst->fogStart);
			pEffect->SetFloat("fogEnd", ShaderConst->fogEnd);
			pEffect->SetFloat("RainAmount", ShaderConst->WetWorld_RainAmount);
			pEffect->SetFloat("RippleWeight", ShaderConst->WetWorld_RippleWeight);
			break;
		case Custom:
			pEffect->SetMatrix("m44world", &(D3DXMATRIX)TheRenderManager->worldMatrix);
			pEffect->SetMatrix("m44view", &(D3DXMATRIX)TheRenderManager->viewMatrix);
			pEffect->SetMatrix("m44proj", &(D3DXMATRIX)TheRenderManager->projMatrix);
			pEffect->SetVector("f4CameraForward", &TheRenderManager->CameraForward);
			pEffect->SetVector("f4CameraPosition", &TheRenderManager->CameraPosition);
			pEffect->SetVector("TikTiming", &ShaderConst->TikTiming);
			pEffect->SetVector("f4SunDir", &ShaderConst->SunDir);
			pEffect->SetVector("fogColor", &ShaderConst->fogColor);
			pEffect->SetVector("sunColor", &ShaderConst->sunColor);
			pEffect->SetFloat("fogStart", ShaderConst->fogStart);
			pEffect->SetFloat("fogEnd", ShaderConst->fogEnd);
			break;
	}

}

void EffectRecord::Render(IDirect3DDevice9 *D3DDevice, IDirect3DSurface9 *RenderTo, IDirect3DSurface9 *RenderSurface, bool ClearRenderTo) {

	UINT pPasses;
  
	pEffect->Begin(&pPasses, NULL);
	for (int p = 0; p < pPasses; p++) {
		if (ClearRenderTo) D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1, 0);
		pEffect->BeginPass(p);
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		pEffect->EndPass();
		D3DDevice->StretchRect(RenderTo,NULL,RenderSurface,NULL,D3DTEXF_NONE);
	}
	pEffect->End();

}

void EffectRecord::GetValue(const char* Const, float *Value) {

	if (pEffect) pEffect->GetFloat(Const, Value);

}

void EffectRecord::SetValue(const char* Const, float Value) {
	
	if (pEffect) pEffect->SetFloat(Const, Value);

}

EffectManager::EffectManager() {

	_MESSAGE("Starting the effects manager...");
	
	EffectVertex = NULL;
	RenderToTexture = NULL;
	RenderTexture = NULL;
	SourceTexture = NULL;
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
	IsInitialized = false;

}

void EffectManager::Initialize()
{

	float uadj, vadj;
	void* VertexPointer;

	uadj = (1.0f / TheRenderManager->width) * 0.5;
	vadj = (1.0f / TheRenderManager->height) * 0.5;

	EffectQuad ShaderVertices[] = {
		{-1 , +1 , 1, 0 + uadj , 0 + vadj},
		{-1 , -1 , 1, 0 + uadj , 1 + vadj},
		{1  , +1 , 1, 1 + uadj , 0 + vadj},
		{1  , -1 , 1, 1 + uadj , 1 + vadj}
	};

	TheRenderManager->device->CreateVertexBuffer(4 * sizeof(EffectQuad), D3DUSAGE_WRITEONLY, EFFECTQUADFORMAT, D3DPOOL_DEFAULT, &EffectVertex, 0);
	EffectVertex->Lock(0, 0, &VertexPointer, 0);
	CopyMemory(VertexPointer, ShaderVertices, sizeof(ShaderVertices));
	EffectVertex->Unlock();

	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &RenderToTexture, NULL);
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &RenderTexture, NULL);
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SourceTexture, NULL);
	RenderToTexture->GetSurfaceLevel(0, &RenderToSurface);
	RenderTexture->GetSurfaceLevel(0, &RenderSurface);
	SourceTexture->GetSurfaceLevel(0, &SourceSurface);

	if (TheSettingManager->SettingsMain.EnableUnderwater) CreateEffect(Underwater);
	if (TheSettingManager->SettingsMain.EnableWaterLens) CreateEffect(WaterLens);
	if (TheSettingManager->SettingsMain.EnableGodRays) CreateEffect(GodRays);
	if (TheSettingManager->SettingsMain.EnableDepthOfField) CreateEffect(DepthOfField);
	if (TheSettingManager->SettingsMain.EnableAmbientOcclusion) CreateEffect(AmbientOcclusion);
	if (TheSettingManager->SettingsMain.EnableColoring) CreateEffect(Coloring);
	if (TheSettingManager->SettingsMain.EnableCinema) CreateEffect(Cinema);
	if (TheSettingManager->SettingsMain.EnableBloom) CreateEffect(Bloom);
	if (TheSettingManager->SettingsMain.EnableSnowAccumulation) CreateEffect(SnowAccumulation);
	if (TheSettingManager->SettingsMain.EnableBloodLens) CreateEffect(BloodLens);
	if (TheSettingManager->SettingsMain.EnableSMAA) CreateEffect(SMAA);
	if (TheSettingManager->SettingsMain.EnableMotionBlur) CreateEffect(MotionBlur);
	if (TheSettingManager->SettingsMain.EnableLowHF) CreateEffect(LowHF);
	if (TheSettingManager->SettingsMain.EnableWetWorld) CreateEffect(WetWorld);
	if (TheSettingManager->SettingsMain.EnableSharpening) CreateEffect(Sharpening);
	if (TheSettingManager->SettingsMain.CustomEffects) CreateEffect(Custom);

	IsInitialized = true;

}

void EffectManager::Render() {

	IDirect3DDevice9 *D3DDevice = TheRenderManager->device;
	IDirect3DSurface9* RenderTo = TheRenderManager->LastFrame;
	
	TheRenderManager->SetCameraData();
	D3DDevice->SetStreamSource(0, EffectVertex, 0, sizeof(EffectQuad));
	SetCurrentVertexStream;
	D3DDevice->StretchRect(RenderTo, NULL, RenderSurface, NULL, D3DTEXF_NONE);

	if (TheSettingManager->SettingsMain.EnableWetWorld && TheShaderManager->ShaderConst.currentWorldSpace && TheShaderManager->ShaderConst.WetWorld_RainAmount > 0) {
		D3DDevice->StretchRect(RenderTo, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		WetWorldEffect->ApplyConstants(WetWorld);
		WetWorldEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}
	else if (TheSettingManager->SettingsMain.EnableSnowAccumulation && TheShaderManager->ShaderConst.currentWorldSpace && TheShaderManager->ShaderConst.SnowAccumulation_Params.w > 0) {
		D3DDevice->StretchRect(RenderTo, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		SnowAccumulationEffect->ApplyConstants(SnowAccumulation);
		SnowAccumulationEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableColoring) {
		ColoringEffect->ApplyConstants(Coloring);
		ColoringEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableBloom) {
		D3DDevice->StretchRect(RenderTo, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		BloomEffect->ApplyConstants(Bloom);
		BloomEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableUnderwater && TheShaderManager->ShaderConst.HasWater && TheRenderManager->CameraPosition.z < TheShaderManager->ShaderConst.WaterHeight + 20)
	{
		if (TheShaderManager->ShaderConst.BloodLens_Percent) TheShaderManager->ShaderConst.BloodLens_Percent = 0.0f;
		TheShaderManager->ShaderConst.WaterLens_Percent = -1.0f;
		UnderwaterEffect->ApplyConstants(Underwater);
		UnderwaterEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}
	else
	{
		if (TheShaderManager->ShaderConst.WaterLens_Percent == -1.0f) TheShaderManager->ShaderConst.WaterLens_Percent = 1.0f;

		if (TheSettingManager->SettingsMain.EnableAmbientOcclusion && TheShaderManager->ShaderConst.AmbientOcclusion_Enabled) {
			D3DDevice->StretchRect(RenderTo, NULL, SourceSurface, NULL, D3DTEXF_NONE);
			AmbientOcclusionEffect->ApplyConstants(AmbientOcclusion);
			AmbientOcclusionEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
		}

		if (TheSettingManager->SettingsMain.EnableGodRays && TheShaderManager->ShaderConst.currentWorldSpace)
		{
			D3DDevice->StretchRect(RenderTo, NULL, SourceSurface, NULL, D3DTEXF_NONE);
			GodRaysEffect->ApplyConstants(GodRays);
			GodRaysEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
		}
	}

	if (TheSettingManager->SettingsMain.EnableDepthOfField && TheShaderManager->ShaderConst.DepthOfField_Enabled)
	{
		D3DDevice->StretchRect(RenderTo, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		DepthOfFieldEffect->ApplyConstants(DepthOfField);
		DepthOfFieldEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableBloodLens && TheShaderManager->ShaderConst.BloodLens_Percent > 0.0f) {
		BloodLensEffect->ApplyConstants(BloodLens);
		BloodLensEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableWaterLens && TheShaderManager->ShaderConst.WaterLens_Percent > 0.0f) {
		WaterLensEffect->ApplyConstants(WaterLens);
		WaterLensEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}
	
	if (TheSettingManager->SettingsMain.EnableMotionBlur && (TheShaderManager->ShaderConst.MotionBlur_AmountX || TheShaderManager->ShaderConst.MotionBlur_AmountY)) {
		MotionBlurEffect->ApplyConstants(MotionBlur);
		MotionBlurEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableLowHF && TheShaderManager->ShaderConst.LowHF_Params.x) {
		LowHFEffect->ApplyConstants(LowHF);
		LowHFEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableSharpening) {
		SharpeningEffect->ApplyConstants(Sharpening);
		SharpeningEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}

	if (TheSettingManager->SettingsMain.EnableCinema && (TheShaderManager->ShaderConst.Cinema_AspectRatio != 0.0f || TheShaderManager->ShaderConst.Cinema_VignetteRadius != 0.0f)) {
		CinemaEffect->ApplyConstants(Cinema);
		CinemaEffect->Render(D3DDevice, RenderTo, RenderSurface, false);
	}
	
	if (TheSettingManager->SettingsMain.CustomEffects) {
		for (CustomEffectRecordList::iterator iter = CustomEffectRecords.begin(); iter != CustomEffectRecords.end(); ++iter) {
			if (iter->second->Enabled) {
				D3DDevice->StretchRect(RenderTo, NULL, SourceSurface, NULL, D3DTEXF_NONE);
				iter->second->ApplyConstants(Custom);
				iter->second->Render(D3DDevice, RenderTo, RenderSurface, false);
			}
		}
	}

	if (TheSettingManager->SettingsMain.EnableSMAA) {
		//SMAA effect needs an A8R8G8B8 surface as rendertarget while the RenderTo surface is a X8R8G8B8; we need to switch the rendersurface.
		D3DDevice->StretchRect(RenderTo, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		D3DDevice->SetRenderTarget(0, RenderToSurface);
		SMAAEffect->Render(D3DDevice, RenderToSurface, RenderSurface, true);
		D3DDevice->StretchRect(RenderToSurface, NULL, RenderTo, NULL, D3DTEXF_NONE);
		D3DDevice->SetRenderTarget(0, RenderTo);
	}

	if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.ScreenshotKey)) {
		char Filename[MAX_PATH];
		char Name[80];

		strcpy(Filename, TheSettingManager->SettingsMain.ScreenshotPath);
		strcat(Filename, ScreenshotFilenamePrefix);
		strftime(Name, 80, "%Y%m%d %H.%M.%S", localtime(&TheShaderManager->currentTime));
		strcat(Filename, Name);
		if (TheSettingManager->SettingsMain.ScreenshotType == 0)
			strcat(Filename, ".bmp");
		else
			strcat(Filename, ".jpg");
		if (GetFileAttributesA(TheSettingManager->SettingsMain.ScreenshotPath) == INVALID_FILE_ATTRIBUTES) CreateDirectoryA(TheSettingManager->SettingsMain.ScreenshotPath, NULL);
		D3DXSaveSurfaceToFileA(Filename, (D3DXIMAGE_FILEFORMAT)TheSettingManager->SettingsMain.ScreenshotType, RenderTo, NULL, NULL);
		TheUtilityManager->ShowMessage("Screenshot taken!");
	}

}

void EffectManager::CreateEffect(EffectTypeEnum EffectType) {
	
	char Filename[MAX_PATH];
	
	strcpy(Filename, EffectsPath);
	switch (EffectType)
	{
		case Underwater:
			strcat(Filename,"Water\\Underwater.fx");
			UnderwaterEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableUnderwater = LoadEffect(UnderwaterEffect, Filename, NULL);
			break;
		case WaterLens:
			strcat(Filename,"Water\\WaterLens.fx");
			WaterLensEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableWaterLens = LoadEffect(WaterLensEffect, Filename, NULL);
			break;
		case GodRays:
			strcat(Filename,"GodRays\\GodRays.fx");
			GodRaysEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableGodRays = LoadEffect(GodRaysEffect, Filename, NULL);
			break;
		case DepthOfField:
			strcat(Filename,"DepthOfField\\DepthOfField.fx");
			DepthOfFieldEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableDepthOfField = LoadEffect(DepthOfFieldEffect, Filename, NULL);
			break;
		case AmbientOcclusion:
			strcat(Filename,"AmbientOcclusion\\AmbientOcclusion.fx");
			AmbientOcclusionEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableAmbientOcclusion = LoadEffect(AmbientOcclusionEffect, Filename, NULL);
			break;
		case Coloring:
			strcat(Filename,"Coloring\\Coloring.fx");
			ColoringEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableColoring = LoadEffect(ColoringEffect, Filename, NULL);
			break;
		case Cinema:
			strcat(Filename,"Cinema\\Cinema.fx");
			CinemaEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableCinema = LoadEffect(CinemaEffect, Filename, NULL);
			break;
		case Bloom:
			strcat(Filename,"Bloom\\Bloom.fx");
			BloomEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableBloom = LoadEffect(BloomEffect, Filename, NULL);
			break;
		case SnowAccumulation:
			strcat(Filename,"Precipitations\\SnowAccumulation.fx");
			SnowAccumulationEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableSnowAccumulation = LoadEffect(SnowAccumulationEffect, Filename, NULL);
			break;
		case BloodLens:
			strcat(Filename,"Blood\\BloodLens.fx");
			BloodLensEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableBloodLens = LoadEffect(BloodLensEffect, Filename, NULL);
			break;
		case SMAA:
			strcat(Filename,"SMAA\\SMAA.fx");
			SMAAEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableSMAA = LoadEffect(SMAAEffect, Filename, NULL);
			break;
		case MotionBlur:
			strcat(Filename,"MotionBlur\\MotionBlur.fx");
			MotionBlurEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableMotionBlur = LoadEffect(MotionBlurEffect, Filename, NULL);
			break;
		case LowHF:
			strcat(Filename,"LowHF\\LowHF.fx");
			LowHFEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableLowHF = LoadEffect(LowHFEffect, Filename, NULL);
			break;
		case WetWorld:
			strcat(Filename,"Precipitations\\WetWorld.fx");
			WetWorldEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableWetWorld = LoadEffect(WetWorldEffect, Filename, NULL);
			break;
		case Sharpening:
			strcat(Filename,"Sharpening\\Sharpening.fx");
			SharpeningEffect = new EffectRecord();
			TheSettingManager->SettingsMain.EnableSharpening = LoadEffect(SharpeningEffect, Filename, NULL);
			break;
		case Custom:
			WIN32_FIND_DATA File;
			HANDLE H;

			if (TheSettingManager->SettingsMain.DevelopCompileEffects)
				strcat(Filename,"Custom\\*.hlsl");
			else
				strcat(Filename,"Custom\\*.fx");
			H = FindFirstFileA(Filename, &File);
			if (H != INVALID_HANDLE_VALUE) {
				if (TheSettingManager->SettingsMain.DevelopCompileEffects) File.cFileName[strlen(File.cFileName) - 5] = '\0';
				strcpy(Filename, EffectsPath);
				strcat(Filename, "Custom\\");
				strcat(Filename, File.cFileName);
				CustomEffect = new EffectRecord();
				LoadEffect(CustomEffect, Filename, File.cFileName);
				while (FindNextFileA(H, &File)) {
					if (TheSettingManager->SettingsMain.DevelopCompileEffects) File.cFileName[strlen(File.cFileName) - 5] = '\0';
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

bool EffectManager::LoadEffect(EffectRecord* TheEffect, char* Filename, char* CustomEffectName) {
	
	bool IsLoaded = TheEffect->LoadEffect(Filename);

	if (IsLoaded) {
		TheEffect->ApplyPermanents(RenderTexture, SourceTexture);
		if (CustomEffectName)
			CustomEffectRecords[std::string(CustomEffectName).substr(0, strlen(CustomEffectName) - 3)] = TheEffect;
	}
	else
		DisposeEffect(TheEffect);
	return IsLoaded;

}

void EffectManager::DisposeEffect(EffectRecord* TheEffect)
{

	if (TheEffect == AmbientOcclusionEffect)
		AmbientOcclusionEffect = NULL;
	else if (TheEffect == BloodLensEffect)
		BloodLensEffect = NULL;
	else if (TheEffect == BloomEffect)
		BloomEffect = NULL;
	else if (TheEffect == CinemaEffect)
		CinemaEffect = NULL;
	else if (TheEffect == ColoringEffect)
		ColoringEffect = NULL;
	else if (TheEffect == DepthOfFieldEffect)
		DepthOfFieldEffect = NULL;
	else if (TheEffect == GodRaysEffect)
		GodRaysEffect = NULL;
	else if (TheEffect == LowHFEffect)
		LowHFEffect = NULL;
	else if (TheEffect == MotionBlurEffect)
		MotionBlurEffect = NULL;
	else if (TheEffect == SMAAEffect)
		SMAAEffect = NULL;
	else if (TheEffect == SnowAccumulationEffect)
		SnowAccumulationEffect = NULL;
	else if (TheEffect == UnderwaterEffect)
		UnderwaterEffect = NULL;
	else if (TheEffect == WaterLensEffect)
		WaterLensEffect = NULL;
	else if (TheEffect == WetWorldEffect)
		WetWorldEffect = NULL;
	else if (TheEffect == SharpeningEffect)
		SharpeningEffect = NULL;

	if (TheEffect) delete TheEffect;

}

float EffectManager::GetCustomEffectValue(const char* Name, const char* Const)
{
	float Value;

	CustomEffectRecordList::iterator v = CustomEffectRecords.find(std::string(Name));
	if (v != CustomEffectRecords.end()) {
		if (!strcmp(Const, "Enabled"))
			Value = v->second->Enabled;
		else
			v->second->GetValue(Const, &Value);
	}
	return Value;

}

void EffectManager::SetCustomEffectValue(const char* Name, const char* Const, float Value)
{

	CustomEffectRecordList::iterator v = CustomEffectRecords.find(std::string(Name));
	if (v != CustomEffectRecords.end()) {
		if (!strcmp(Const, "Enabled"))
			v->second->Enabled = Value;
		else
			v->second->SetValue(Const, Value);
	}

}

void EffectManager::SetEffectEnabled(const char* Name, bool Value)
{

	if (!strcmp(Name, "AmbientOcclusion")) {
		TheSettingManager->SettingsMain.EnableAmbientOcclusion = Value;
		DisposeEffect(AmbientOcclusionEffect);
		if (Value) CreateEffect(AmbientOcclusion);
	}
	else if (!strcmp(Name, "BloodLens")) {
		TheSettingManager->SettingsMain.EnableBloodLens = Value;
		DisposeEffect(BloodLensEffect);
		if (Value) CreateEffect(BloodLens);
	}
	else if (!strcmp(Name, "Bloom")) {
		TheSettingManager->SettingsMain.EnableBloom = Value;
		DisposeEffect(BloomEffect);
		if (Value) CreateEffect(Bloom);
	}
	else if (!strcmp(Name, "Cinema")) {
		TheSettingManager->SettingsMain.EnableCinema = Value;
		DisposeEffect(CinemaEffect);
		if (Value) CreateEffect(Cinema);
	}
	else if (!strcmp(Name, "Coloring")) {
		TheSettingManager->SettingsMain.EnableColoring = Value;
		DisposeEffect(ColoringEffect);
		if (Value) CreateEffect(Coloring);
	}
	else if (!strcmp(Name, "DepthOfField")) {
		TheSettingManager->SettingsMain.EnableDepthOfField = Value;
		DisposeEffect(DepthOfFieldEffect);
		if (Value) CreateEffect(DepthOfField);
	}
	else if (!strcmp(Name, "GodRays")) {
		TheSettingManager->SettingsMain.EnableGodRays = Value;
		DisposeEffect(GodRaysEffect);
		if (Value) CreateEffect(GodRays);
	}
	else if (!strcmp(Name, "LowHF")) {
		TheSettingManager->SettingsMain.EnableLowHF = Value;
		DisposeEffect(LowHFEffect);
		if (Value) CreateEffect(LowHF);
	}
	else if (!strcmp(Name, "MotionBlur")) {
		TheSettingManager->SettingsMain.EnableMotionBlur = Value;
		DisposeEffect(MotionBlurEffect);
		if (Value) CreateEffect(MotionBlur);
	}
	else if (!strcmp(Name, "SMAA")) {
		TheSettingManager->SettingsMain.EnableSMAA = Value;
		DisposeEffect(SMAAEffect);
		if (Value) CreateEffect(SMAA);
	}
	else if (!strcmp(Name, "SnowAccumulation")) {
		TheSettingManager->SettingsMain.EnableSnowAccumulation = Value;
		DisposeEffect(SnowAccumulationEffect);
		if (Value) CreateEffect(SnowAccumulation);
	}
	else if (!strcmp(Name, "Underwater")) {
		TheSettingManager->SettingsMain.EnableUnderwater = Value;
		DisposeEffect(UnderwaterEffect);
		if (Value) CreateEffect(Underwater);
	}
	else if (!strcmp(Name, "WaterLens")) {
		TheSettingManager->SettingsMain.EnableWaterLens = Value;
		DisposeEffect(WaterLensEffect);
		if (Value) CreateEffect(WaterLens);
	}
	else if (!strcmp(Name, "WetWorld")) {
		TheSettingManager->SettingsMain.EnableWetWorld = Value;
		DisposeEffect(WetWorldEffect);
		if (Value) CreateEffect(WetWorld);
	}
	else if (!strcmp(Name, "Sharpening")) {
		TheSettingManager->SettingsMain.EnableSharpening = Value;
		DisposeEffect(SharpeningEffect);
		if (Value) CreateEffect(Sharpening);
	}

}