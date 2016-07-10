#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#define kInitializeRenderer 0x004052F0
#define kGetShaderFunction 0x007DAC70
#define kNewVertexShader 0x00780D80
#define kNewPixelShader 0x00780B90
#elif defined(SKYRIM)
#define DumpShaders 0
#if DumpShaders
#include <fstream>
#endif
#include <string>
#include "skse\SafeWrite.h"
#define kInitializeRenderer 0x00691030
static const UInt32 kVertexShaderFunctionHook = 0x00CCBBB1;
static const UInt32 kVertexShaderFunctionReturn = 0x00CCBBC3;
static const UInt32 kPixelShaderFunctionHook = 0x00CCC4BC;
static const UInt32 kPixelShaderFunctionReturn = 0x00CCC4C5;
#define kNewVertexShader 0x00CCBB00
#define kNewPixelShader 0x00CCC420
static char ShaderPrefix[8] = "SHADER";
static int VertexShaderCounter = 0;
static int PixelShaderCounter = 0;
class BSIStream;
#endif
#include "ShaderIOHook.h"
#include "Hooking\detours\detours.h"

static RuntimeShaderRecord* ShaderRecord = NULL;
static void* ShaderFunction = NULL;
static char ShaderName[24] = "SHADER";

class ShaderIOHook {

public:
	NiDX9Renderer* TrackInitializeRenderer();
#if defined(OBLIVION)
	void* TrackGetShaderFunction(char* ShaderName);
	NiD3DVertexShader* TrackNewVertexShader(NiDX9Renderer* Renderer);
	NiD3DPixelShader* TrackNewPixelShader(NiDX9Renderer* Renderer);
#elif defined(SKYRIM)
	bool TrackLoadBloodSplatterShader(BSIStream* ShaderPackage);
	bool TrackLoadDistantTreeShader(BSIStream* ShaderPackage);
	bool TrackLoadGrassShader(BSIStream* ShaderPackage);
	bool TrackLoadParticleShader(BSIStream* ShaderPackage);
	bool TrackLoadSkyShader(BSIStream* ShaderPackage);
	bool TrackLoadEffectShader(BSIStream* ShaderPackage);
	bool TrackLoadLightingShader(BSIStream* ShaderPackage);
	bool TrackLoadUtilityShader(BSIStream* ShaderPackage);
	bool TrackLoadWaterShader(BSIStream* ShaderPackage);
	int TrackNewVertexShader(BSIStream* ShaderPackage, int Arg2);
	int TrackNewPixelShader(BSIStream* ShaderPackage, int Arg2);
#endif

};

NiDX9Renderer* (__thiscall ShaderIOHook::* InitializeRenderer)();
NiDX9Renderer* (__thiscall ShaderIOHook::* TrackInitializeRenderer)();
NiDX9Renderer* ShaderIOHook::TrackInitializeRenderer() {

	NiDX9Renderer* Renderer = (this->*InitializeRenderer)();

	TheRenderManager = (RenderManager*)Renderer;
	TheRenderManager->Initialize();
	TheShaderManager = new ShaderManager();
	TheEffectManager = new EffectManager();
	TheTextureManager = new TextureManager();
	TheEquipmentManager = new EquipmentManager();
	TheFrameRateManager = new FrameRateManager();
	return Renderer;

}
#if defined(OBLIVION)
void* (__thiscall ShaderIOHook::* GetShaderFunction)(char*);
void* (__thiscall ShaderIOHook::* TrackGetShaderFunction)(char*);
void* ShaderIOHook::TrackGetShaderFunction(char* Name) {

	ShaderFunction = (this->*GetShaderFunction)(Name);

	if (ShaderRecord = TheShaderManager->LoadShader(Name)) ShaderFunction = (void*)((int)ShaderRecord->pFunction - 0x104);
	strcpy(ShaderName, Name);
	return ShaderFunction;

}

NiD3DVertexShader* (__thiscall ShaderIOHook::* NewVertexShader)(NiDX9Renderer*);
NiD3DVertexShader* (__thiscall ShaderIOHook::* TrackNewVertexShader)(NiDX9Renderer*);
NiD3DVertexShader* ShaderIOHook::TrackNewVertexShader(NiDX9Renderer* Renderer) {

	NiD3DVertexShaderEx* VertexShader = (NiD3DVertexShaderEx*)(this->*NewVertexShader)(Renderer);

	VertexShader->ShaderRecord = ShaderRecord;
	VertexShader->ShaderName = NULL;
	if (TheSettingManager->SettingsMain.DevelopTraceVanillaShaders) {
		VertexShader->ShaderName = new char(strlen(ShaderName) + 1);
		strcpy(VertexShader->ShaderName, ShaderName);
	}
	return (NiD3DVertexShader*)VertexShader;

}

NiD3DPixelShader* (__thiscall ShaderIOHook::* NewPixelShader)(NiDX9Renderer*);
NiD3DPixelShader* (__thiscall ShaderIOHook::* TrackNewPixelShader)(NiDX9Renderer*);
NiD3DPixelShader* ShaderIOHook::TrackNewPixelShader(NiDX9Renderer* Renderer) {

	NiD3DPixelShaderEx* PixelShader = (NiD3DPixelShaderEx*)(this->*NewPixelShader)(Renderer);
	
	PixelShader->ShaderRecord = ShaderRecord;
	PixelShader->ShaderName = NULL;
	if (TheSettingManager->SettingsMain.DevelopTraceVanillaShaders) {
		PixelShader->ShaderName = new char(strlen(ShaderName) + 1);
		strcpy(PixelShader->ShaderName, ShaderName);
	}
	return (NiD3DPixelShader*)PixelShader;

}
#elif defined(SKYRIM)
#if DumpShaders
void DumpShader(void* Function, int Len) {
	
	if (!strstr(ShaderName,"SHADER")) {
#if (DumpShaders == 1)
		std::string FileNameDump = "C:\\Archivio\\Downloads\\SkyrimShaders\\shaders001_dump\\";
		FileNameDump += ShaderName;
		std::ofstream FileBinary(FileNameDump, std::ios::out|std::ios::binary);
		FileBinary.write((char*)Function, Len);
		FileBinary.flush();
		FileBinary.close();
#elif (DumpShaders == 2)
		ID3DXBuffer* pDisasm = NULL;
		D3DXDisassembleShader((const DWORD*)Function, FALSE, NULL, &pDisasm);
		std::string FileNameDisasm = "C:\\Archivio\\Downloads\\SkyrimShaders\\shaders001_disasm\\";
		FileNameDisasm += ShaderName;
		std::ofstream FileDisasm(FileNameDisasm, std::ios::out|std::ios::binary);
		FileDisasm.write((char*)pDisasm->GetBufferPointer(), pDisasm->GetBufferSize());
		FileDisasm.flush();
		FileDisasm.close();
#endif
	}

}
#endif
static __declspec(naked) void VertexShaderFunctionHook()
{

	__asm
	{
#if DumpShaders
		pushad
		push    eax
		push	ebx
		call	DumpShader
		pop		ebx
		pop		eax
		popad
#endif
		mov		eax, [esp + 0x20]
		mov		edx, [eax]
		mov		edx, [edx + 0x16C]
		lea		ecx, [edi + 0x20]
		push	ecx
		cmp		ShaderFunction, 0
		jnz		loc_shaderfunction
		push	ebx
		jmp		loc_continue

	loc_shaderfunction:
		push	ShaderFunction

	loc_continue:
		push	eax
		jmp		kVertexShaderFunctionReturn
	}

}

static __declspec(naked) void PixelShaderFunctionHook()
{

	__asm
	{
#if DumpShaders
		pushad
		push    eax
		mov		ebx, [esi + 0x5C]
		push	ebx
		call	DumpShader
		pop		ebx
		pop		eax
		popad
#endif
		cmp		ShaderFunction, 0
		jnz		loc_shaderfunction
		jmp		kPixelShaderFunctionReturn

	loc_shaderfunction:
		mov		eax, ShaderFunction
		mov		[esi + 0x5C], eax
		jmp		kPixelShaderFunctionReturn
	}

}

bool (__thiscall ShaderIOHook::* LoadBloodSplatterShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadBloodSplatterShader)(BSIStream*);
bool ShaderIOHook::TrackLoadBloodSplatterShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "BLOOD");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadBloodSplatterShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadDistantTreeShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadDistantTreeShader)(BSIStream*);
bool ShaderIOHook::TrackLoadDistantTreeShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "DTREE");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadDistantTreeShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadGrassShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadGrassShader)(BSIStream*);
bool ShaderIOHook::TrackLoadGrassShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "GRASS");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadGrassShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadParticleShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadParticleShader)(BSIStream*);
bool ShaderIOHook::TrackLoadParticleShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "PARTIC");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadParticleShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadSkyShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadSkyShader)(BSIStream*);
bool ShaderIOHook::TrackLoadSkyShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "SKY");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadSkyShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadEffectShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadEffectShader)(BSIStream*);
bool ShaderIOHook::TrackLoadEffectShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "EFFECT");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadEffectShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadLightingShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadLightingShader)(BSIStream*);
bool ShaderIOHook::TrackLoadLightingShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "LIGHT");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadLightingShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadUtilityShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadUtilityShader)(BSIStream*);
bool ShaderIOHook::TrackLoadUtilityShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "UTIL");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadUtilityShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadWaterShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadWaterShader)(BSIStream*);
bool ShaderIOHook::TrackLoadWaterShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "WATER");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	bool r = (this->*LoadWaterShader)(ShaderPackage);
	strcpy(ShaderPrefix, "SHADER"); // We are not interested to the ImageSpace shaders by now (loaded after the water), so we reset the name to recognize and avoid them
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return r;

}

int (__thiscall ShaderIOHook::* NewVertexShader)(BSIStream*, int);
int (__thiscall ShaderIOHook::* TrackNewVertexShader)(BSIStream*, int);
int ShaderIOHook::TrackNewVertexShader(BSIStream* ShaderPackage, int Arg2) {
	
	ShaderFunction = NULL;
	std::string Name = ShaderPrefix;
	
	VertexShaderCounter += 1;
	Name += std::to_string(VertexShaderCounter);
	Name += ".vso";
	strcpy(ShaderName, Name.c_str());
	if (ShaderRecord = TheShaderManager->LoadShader(ShaderName)) ShaderFunction = ShaderRecord->pFunction;

	int r = (this->*NewVertexShader)(ShaderPackage, Arg2);
	NiD3DVertexShaderEx* VertexShader = (NiD3DVertexShaderEx*)this;
	
	VertexShader->ShaderRecord = ShaderRecord;
	VertexShader->ShaderName = NULL;
	if (TheSettingManager->SettingsMain.DevelopTraceVanillaShaders) {
		VertexShader->ShaderName = new char(strlen(ShaderName) + 1);
		strcpy(VertexShader->ShaderName, ShaderName);
	}
	return r;

}

int (__thiscall ShaderIOHook::* NewPixelShader)(BSIStream*, int);
int (__thiscall ShaderIOHook::* TrackNewPixelShader)(BSIStream*, int);
int ShaderIOHook::TrackNewPixelShader(BSIStream* ShaderPackage, int Arg2) {
	
	ShaderFunction = NULL;
	std::string Name = ShaderPrefix;

	PixelShaderCounter += 1;
	Name += std::to_string(PixelShaderCounter);
	Name += ".pso";
	strcpy(ShaderName, Name.c_str());
	if (ShaderRecord = TheShaderManager->LoadShader(ShaderName)) ShaderFunction = ShaderRecord->pFunction;

	int r = (this->*NewPixelShader)(ShaderPackage, Arg2);
	NiD3DPixelShaderEx* PixelShader = (NiD3DPixelShaderEx*)this;
	
	PixelShader->ShaderRecord = ShaderRecord;
	PixelShader->ShaderName = NULL;
	if (TheSettingManager->SettingsMain.DevelopTraceVanillaShaders) {
		PixelShader->ShaderName = new char(strlen(ShaderName) + 1);
		strcpy(PixelShader->ShaderName, ShaderName);
	}
	if (ShaderFunction) TheRenderManager->device->CreatePixelShader((const DWORD*)ShaderFunction, &PixelShader->ShaderHandle);
	return r;

}
#endif

void CreateShaderIOHook()
{

	*((int *)&InitializeRenderer)		= kInitializeRenderer;
	TrackInitializeRenderer				= &ShaderIOHook::TrackInitializeRenderer;
#if defined(OBLIVION)
	*((int *)&GetShaderFunction)		= kGetShaderFunction;
	TrackGetShaderFunction				= &ShaderIOHook::TrackGetShaderFunction;
#elif defined(SKYRIM)
	*((int *)&LoadBloodSplatterShader)	= 0x00C9D0D0;
	TrackLoadBloodSplatterShader		= &ShaderIOHook::TrackLoadBloodSplatterShader;
	*((int *)&LoadDistantTreeShader)	= 0x00CAE6C0;
	TrackLoadDistantTreeShader			= &ShaderIOHook::TrackLoadDistantTreeShader;
	*((int *)&LoadGrassShader)			= 0x00C95FC0;
	TrackLoadGrassShader				= &ShaderIOHook::TrackLoadGrassShader;
	*((int *)&LoadParticleShader)		= 0x00CC20C0;
	TrackLoadParticleShader				= &ShaderIOHook::TrackLoadParticleShader;
	*((int *)&LoadSkyShader)			= 0x00CA9AE0;
	TrackLoadSkyShader					= &ShaderIOHook::TrackLoadSkyShader;
	*((int *)&LoadEffectShader)			= 0x00CAD980;
	TrackLoadEffectShader				= &ShaderIOHook::TrackLoadEffectShader;
	*((int *)&LoadLightingShader)		= 0x00CA2090;
	TrackLoadLightingShader				= &ShaderIOHook::TrackLoadLightingShader;
	*((int *)&LoadUtilityShader)		= 0x00CB7E60;
	TrackLoadUtilityShader				= &ShaderIOHook::TrackLoadUtilityShader;
	*((int *)&LoadWaterShader)			= 0x00CB38D0;
	TrackLoadWaterShader				= &ShaderIOHook::TrackLoadWaterShader;
#endif
	*((int *)&NewVertexShader)			= kNewVertexShader;
	TrackNewVertexShader				= &ShaderIOHook::TrackNewVertexShader;
	*((int *)&NewPixelShader)			= kNewPixelShader;
	TrackNewPixelShader					= &ShaderIOHook::TrackNewPixelShader;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)InitializeRenderer,		*((PVOID *)&TrackInitializeRenderer));
#if defined(OBLIVION)
	DetourAttach(&(PVOID&)GetShaderFunction,		*((PVOID *)&TrackGetShaderFunction));
#elif defined (SKYRIM)
	DetourAttach(&(PVOID&)LoadBloodSplatterShader,	*((PVOID *)&TrackLoadBloodSplatterShader));
	DetourAttach(&(PVOID&)LoadDistantTreeShader,	*((PVOID *)&TrackLoadDistantTreeShader));
	DetourAttach(&(PVOID&)LoadGrassShader,			*((PVOID *)&TrackLoadGrassShader));
	DetourAttach(&(PVOID&)LoadParticleShader,		*((PVOID *)&TrackLoadParticleShader));
	DetourAttach(&(PVOID&)LoadSkyShader,			*((PVOID *)&TrackLoadSkyShader));
	DetourAttach(&(PVOID&)LoadEffectShader,			*((PVOID *)&TrackLoadEffectShader));
	DetourAttach(&(PVOID&)LoadLightingShader,		*((PVOID *)&TrackLoadLightingShader));
	DetourAttach(&(PVOID&)LoadUtilityShader,		*((PVOID *)&TrackLoadUtilityShader));
	DetourAttach(&(PVOID&)LoadWaterShader,			*((PVOID *)&TrackLoadWaterShader));
#endif
	DetourAttach(&(PVOID&)NewVertexShader,			*((PVOID *)&TrackNewVertexShader));
	DetourAttach(&(PVOID&)NewPixelShader,			*((PVOID *)&TrackNewPixelShader));
	DetourTransactionCommit();

#if defined(OBLIVION)
	SafeWrite32(0x0076BD75, 0xB44); // Extends the NiDX9Renderer allocation from size 0xB00 to 0xB44 to store additional data
	SafeWrite8(0x00801BCB, 0x40);	// Extends the NiD3DVertexShader allocation from size 0x38 to 0x40 to store additional data
	SafeWrite8(0x008023A1, 0x34);	// Extends the NiD3DPixelShader allocation from size 0x2C to 0x34 to store additional data
#elif defined(SKYRIM)
	SafeWrite32(0x00CDB659, 0x8C4); // Extends the NiDX9Renderer allocation from size 0x880 to 0x8C4 to store additional data
	SafeWrite8(0x00CCBD66, 0x3C);	// Extends the NiD3DVertexShader allocation from size 0x34 to 0x3C to store additional data
	SafeWrite8(0x00CCC676, 0x6C);	// Extends the NiD3DPixelShader allocation from size 0x64 to 0x6C to store additional data
	WriteRelJump(kVertexShaderFunctionHook, (UInt32)VertexShaderFunctionHook);
	WriteRelJump(kPixelShaderFunctionHook, (UInt32)PixelShaderFunctionHook);
#endif

}