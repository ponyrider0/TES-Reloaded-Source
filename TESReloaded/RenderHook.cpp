#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#define kInitializeRenderer 0x004052F0
#define kRender 0x0040C830
#define kSetupShaderPrograms 0x0075FBA0
#define kProcessImageSpaceShaders 0x007B48E0
#define kBeginScene 0x0076BE00
#define kMenuRenderedTexture 0x00B333E8
#elif defined(SKYRIM)
#include "skse\SafeWrite.h"
#include "skse\GameData.h"
#define kInitializeRenderer 0x00691030
#define kRender 0x0069BDF0
#define kSetupShaderPrograms 0x00CC4E80
#define kProcessImageSpaceShaders 0x00C70DA0
#define kBeginScene 0x00CDA620
#define kMenuRenderedTexture 0x01B2E8D8
static const UInt32 kRenderFirstPersonNode = 0x0069BD27;
static const UInt32 kRenderFirstPersonNodeRepeat = 0x0069BCC4;
static const UInt32 kRenderFirstPersonNodeReturn = 0x0069BD32;
static const UInt32 kClearDepth = 0x00698BBA;
static const UInt32 kClearDepthReturn = 0x00698BC3;
static const UInt32 kSetShadowDistance = 0x00CBB140;
static const UInt32 kSetShadowDistanceReturn = 0x00CBB146;
static const UInt32 kSetShadowDistanceShader = 0x00CB7365;
static const UInt32 kSetShadowDistanceShaderReturn = 0x00CB736D;
static const UInt32 kFixSunFlags = 0x0069A92F;
static const UInt32 kFixSunFlagsReturn = 0x0069A938;
static UInt32 ClearMode = 0;
#endif
#include "RenderHook.h"
#include "Hooking\detours\detours.h"

class RenderHook {

public:
	NiDX9Renderer* TrackInitializeRenderer();
	bool TrackBeginScene();
#if defined (OBLIVION)
	void TrackRender(BSRenderedTexture* RenderedTexture);
	bool TrackEndTargetGroup(NiCamera* Camera, NiRenderTargetGroup* TargetGroup);
	void TrackHDRRender(NiScreenElements* ScreenElements, BSRenderedTexture** RenderedTexture1, BSRenderedTexture** RenderedTexture2, UInt8 Arg4);
	UInt32 TrackSetupShaderPrograms(NiGeometry* Geometry, NiSkinInstance* SkinInstance, UInt32 SkinPartition, UInt32 GeometryBufferData, NiPropertyState* pkState, NiDynamicEffectState* pkEffects, UInt32 WorldTransform, UInt32 WorldBound, UInt32 Pass);
	void TrackCulling(NiCullingProcess* cProcess);
	void TrackCullingBSFadeNode(NiCullingProcess* cProcess);
	float TrackFarPlane();
	IDirect3DBaseTexture9* TrackPrepareTextureForRendering(NiTexture* Texture, UInt32 Arg2, UInt32 Arg3, UInt32 Arg4);
	HRESULT TrackSetSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type, UInt32 Value, UInt8 Save);
#elif defined(SKYRIM)
	void TrackRender(BSRenderedTexture* RenderedTexture, int Arg2, int Arg3);
	bool TrackSetupShaderPrograms(UInt32 Arg1, UInt32 Arg2);
	void TrackRenderWorldSceneGraph(Sun* SkySun, UInt8 IsFirstPerson, UInt8 WireFrame);
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
void (__thiscall RenderHook::* Render)(BSRenderedTexture*);
void (__thiscall RenderHook::* TrackRender)(BSRenderedTexture*);
void RenderHook::TrackRender(BSRenderedTexture* RenderedTexture) {
	
	TheRenderManager->SetSceneGraph();
	TheShaderManager->UpdateConstants();
	if (TheRenderManager->BackBuffer) TheRenderManager->defaultRTGroup->RenderTargets[0]->data->Surface = TheRenderManager->defaultRTGroup->RenderTargets[1]->data->Surface;
	if (TheSettingManager->SettingsMain.DevelopTraceShaders) _MESSAGE("RENDERING...");
	(this->*Render)(RenderedTexture);

}

bool (__thiscall RenderHook::* EndTargetGroup)(NiCamera*, NiRenderTargetGroup*);
bool (__thiscall RenderHook::* TrackEndTargetGroup)(NiCamera*, NiRenderTargetGroup*);
bool RenderHook::TrackEndTargetGroup(NiCamera* Camera, NiRenderTargetGroup* TargetGroup) {

	if (TheRenderManager->BackBuffer) TargetGroup = TheRenderManager->defaultRTGroup;
	return (this->*EndTargetGroup)(Camera, TargetGroup);

}

void (__thiscall RenderHook::* HDRRender)(NiScreenElements*, BSRenderedTexture**, BSRenderedTexture**, UInt8);
void (__thiscall RenderHook::* TrackHDRRender)(NiScreenElements*, BSRenderedTexture**, BSRenderedTexture**, UInt8);
void RenderHook::TrackHDRRender(NiScreenElements* ScreenElements, BSRenderedTexture** RenderedTexture1, BSRenderedTexture** RenderedTexture2, UInt8 Arg4) {
	
	TheRenderManager->clearColor = D3DCOLOR_RGBA(0, 0, 0, 0);
	(this->*HDRRender)(ScreenElements, RenderedTexture1, RenderedTexture2, Arg4);

}

void (__thiscall RenderHook::* Culling)(NiCullingProcess*);
void (__thiscall RenderHook::* TrackCulling)(NiCullingProcess*);
void RenderHook::TrackCulling(NiCullingProcess* Process) {
	
	NiAVObject* Object = (NiAVObject*)this;
	
	if (Object && Object->m_pcName) {
		if (TheShaderManager->ShaderConst.currentWorldSpace == NULL && (!memcmp(Object->m_pcName, "Current Precip Root", 19) || !memcmp(Object->m_pcName, "Last Precip Root", 16))) return;
	}
	(this->*Culling)(Process);

}

void (__thiscall RenderHook::* CullingBSFadeNode)(NiCullingProcess*);
void (__thiscall RenderHook::* TrackCullingBSFadeNode)(NiCullingProcess*);
void RenderHook::TrackCullingBSFadeNode(NiCullingProcess* Process) {

	BSFadeNode* Object = (BSFadeNode*)this;

	if (TheSettingManager->SettingsMain.FrameRate && TheShaderManager->ShaderConst.currentWorldSpace)
		if (Object->cMultType == 6 && !strstr(Object->m_pcName, "ImperialCity") && TheFrameRateManager->IsOutGrid(Object)) return;
	(this->*CullingBSFadeNode)(Process);

}

float (__thiscall RenderHook::* FarPlane)();
float (__thiscall RenderHook::* TrackFarPlane)();
float RenderHook::TrackFarPlane() {
	
	float r = (this->*FarPlane)();

	if (TheSettingManager->SettingsMain.FarPlaneDistance && r == 283840.0f) r = TheSettingManager->SettingsMain.FarPlaneDistance;
	return r;

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
	
	NiPixelData* r = NULL;
	
	TheRenderManager->IsSaveGameScreenShot = 1;
	r = SaveGameScreenshot(pWidth, pHeight);
	TheRenderManager->IsSaveGameScreenShot = 0;
	return r;

}

static UInt32* ShaderPackageSelector = (UInt32*)0x00B42F48;
static UInt32* ShaderPackageSelectorMax = (UInt32*)0x00B42D74;
void (__cdecl * SetShaderPackageSelector)(int, int, UInt8, int, char*, int) = (void (__cdecl *)(int, int, UInt8, int, char*, int))0x007B45F0;
void __cdecl TrackSetShaderPackageSelector(int Arg1, int Arg2, UInt8 Force1XShaders, int Arg4, char* GraphicsDLL, int Arg6) {
	
	if (TheSettingManager->SettingsMain.NVIDIAPatch && strstr(GraphicsDLL, "nvd3dum.dll")) strcpy(GraphicsDLL, "nvd3dum.dll");
	SetShaderPackageSelector(Arg1, Arg2, Force1XShaders, Arg4, GraphicsDLL, Arg6);
	if (TheSettingManager->SettingsMain.ShaderModel3) {
		if (*ShaderPackageSelector == 5 && *ShaderPackageSelectorMax == 5)
			*ShaderPackageSelector = *ShaderPackageSelectorMax = 7;
		else if (*ShaderPackageSelector == 6 && *ShaderPackageSelectorMax == 6)
			*ShaderPackageSelector = *ShaderPackageSelectorMax = 7;
	}
	
}

void (__cdecl * RenderObject)(NiCamera*, NiAVObject*, NiCullingProcess*, NiVisibleArray*) = (void (__cdecl *)(NiCamera*, NiAVObject*, NiCullingProcess*, NiVisibleArray*))0x0070C0B0;
void __cdecl TrackRenderObject(NiCamera* Camera, NiAVObject* Object, NiCullingProcess* CullingProcess, NiVisibleArray* VisibleArray) {

	SceneGraph* WorldSceneGraph = *(SceneGraph**)kWorldSceneGraph;

	RenderObject(Camera, Object, CullingProcess, VisibleArray);
	if (Object == WorldSceneGraph && TheRenderManager->FirstPersonDiscarded) {
		TheRenderManager->ResolveDepthBuffer();
	}
	else if (Object == (*g_thePlayer)->firstPersonNiNode) {
		TheRenderManager->ResolveDepthBuffer();
		TheRenderManager->Clear(NULL, NiRenderer::kClear_ZBUFFER);
		RenderObject(Camera, Object, CullingProcess, VisibleArray);
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

void (__thiscall RenderHook::* RenderWorldSceneGraph)(Sun*, UInt8, UInt8);
void (__thiscall RenderHook::* TrackRenderWorldSceneGraph)(Sun*, UInt8, UInt8);
void RenderHook::TrackRenderWorldSceneGraph(Sun* SkySun, UInt8 IsFirstPerson, UInt8 WireFrame) {

	(this->*RenderWorldSceneGraph)(SkySun, IsFirstPerson, WireFrame);
	if (!IsFirstPerson) TheRenderManager->ResolveDepthBuffer();

}

static const UInt32 NiDX9RendererClear = 0x00CD5D00;
static __declspec(naked) void ClearDepth()
{
	__asm
	{
		push	ClearMode
		push	0
		call	NiDX9RendererClear
		jmp		kClearDepthReturn
	}
}

static const UInt32 RenderFirstPersonShadow = 0x00695740;
static __declspec(naked) void RenderFirstPersonNode()
{
	__asm
	{
		cmp		ClearMode, 0
		jnz		loc_jumpout
		mov		ClearMode, 4
		pushad
		mov		ecx, TheRenderManager
		call	RenderManager::ResolveDepthBuffer
		popad
		jmp		kRenderFirstPersonNodeRepeat

	loc_jumpout:
		call	RenderFirstPersonShadow
		mov		ClearMode, 0
		jmp		kRenderFirstPersonNodeReturn
	}
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

static __declspec(naked) void FixSunFlags()
{
	static int max = 0;

	__asm
	{
		or		eax, 0x4002
		add		eax, 0x2B
		push	eax
		mov		eax, [esp + 0x90 - 0x74]
		mov		max, eax
		mov		eax, 0x1BA7680
		mov		eax, [eax]
		mov		eax, [eax + 0x138]
		cmp		max, eax
		je		loc_fix
		jmp		kFixSunFlagsReturn

	loc_fix:
		pop		eax
		mov		eax, 0x0040402D
		push	eax
		jmp		kFixSunFlagsReturn
	}
}

#endif

void (__cdecl * ProcessImageSpaceShaders)(NiDX9Renderer*, BSRenderedTexture*, BSRenderedTexture*) = (void (__cdecl *)(NiDX9Renderer*, BSRenderedTexture*, BSRenderedTexture*))kProcessImageSpaceShaders;
void __cdecl TrackProcessImageSpaceShaders(NiDX9Renderer* Renderer, BSRenderedTexture* RenderedTexture1, BSRenderedTexture* RenderedTexture2) {
	
	BSRenderedTexture* MenuRenderedTexture = *(BSRenderedTexture**)kMenuRenderedTexture;
	
	if (TheRenderManager->BackBuffer) TheRenderManager->defaultRTGroup->RenderTargets[0]->data->Surface = TheRenderManager->BackBuffer;
	ProcessImageSpaceShaders(Renderer, RenderedTexture1, RenderedTexture2);
	if (!RenderedTexture2 || MenuRenderedTexture) {
		if (TheRenderManager->currentRTGroup) {
			TheShaderManager->RenderEffects(TheRenderManager->currentRTGroup->RenderTargets[0]->data->Surface);
			TheGameMenuManager->Render();
		}
	}
	if (TheRenderManager->IsSaveGameScreenShot) {
		if (MenuRenderedTexture)
			TheRenderManager->device->StretchRect(MenuRenderedTexture->RenderTargetGroup->RenderTargets[0]->data->Surface, NULL, TheRenderManager->currentRTGroup->RenderTargets[0]->data->Surface, &TheRenderManager->SaveGameScreenShotRECT, D3DTEXF_NONE);
		else
			TheRenderManager->device->StretchRect(TheRenderManager->defaultRTGroup->RenderTargets[0]->data->Surface, NULL, TheRenderManager->currentRTGroup->RenderTargets[0]->data->Surface, &TheRenderManager->SaveGameScreenShotRECT, D3DTEXF_NONE);
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
	*((int *)&EndTargetGroup)				= 0x007AAA30;
	TrackEndTargetGroup						= &RenderHook::TrackEndTargetGroup;
	*((int *)&HDRRender)					= 0x007BDFC0;
	TrackHDRRender							= &RenderHook::TrackHDRRender;
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
#elif defined(SKYRIM)
	*((int *)&RenderWorldSceneGraph)		= 0x00692290;
	TrackRenderWorldSceneGraph				= &RenderHook::TrackRenderWorldSceneGraph;
#endif

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)InitializeRenderer,			*((PVOID *)&TrackInitializeRenderer));
	DetourAttach(&(PVOID&)Render,						*((PVOID *)&TrackRender));
	DetourAttach(&(PVOID&)SetupShaderPrograms,			*((PVOID *)&TrackSetupShaderPrograms));
	DetourAttach(&(PVOID&)BeginScene,					*((PVOID *)&TrackBeginScene));
	DetourAttach(&(PVOID&)ProcessImageSpaceShaders,				   &TrackProcessImageSpaceShaders);
#if defined(OBLIVION)
	DetourAttach(&(PVOID&)EndTargetGroup,				*((PVOID *)&TrackEndTargetGroup));
	DetourAttach(&(PVOID&)HDRRender,					*((PVOID *)&TrackHDRRender));
	DetourAttach(&(PVOID&)Culling,						*((PVOID *)&TrackCulling));
	DetourAttach(&(PVOID&)CullingBSFadeNode,			*((PVOID *)&TrackCullingBSFadeNode));
	DetourAttach(&(PVOID&)FarPlane,						*((PVOID *)&TrackFarPlane));
	DetourAttach(&(PVOID&)PrepareTextureForRendering,	*((PVOID *)&TrackPrepareTextureForRendering));
	DetourAttach(&(PVOID&)SetSamplerState,				*((PVOID *)&TrackSetSamplerState));
	DetourAttach(&(PVOID&)SaveGameScreenshot,					   &TrackSaveGameScreenshot);
	DetourAttach(&(PVOID&)SetShaderPackageSelector,				   &TrackSetShaderPackageSelector);
	DetourAttach(&(PVOID&)RenderObject,							   &TrackRenderObject);
#elif defined(SKYRIM)
	DetourAttach(&(PVOID&)RenderWorldSceneGraph,		*((PVOID *)&TrackRenderWorldSceneGraph));
#endif
    DetourTransactionCommit();

#if defined(OBLIVION)
	if (TheSettingManager->SettingsMain.RendererBoost) {
		WriteRelJump(0x007D6AC5, 0x007D6ACA);
		WriteRelJump(0x007D6AF1, 0x007D6AF6);
	}
	SafeWrite32(0x0076BD75, 0xB44); // Extends the NiDX9Renderer allocation from size 0xB00 to 0xB44 to store additional data
	SafeWrite32(0x0049BFAF, TheSettingManager->SettingsMain.WaterReflectionMapSize); // Constructor
	SafeWrite32(0x007C1045, TheSettingManager->SettingsMain.WaterReflectionMapSize); // RenderedSurface
	SafeWrite32(0x007C104F, TheSettingManager->SettingsMain.WaterReflectionMapSize); // RenderedSurface
	SafeWrite32(0x007C10F9, TheSettingManager->SettingsMain.WaterReflectionMapSize); // RenderedSurface
	SafeWrite32(0x007C1103, TheSettingManager->SettingsMain.WaterReflectionMapSize); // RenderedSurface
	SafeWrite8(0x00A38280, 0x5A); // Fixes the "purple water bug"
	SafeWrite8(0x0040CE11, 0); // Stops to clear the depth buffer when rendering the 1st person node
	if (TheSettingManager->SettingsMain.AnisotropicFilter >= 2) {
		SafeWrite8(0x007BE1D3, TheSettingManager->SettingsMain.AnisotropicFilter);
		SafeWrite8(0x007BE32B, TheSettingManager->SettingsMain.AnisotropicFilter);
	}
	*LocalWaterHiRes = 1; //Fixes a bug on the WaterHeightMapConstructor (the constructor is called before initializing the value with the bUseWaterHiRes ini value)
#elif defined(SKYRIM)
	SafeWrite32(0x00CDB659, 0x8C4); // Extends the NiDX9Renderer allocation from size 0x880 to 0x8C4 to store additional data
	WriteRelJump(kClearDepth, (UInt32)ClearDepth);
	WriteRelJump(kRenderFirstPersonNode, (UInt32)RenderFirstPersonNode);
	if (TheSettingManager->SettingsMain.ShadowMode) {
		WriteRelJump(kSetShadowDistance,		(UInt32)SetShadowDistance);
		WriteRelJump(kSetShadowDistanceShader,	(UInt32)SetShadowDistanceShader);
		if (TheSettingManager->SettingsMain.ShadowModeMultiPointsLighting) {
			WriteRelJump(0x0069A7FF, 0x0069A8A7);
			WriteRelJump(kFixSunFlags, (UInt32)FixSunFlags);
		}
	}
#endif

}
