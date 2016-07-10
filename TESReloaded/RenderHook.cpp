#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#define kMainPass 0x0040C830
#define kSetupShaderPrograms 0x0075FBA0
#define kProcessImageSpaceShaders 0x007B48E0
#define kBeginScene 0x0076BE00;
static const UInt32 kGetDepthBufferHook = 0x0040CCE8;
static const UInt32 kGetDepthBufferReturn = 0x0040CCEF;
#elif defined(SKYRIM)
#include "skse\SafeWrite.h"
#define kMainPass 0x0069BDF0
#define kSetupShaderPrograms 0x00CC4E80
#define kProcessImageSpaceShaders 0x00C70DA0
#define kBeginScene 0x00CDA620;
static const UInt32 kGetDepthBufferHook = 0x0069BCBD;
static const UInt32 kGetDepthBufferReturn = 0x0069BCC2;
#endif
#include "RenderHook.h"
#include "Hooking\detours\detours.h"

static bool FarPlaneJump = 0;

class RenderHook {

public:
	bool TrackBeginScene();
#if defined (OBLIVION)
	void TrackMainPass(BSRenderedTexture* RenderedTexture);
	UInt32 TrackSetupShaderPrograms(NiGeometry* Geometry, NiSkinInstance* SkinInstance, UInt32 SkinPartition, UInt32 GeometryBufferData, NiPropertyState* pkState, NiDynamicEffectState* pkEffects, UInt32 WorldTransform, UInt32 WorldBound, UInt32 Pass);
#elif defined(SKYRIM)
	void TrackMainPass(BSRenderedTexture* RenderedTexture, int Arg2, int Arg3);
	bool TrackSetupShaderPrograms(UInt32 Arg1, UInt32 Arg2);
	HRESULT TrackSetShaderMultiConstantF();
#endif
	
#if defined (OBLIVION)
	void TrackCulling(NiCullingProcess* cProcess);
	void TrackCullingBSFadeNode(NiCullingProcess* cProcess);
	float TrackFarPlane();
	IDirect3DBaseTexture9* TrackPrepareTextureForRendering(NiTexture* Texture, UInt32 Arg2, UInt32 Arg3, UInt32 Arg4);
#endif
};

#if defined(OBLIVION)
void (__thiscall RenderHook::* MainPass)(BSRenderedTexture*);
void (__thiscall RenderHook::* TrackMainPass)(BSRenderedTexture*);
void RenderHook::TrackMainPass(BSRenderedTexture* RenderedTexture) {

	TheRenderManager->SetFoV();
	TheRenderManager->SetNearDistance();
	TheShaderManager->UpdateFrameConstants();
	(this->*MainPass)(RenderedTexture);

}

void (__thiscall RenderHook::* Culling)(NiCullingProcess*);
void (__thiscall RenderHook::* TrackCulling)(NiCullingProcess*);
void RenderHook::TrackCulling(NiCullingProcess* cProcess) {
	
	NiAVObject* Object = (NiAVObject*)this;
	
	if (Object && Object->m_pcName) {
		if (TheSettingManager->SettingsMain.DevelopTraceCulling) _MESSAGE("Culling: %s", Object->m_pcName);
		if (TheShaderManager->ShaderConst.currentWorldSpace == NULL && (!memcmp(Object->m_pcName,"Current Precip Root",19) || !memcmp(Object->m_pcName,"Last Precip Root",16))) return;
	}
	(this->*Culling)(cProcess);

}

void (__thiscall RenderHook::* CullingBSFadeNode)(NiCullingProcess*);
void (__thiscall RenderHook::* TrackCullingBSFadeNode)(NiCullingProcess*);
void RenderHook::TrackCullingBSFadeNode(NiCullingProcess* cProcess) {

	BSFadeNode* Object = (BSFadeNode*)this;

	if (TheSettingManager->SettingsMain.FrameRate && TheShaderManager->ShaderConst.currentWorldSpace)
		if (Object->cMultType == 6 && !strstr(Object->m_pcName,"ImperialCity") && (TheFrameRateManager->IsOutGrid(Object))) return;
	(this->*CullingBSFadeNode)(cProcess);

}

float (__thiscall RenderHook::* FarPlane)();
float (__thiscall RenderHook::* TrackFarPlane)();
float RenderHook::TrackFarPlane() {
	
	float fp = (this->*FarPlane)();

	if (TheSettingManager->SettingsMain.FarPlaneDistance && fp == 283840.0f) fp = TheSettingManager->SettingsMain.FarPlaneDistance;
	return fp;

}

IDirect3DBaseTexture9* (__thiscall RenderHook::* PrepareTextureForRendering)(NiTexture*, UInt32, UInt32, UInt32);
IDirect3DBaseTexture9* (__thiscall RenderHook::* TrackPrepareTextureForRendering)(NiTexture*, UInt32, UInt32, UInt32);
IDirect3DBaseTexture9* RenderHook::TrackPrepareTextureForRendering(NiTexture* Texture, UInt32 Arg2, UInt32 Arg3, UInt32 Arg4) {
	
	TheShaderManager->ShaderConst.TextureData.x = Texture->GetWidth();
	TheShaderManager->ShaderConst.TextureData.y = Texture->GetHeight();
	return (this->*PrepareTextureForRendering)(Texture, Arg2, Arg3, Arg4);

}

UInt32 (__thiscall RenderHook::* SetupShaderPrograms)(NiGeometry*, NiSkinInstance*, UInt32, UInt32, NiPropertyState*, NiDynamicEffectState*, UInt32, UInt32, UInt32);
UInt32 (__thiscall RenderHook::* TrackSetupShaderPrograms)(NiGeometry*, NiSkinInstance*, UInt32, UInt32, NiPropertyState*, NiDynamicEffectState*, UInt32, UInt32, UInt32);
UInt32 RenderHook::TrackSetupShaderPrograms(NiGeometry* Geometry, NiSkinInstance* SkinInstance, UInt32 SkinPartition, UInt32 GeometryBufferData, NiPropertyState* pkState, NiDynamicEffectState* pkEffects, UInt32 WorldTransform, UInt32 WorldBound, UInt32 Pass) {

	NiD3DPass* pPass = (NiD3DPass*)this;
	NiD3DVertexShaderEx* VertexShader = (NiD3DVertexShaderEx*)pPass->VertexShader;
	NiD3DPixelShaderEx* PixelShader = (NiD3DPixelShaderEx*)pPass->PixelShader;
	
	if (VertexShader) {
		if (VertexShader->ShaderRecord) {
			VertexShader->ShaderRecord->SetCT();
			if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("OR SetVertexShader: %s", VertexShader->ShaderRecord->Name);
		}
		else if (TheSettingManager->SettingsMain.DevelopTraceVanillaShaders) _MESSAGE("SetVertexShader: %s", VertexShader->ShaderName);
	}
	if (PixelShader) {
		if (PixelShader->ShaderRecord) {
			PixelShader->ShaderRecord->SetCT();
			if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("OR SetPixelShader: %s", PixelShader->ShaderRecord->Name);
		}
		else if (TheSettingManager->SettingsMain.DevelopTraceVanillaShaders) _MESSAGE("SetPixelShader: %s", PixelShader->ShaderName);
	}
	return (this->*SetupShaderPrograms)(Geometry, SkinInstance, SkinPartition, GeometryBufferData, pkState, pkEffects, WorldTransform, WorldBound, Pass);

}

NiPixelData* (__cdecl * SaveGameScreenshot)(int*, int*) = (NiPixelData* (__cdecl *)(int*, int*))0x00411B70;
NiPixelData* __cdecl TrackSaveGameScreenshot(int* pWidth, int* pHeight) {
	
	NiPixelData* r;
	
	TheRenderManager->IsSaveGameScreenShot = 1;
	r = SaveGameScreenshot(pWidth, pHeight);
	TheRenderManager->IsSaveGameScreenShot = 0;
	return r;

}
#elif defined(SKYRIM)

void (__thiscall RenderHook::* MainPass)(BSRenderedTexture*, int, int);
void (__thiscall RenderHook::* TrackMainPass)(BSRenderedTexture*, int, int);
void RenderHook::TrackMainPass(BSRenderedTexture* RenderedTexture, int Arg2, int Arg3) {

	TheRenderManager->SetFoV();
	TheRenderManager->SetNearDistance();
	TheShaderManager->UpdateFrameConstants();
	(this->*MainPass)(RenderedTexture, Arg2, Arg3);

}

bool (__thiscall RenderHook::* SetupShaderPrograms)(UInt32, UInt32);
bool (__thiscall RenderHook::* TrackSetupShaderPrograms)(UInt32, UInt32);
bool RenderHook::TrackSetupShaderPrograms(UInt32 Arg1, UInt32 Arg2) {

	bool r = (this->*SetupShaderPrograms)(Arg1, Arg2);
	NiD3DVertexShaderEx* VertexShader = *(NiD3DVertexShaderEx**)0x01BABFB4;
	NiD3DPixelShaderEx* PixelShader = *(NiD3DPixelShaderEx**)0x01BABFB0;
	
	if (VertexShader) {
		if (VertexShader->ShaderRecord) {
			VertexShader->ShaderRecord->SetCT();
			if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("SR SetVertexShader: %s", VertexShader->ShaderRecord->Name);
		}
		else if (TheSettingManager->SettingsMain.DevelopTraceVanillaShaders && !strstr(VertexShader->ShaderName, "SHADER")) _MESSAGE("SetVertexShader: %s", VertexShader->ShaderName);
	}
	if (PixelShader) {
		if (PixelShader->ShaderRecord) {
			PixelShader->ShaderRecord->SetCT();
			if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("SR SetPixelShader: %s", PixelShader->ShaderRecord->Name);
		}
		else if (TheSettingManager->SettingsMain.DevelopTraceVanillaShaders && !strstr(PixelShader->ShaderName, "SHADER")) _MESSAGE("SetPixelShader: %s", PixelShader->ShaderName);
	}
	return r;

}

HRESULT (__thiscall RenderHook::* SetShaderMultiConstantF)();
HRESULT (__thiscall RenderHook::* TrackSetShaderMultiConstantF)();
HRESULT RenderHook::TrackSetShaderMultiConstantF() {

	NiD3DPixelShaderEx* PixelShader = *(NiD3DPixelShaderEx**)0x01BABFB0;

	if (PixelShader && PixelShader->ShaderRecord && PixelShader->StartRegister == 0) {		
		if (!memcmp(PixelShader->ShaderRecord->Name, "WATER5.pso", 10)) {
			D3DXVECTOR4* ConstantData = (D3DXVECTOR4*)0x01BAE0A8;
			ConstantData += 4;
			TheShaderManager->ShaderConst.Water_shallowColor = *ConstantData; // Stores the ShallowColor (register 4) to pass it to the underwater
			ConstantData += 1;
			TheShaderManager->ShaderConst.Water_depthColor = *ConstantData; // Stores the DeepColor (register 5) to pass it to the underwater
		}
	}
	return (this->*SetShaderMultiConstantF)();

}
#endif

bool (__thiscall RenderHook::* BeginScene)();
bool (__thiscall RenderHook::* TrackBeginScene)();
bool RenderHook::TrackBeginScene() {

	TheShaderManager->BeginScene();
	return (this->*BeginScene)();

}

void (__cdecl * ProcessImageSpaceShaders)(NiDX9Renderer*, BSRenderedTexture*, BSRenderedTexture*) = (void (__cdecl *)(NiDX9Renderer*, BSRenderedTexture*, BSRenderedTexture*))kProcessImageSpaceShaders;
void __cdecl TrackProcessImageSpaceShaders(NiDX9Renderer* Renderer, BSRenderedTexture* RenderedTexture1, BSRenderedTexture* RenderedTexture2) {

	Ni2DBuffer* RenderBuffer = NULL;

	ProcessImageSpaceShaders(Renderer, RenderedTexture1, RenderedTexture2);
	if (TheRenderManager->currentRTGroup) {
		RenderBuffer = TheRenderManager->currentRTGroup->targets[0];
		if (RenderBuffer->width == TheRenderManager->width && RenderBuffer->height == TheRenderManager->height) {
			TheRenderManager->LastFrame = RenderBuffer->data->Surface;
			if (!TheEffectManager->IsInitialized) TheEffectManager->Initialize();
			TheEffectManager->Render();
		}
		else if (TheRenderManager->IsSaveGameScreenShot)
			TheRenderManager->device->StretchRect(TheRenderManager->LastFrame, NULL, RenderBuffer->data->Surface, &TheRenderManager->SaveGameScreenShotRECT, D3DTEXF_NONE);
	}

}

void GetDepthBuffer()
{
	TheRenderManager->ResolveDepthBuffer();
}

static __declspec(naked) void GetDepthBufferHook()
{
	__asm
	{
		pushad
		call	GetDepthBuffer
		popad
#if defined (OBLIVION)
		mov		eax, 0x00B42F3E
		cmp     byte ptr [eax], 0
#elif defined (SKYRIM)
		cmp     byte ptr [esp+0x14], 0
#endif
		jmp		kGetDepthBufferReturn
	}
}

void CreateRenderHook() {

	*((int *)&MainPass)						= kMainPass;
	TrackMainPass							= &RenderHook::TrackMainPass;
	*((int *)&SetupShaderPrograms)			= kSetupShaderPrograms;
	TrackSetupShaderPrograms				= &RenderHook::TrackSetupShaderPrograms;
	*((int *)&BeginScene)					= kBeginScene;
	TrackBeginScene							= &RenderHook::TrackBeginScene;
#if defined(OBLIVION)
	*((int *)&Culling)						= 0x0070AB40;
	TrackCulling							= &RenderHook::TrackCulling;
	*((int *)&CullingBSFadeNode)			= 0x004A0920;
	TrackCullingBSFadeNode					= &RenderHook::TrackCullingBSFadeNode;
	*((int *)&FarPlane)						= 0x00410EE0;
	TrackFarPlane							= &RenderHook::TrackFarPlane;
	*((int *)&PrepareTextureForRendering)	= 0x0077AC90;
	TrackPrepareTextureForRendering			= &RenderHook::TrackPrepareTextureForRendering;
#elif defined(SKYRIM)
	*((int *)&SetShaderMultiConstantF)		= 0x00CC4BF0;
	TrackSetShaderMultiConstantF			= &RenderHook::TrackSetShaderMultiConstantF;
#endif

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)MainPass,						*((PVOID *)&TrackMainPass));
	DetourAttach(&(PVOID&)SetupShaderPrograms,			*((PVOID *)&TrackSetupShaderPrograms));
	DetourAttach(&(PVOID&)BeginScene,					*((PVOID *)&TrackBeginScene));
	DetourAttach(&(PVOID&)ProcessImageSpaceShaders,				   &TrackProcessImageSpaceShaders);
#if defined(OBLIVION)
	DetourAttach(&(PVOID&)Culling,						*((PVOID *)&TrackCulling));
	DetourAttach(&(PVOID&)CullingBSFadeNode,			*((PVOID *)&TrackCullingBSFadeNode));
	DetourAttach(&(PVOID&)FarPlane,						*((PVOID *)&TrackFarPlane));
	DetourAttach(&(PVOID&)PrepareTextureForRendering,	*((PVOID *)&TrackPrepareTextureForRendering));
	DetourAttach(&(PVOID&)SaveGameScreenshot,					   &TrackSaveGameScreenshot);
#elif defined(SKYRIM)
	DetourAttach(&(PVOID&)SetShaderMultiConstantF,		*((PVOID *)&TrackSetShaderMultiConstantF));
#endif
    DetourTransactionCommit();
	
	WriteRelJump(kGetDepthBufferHook, (UInt32)GetDepthBufferHook);
#if defined(OBLIVION)
	SafeWrite8(0x00A38280, 0x5A); // Fixes the "purple water bug"
	SafeWrite32(0x0049BFAF, TheSettingManager->SettingsMain.WaterReflectionMapSize);
	*LocalWaterHiRes = 1; //Fixes a bug on the WaterHeightMapConstructor (the constructor is called before initializing the setting with the bUseWaterHiRes ini value)
#endif

}
