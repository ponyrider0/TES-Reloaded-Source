#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#define kInitializeRenderer 0x004052F0
#define kRender 0x0040C830
#define kSetupShaderPrograms 0x0075FBA0
#define kProcessImageSpaceShaders 0x007B48E0
#define kBeginScene 0x0076BE00
static const UInt32 kGetDepthBufferHook = 0x0040CCE8;
static const UInt32 kGetDepthBufferReturn = 0x0040CCEF;
static const UInt32 kSetGraphicsInitialization = 0x00498893;
static const UInt32 kSetGraphicsInitializationReturn = 0x0049896A;
#elif defined(SKYRIM)
#include "skse\SafeWrite.h"
#define kInitializeRenderer 0x00691030
#define kRender 0x0069BDF0
#define kSetupShaderPrograms 0x00CC4E80
#define kProcessImageSpaceShaders 0x00C70DA0
#define kBeginScene 0x00CDA620
static const UInt32 kGetDepthBufferHook = 0x0069BCBD;
static const UInt32 kGetDepthBufferReturn = 0x0069BCC2;
static const UInt32 kSetShadowDistance = 0x00CBB140;
static const UInt32 kSetShadowDistanceReturn = 0x00CBB146;
static const UInt32 kSetShadowDistanceShader = 0x00CB7365;
static const UInt32 kSetShadowDistanceShaderReturn = 0x00CB736D;
#endif
#include "RenderHook.h"
#include "Hooking\detours\detours.h"

static bool FarPlaneJump = 0;

class RenderHook {

public:
	NiDX9Renderer* TrackInitializeRenderer();
	bool TrackBeginScene();
#if defined (OBLIVION)
	void TrackMainRender();
	void TrackRender(BSRenderedTexture* RenderedTexture);
	UInt32 TrackSetupShaderPrograms(NiGeometry* Geometry, NiSkinInstance* SkinInstance, UInt32 SkinPartition, UInt32 GeometryBufferData, NiPropertyState* pkState, NiDynamicEffectState* pkEffects, UInt32 WorldTransform, UInt32 WorldBound, UInt32 Pass);
	void TrackCulling(NiCullingProcess* cProcess);
	void TrackCullingBSFadeNode(NiCullingProcess* cProcess);
	float TrackFarPlane();
	IDirect3DBaseTexture9* TrackPrepareTextureForRendering(NiTexture* Texture, UInt32 Arg2, UInt32 Arg3, UInt32 Arg4);
	HRESULT TrackSetSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type, UInt32 Value, UInt8 Save);
#elif defined(SKYRIM)
	void TrackRender(BSRenderedTexture* RenderedTexture, int Arg2, int Arg3);
	bool TrackSetupShaderPrograms(UInt32 Arg1, UInt32 Arg2);
#endif

};

NiDX9Renderer* (__thiscall RenderHook::* InitializeRenderer)();
NiDX9Renderer* (__thiscall RenderHook::* TrackInitializeRenderer)();
NiDX9Renderer* RenderHook::TrackInitializeRenderer() {
	
	NiDX9Renderer* Renderer = (this->*InitializeRenderer)();
	
	TheRenderManager = (RenderManager*)Renderer;
	TheRenderManager->Initialize();
	new TextureManager();
	new ShaderManager();
	new EquipmentManager();
	new FrameRateManager();
	new GameEventManager();
	new GameMenuManager();
	new KeyboardManager();
	new ScriptManager();
	return Renderer;

}

bool (__thiscall RenderHook::* BeginScene)();
bool (__thiscall RenderHook::* TrackBeginScene)();
bool RenderHook::TrackBeginScene() {

	TheShaderManager->BeginScene();
	return (this->*BeginScene)();

}

#if defined(OBLIVION)
void (__thiscall RenderHook::* MainRender)();
void (__thiscall RenderHook::* TrackMainRender)();
void RenderHook::TrackMainRender() {

	TheScriptManager->Run();
	(this->*MainRender)();

}

void (__thiscall RenderHook::* Render)(BSRenderedTexture*);
void (__thiscall RenderHook::* TrackRender)(BSRenderedTexture*);
void RenderHook::TrackRender(BSRenderedTexture* RenderedTexture) {
	
	TheRenderManager->SetSceneGraph();
	TheShaderManager->UpdateConstants();
	if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("RENDERING...");
	(this->*Render)(RenderedTexture);

}

void (__thiscall RenderHook::* Culling)(NiCullingProcess*);
void (__thiscall RenderHook::* TrackCulling)(NiCullingProcess*);
void RenderHook::TrackCulling(NiCullingProcess* cProcess) {
	
	NiAVObject* Object = (NiAVObject*)this;
	
	if (Object && Object->m_pcName) {
		if (TheShaderManager->ShaderConst.currentWorldSpace == NULL && (!memcmp(Object->m_pcName, "Current Precip Root", 19) || !memcmp(Object->m_pcName, "Last Precip Root", 16))) return;
	}
	(this->*Culling)(cProcess);

}

void (__thiscall RenderHook::* CullingBSFadeNode)(NiCullingProcess*);
void (__thiscall RenderHook::* TrackCullingBSFadeNode)(NiCullingProcess*);
void RenderHook::TrackCullingBSFadeNode(NiCullingProcess* cProcess) {

	BSFadeNode* Object = (BSFadeNode*)this;

	if (TheSettingManager->SettingsMain.FrameRate && TheShaderManager->ShaderConst.currentWorldSpace)
		if (Object->cMultType == 6 && !strstr(Object->m_pcName, "ImperialCity") && TheFrameRateManager->IsOutGrid(Object)) return;
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
	
	if (VertexShader && TheRenderManager->renderState->GetVertexShader() != VertexShader->ShaderHandle) {
		if (VertexShader->ShaderProg) VertexShader->ShaderProg->SetCT();
		if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("SetVertexShader: %s", VertexShader->ShaderName);
	}
	if (PixelShader && TheRenderManager->renderState->GetPixelShader() != PixelShader->ShaderHandle) {
		if (PixelShader->ShaderProg) PixelShader->ShaderProg->SetCT();
		if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("SetPixelShader: %s", PixelShader->ShaderName);
	}
	return (this->*SetupShaderPrograms)(Geometry, SkinInstance, SkinPartition, GeometryBufferData, pkState, pkEffects, WorldTransform, WorldBound, Pass);

}

HRESULT (__thiscall RenderHook::* SetSamplerState)(UInt32, D3DSAMPLERSTATETYPE, UInt32, UInt8);
HRESULT (__thiscall RenderHook::* TrackSetSamplerState)(UInt32, D3DSAMPLERSTATETYPE, UInt32, UInt8);
HRESULT RenderHook::TrackSetSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type, UInt32 Value, UInt8 Save) {

	if (TheRenderManager) TheRenderManager->SetSamplerState(Sampler, Type, &Value, false);
	return (this->*SetSamplerState)(Sampler, Type, Value, Save);

}

NiPixelData* (__cdecl * SaveGameScreenshot)(int*, int*) = (NiPixelData* (__cdecl *)(int*, int*))0x00411B70;
NiPixelData* __cdecl TrackSaveGameScreenshot(int* pWidth, int* pHeight) {
	
	NiPixelData* r;
	
	TheRenderManager->IsSaveGameScreenShot = 1;
	r = SaveGameScreenshot(pWidth, pHeight);
	TheRenderManager->IsSaveGameScreenShot = 0;
	return r;

}

static UInt32* ShaderPackageSelector1 = (UInt32*)0x00B42F48;
static UInt32* ShaderPackageSelector2 = (UInt32*)0x00B42E88;
static UInt32* ShaderPackageSelectorMax = (UInt32*)0x00B42D74;
static UInt32* ShaderPackageSelectorMask = (UInt32*)0x00B42F40;
static UInt8* ShaderPackageSelectorShader3 = (UInt8*)0x00B42EA5;
UInt32 (__cdecl * SetShaderPackageSelector)(int, int, UInt8, int, char*, int) = (UInt32 (__cdecl *)(int, int, UInt8, int, char*, int))0x007B45F0;
UInt32 __cdecl TrackSetShaderPackageSelector(int Arg1, int Arg2, UInt8 Force1XShaders, int Arg4, char* GraphicsDLL, int Arg6) {
	
	if (Arg1) {
		*ShaderPackageSelectorMask = 0x2F;
		*ShaderPackageSelectorMax = *ShaderPackageSelector1 = TheSettingManager->SettingsMain.ShaderProfile;
		*ShaderPackageSelectorShader3 = 1;
		*ShaderPackageSelector2 = 8;
	}
	return *ShaderPackageSelector1;

}

static __declspec(naked) void SetGraphicsInitialization()
{
	__asm
	{
		mov		byte ptr ds:[0x00B43075], 0
		mov		byte ptr ds:[0x00B42E97], 0
		jmp		kSetGraphicsInitializationReturn
	}
}
#elif defined(SKYRIM)

void (__thiscall RenderHook::* Render)(BSRenderedTexture*, int, int);
void (__thiscall RenderHook::* TrackRender)(BSRenderedTexture*, int, int);
void RenderHook::TrackRender(BSRenderedTexture* RenderedTexture, int Arg2, int Arg3) {

	TheRenderManager->SetSceneGraph();
	TheShaderManager->UpdateConstants();
	if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("RENDERING...");
	(this->*Render)(RenderedTexture, Arg2, Arg3);

}

bool (__thiscall RenderHook::* SetupShaderPrograms)(UInt32, UInt32);
bool (__thiscall RenderHook::* TrackSetupShaderPrograms)(UInt32, UInt32);
bool RenderHook::TrackSetupShaderPrograms(UInt32 Arg1, UInt32 Arg2) {

	bool r = (this->*SetupShaderPrograms)(Arg1, Arg2);
	NiD3DVertexShaderEx* VertexShader = *(NiD3DVertexShaderEx**)0x01BABFB4;
	NiD3DPixelShaderEx* PixelShader = *(NiD3DPixelShaderEx**)0x01BABFB0;
	
	if (VertexShader) {
		if (VertexShader->ShaderProg) VertexShader->ShaderProg->SetCT();
		if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("SetVertexShader: %s", VertexShader->ShaderName);
	}
	if (PixelShader) {
		if (PixelShader->ShaderProg) PixelShader->ShaderProg->SetCT();
		if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("SetPixelShader: %s", PixelShader->ShaderName);
	}
	return r;

}

void SetShadowDistanceValue(float* Distance, UInt32 Pass) {

	if (Pass == 0) *Distance /= TheSettingManager->SettingsMain.ShadowModeNearQuality;

}

static __declspec(naked) void SetShadowDistance()
{
	__asm
	{
		faddp   st(1), st
		fstp	dword ptr [esp + 0x4C]
		lea		ecx, [esp + 0x4C]
		mov		edx, [esp + 0x74]
		pushad
		pushfd
		push	edx
		push	ecx
		call	SetShadowDistanceValue
		add		esp, 8
		popfd
		popad
		jmp		kSetShadowDistanceReturn
	}
}

static __declspec(naked) void SetShadowDistanceShader()
{
	__asm
	{
		lea		ecx, [esp + 0xE0 - 0xC4 + 4]
		pushad
		pushfd
		push	0
		push	ecx
		call	SetShadowDistanceValue
		add		esp, 8
		popfd
		popad
		mov		ecx, [esp + 0xE0 - 0xC4 + 4]
		mov		[esp + esi * 4 + 0xE0 - 0x98], ecx
		jmp		kSetShadowDistanceShaderReturn
	}
}

#endif

void (__cdecl * ProcessImageSpaceShaders)(NiDX9Renderer*, BSRenderedTexture*, BSRenderedTexture*) = (void (__cdecl *)(NiDX9Renderer*, BSRenderedTexture*, BSRenderedTexture*))kProcessImageSpaceShaders;
void __cdecl TrackProcessImageSpaceShaders(NiDX9Renderer* Renderer, BSRenderedTexture* RenderedTexture1, BSRenderedTexture* RenderedTexture2) {

	Ni2DBuffer* RenderBuffer = NULL;
	
	ProcessImageSpaceShaders(Renderer, RenderedTexture1, RenderedTexture2);
	if (TheRenderManager->currentRTGroup) {
		RenderBuffer = TheRenderManager->currentRTGroup->RenderTargets[0];
		if (RenderBuffer->width == TheRenderManager->width && RenderBuffer->height == TheRenderManager->height) {
			TheRenderManager->RenderTarget = RenderBuffer->data->Surface;
			TheShaderManager->RenderEffects();
			TheGameMenuManager->Render();
		}
		else if (TheRenderManager->IsSaveGameScreenShot)
			TheRenderManager->device->StretchRect(TheRenderManager->RenderTarget, NULL, RenderBuffer->data->Surface, &TheRenderManager->SaveGameScreenShotRECT, D3DTEXF_NONE);
	}

}

static __declspec(naked) void GetDepthBufferHook()
{
	__asm
	{
		pushad
		mov		ecx, TheRenderManager
		call	RenderManager::ResolveDepthBuffer
		popad
#if defined (OBLIVION)
		mov		eax, 0x00B42F3E
		cmp     byte ptr[eax], 0
#elif defined (SKYRIM)
		cmp     byte ptr[esp + 0x14], 0
#endif
		jmp		kGetDepthBufferReturn
	}
}

void CreateRenderHook() {

	*((int *)&InitializeRenderer)			= kInitializeRenderer;
	TrackInitializeRenderer					= &RenderHook::TrackInitializeRenderer;
	*((int *)&Render)						= kRender;
	TrackRender								= &RenderHook::TrackRender;
	*((int *)&SetupShaderPrograms)			= kSetupShaderPrograms;
	TrackSetupShaderPrograms				= &RenderHook::TrackSetupShaderPrograms;
	*((int *)&BeginScene)					= kBeginScene;
	TrackBeginScene							= &RenderHook::TrackBeginScene;
#if defined(OBLIVION)
	*((int *)&MainRender)					= 0x0040D4D0;
	TrackMainRender							= &RenderHook::TrackMainRender;
	*((int *)&Culling)						= 0x0070AB40;
	TrackCulling							= &RenderHook::TrackCulling;
	*((int *)&CullingBSFadeNode)			= 0x004A0920;
	TrackCullingBSFadeNode					= &RenderHook::TrackCullingBSFadeNode;
	*((int *)&FarPlane)						= 0x00410EE0;
	TrackFarPlane							= &RenderHook::TrackFarPlane;
	*((int *)&PrepareTextureForRendering)	= 0x0077AC90;
	TrackPrepareTextureForRendering			= &RenderHook::TrackPrepareTextureForRendering;
	*((int *)&SetSamplerState)				= 0x0077B610;
	TrackSetSamplerState					= &RenderHook::TrackSetSamplerState;
#endif

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)InitializeRenderer,			*((PVOID *)&TrackInitializeRenderer));
	DetourAttach(&(PVOID&)Render,						*((PVOID *)&TrackRender));
	DetourAttach(&(PVOID&)SetupShaderPrograms,			*((PVOID *)&TrackSetupShaderPrograms));
	DetourAttach(&(PVOID&)BeginScene,					*((PVOID *)&TrackBeginScene));
	DetourAttach(&(PVOID&)ProcessImageSpaceShaders,				   &TrackProcessImageSpaceShaders);
#if defined(OBLIVION)
	DetourAttach(&(PVOID&)MainRender,					*((PVOID *)&TrackMainRender));
	DetourAttach(&(PVOID&)Culling,						*((PVOID *)&TrackCulling));
	DetourAttach(&(PVOID&)CullingBSFadeNode,			*((PVOID *)&TrackCullingBSFadeNode));
	DetourAttach(&(PVOID&)FarPlane,						*((PVOID *)&TrackFarPlane));
	DetourAttach(&(PVOID&)PrepareTextureForRendering,	*((PVOID *)&TrackPrepareTextureForRendering));
	DetourAttach(&(PVOID&)SetSamplerState,				*((PVOID *)&TrackSetSamplerState));
	DetourAttach(&(PVOID&)SaveGameScreenshot,					   &TrackSaveGameScreenshot);
	DetourAttach(&(PVOID&)SetShaderPackageSelector,				   &TrackSetShaderPackageSelector);
#endif
    DetourTransactionCommit();

	WriteRelJump(kGetDepthBufferHook,		(UInt32)GetDepthBufferHook); // Grabs the depth buffer for full screen shaders
#if defined(OBLIVION)
	WriteRelJump(kSetGraphicsInitialization, (UInt32)SetGraphicsInitialization);
	SafeWrite8(0x00A38280, 0x5A); // Fixes the "purple water bug"
	SafeWrite32(0x0049BFAF, TheSettingManager->SettingsMain.WaterReflectionMapSize); // Constructor
	SafeWrite32(0x007C1045, TheSettingManager->SettingsMain.WaterReflectionMapSize); // RenderedSurface
	SafeWrite32(0x007C104F, TheSettingManager->SettingsMain.WaterReflectionMapSize); // RenderedSurface
	SafeWrite32(0x007C10F9, TheSettingManager->SettingsMain.WaterReflectionMapSize); // RenderedSurface
	SafeWrite32(0x007C1103, TheSettingManager->SettingsMain.WaterReflectionMapSize); // RenderedSurface
	*LocalWaterHiRes = 1; //Fixes a bug on the WaterHeightMapConstructor (the constructor is called before initializing the setting with the bUseWaterHiRes ini value)
	if (TheSettingManager->SettingsMain.AnisotropicFilter >= 2) {
		SafeWrite8(0x007BE1D3, TheSettingManager->SettingsMain.AnisotropicFilter);
		SafeWrite8(0x007BE32B, TheSettingManager->SettingsMain.AnisotropicFilter);
	}
#elif defined(SKYRIM)
	if (TheSettingManager->SettingsMain.ShadowMode) {
		WriteRelJump(kSetShadowDistance,		(UInt32)SetShadowDistance);
		WriteRelJump(kSetShadowDistanceShader,	(UInt32)SetShadowDistanceShader);
	}
#endif

}
