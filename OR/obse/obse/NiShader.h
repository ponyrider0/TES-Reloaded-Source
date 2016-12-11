#include "obse/NiRenderer.h"
#include "obse/NiTypes.h"

class NiD3DRenderStateGroup;
class NiD3DShaderConstantMap;
class NiD3DShaderDeclaration;
class NiD3DTextureStage;

class NiD3DShaderProgram : public NiRefObject
{
public:
	NiD3DShaderProgram(NiDX9Renderer* Renderer, UInt32 Arg2);
	virtual ~NiD3DShaderProgram();

	enum ProgramType
	{
		PROGRAM_VERTEX = 0,
		PROGRAM_PIXEL,
		PROGRAM_GEOMETRY,
		PROGRAM_MAX
	};

	ProgramType					eProgramType;		// 08
	char*						Name;				// 0C
	char*						ShaderProgramName;	// 10
	UInt32						CodeSize;			// 14
	void*						Unk018;				// 18
	IDirect3DDevice9*			Device;				// 1C
	NiDX9Renderer*				Renderer;			// 20
	NiDX9RenderState*			RenderState;		// 24

};

class NiD3DVertexShader : public NiD3DShaderProgram
{
public:
	NiD3DVertexShader(NiDX9Renderer* Renderer, UInt32 Arg2);
	virtual ~NiD3DVertexShader();

	UInt8							SoftwareVP;		// 28
	UInt8							pad[3];			// 29
	UInt32							Usage;			// 2C
	IDirect3DVertexShader9*			ShaderHandle;	// 30
	IDirect3DVertexDeclaration9*	Declaration;	// 34

};

class NiD3DPixelShader : public NiD3DShaderProgram
{
public:
	NiD3DPixelShader(NiDX9Renderer* Renderer, UInt32 Arg2);
	virtual ~NiD3DPixelShader();

	IDirect3DPixelShader9*			ShaderHandle;	// 28

};

class NiD3DPass
{
public:
	NiD3DPass();
	virtual ~NiD3DPass();

	char							Name[16];					// 04
	UInt32							CurrentStage;				// 14
	UInt32							StageCount;					// 18
	UInt32							TexturesPerPass;			// 1C
	NiTArray<NiD3DTextureStage*>	Stages;						// 20
	NiD3DRenderStateGroup*			RenderStateGroup;			// 30
	NiD3DShaderConstantMap*			PixelConstantMap;			// 34
	char*							PixelShaderProgramFile;		// 38
	char*							PixelShaderEntryPoint;		// 3C
	char*							PixelShaderTarget;			// 40
	NiD3DPixelShader*				PixelShader;				// 44
	NiD3DShaderConstantMap*			VertexConstantMap;			// 48
	char*							VertexShaderProgramFile;	// 4C
	char*							VertexShaderEntryPoint;		// 50
	char*							VertexShaderTarget;			// 54
	NiD3DVertexShader*				VertexShader;				// 58
	UInt8							SoftwareVP;					// 5C
	UInt8							RendererOwned;				// 5D
	UInt8							pad[2];
	UInt32							RefCount;					// 60

};

class NiShader : public NiRefObject
{
public:
	NiShader();
	virtual ~NiShader();

	char*		Name;					// 008
	UInt32		Unk00C;					// 00C

};

class NiD3DShaderInterface : public NiShader
{
public:
	NiD3DShaderInterface();
	virtual ~NiD3DShaderInterface();

	IDirect3DDevice9*	D3DDevice;		// 010
	NiDX9Renderer*		D3DRenderer;	// 014
	NiDX9RenderState*	D3DRenderState;	// 018
	UInt8				Unk01C;			// 01C
	UInt8				Unk01D;			// 01D
	UInt8				pad[2];

};

class NiD3DShader : public NiD3DShaderInterface
{
public:
	NiD3DShader();
	virtual ~NiD3DShader();

	UInt8					IsInitialized;		// 020
	UInt8					Unk021;				// 021
	UInt8					pad[2];
	NiD3DShaderDeclaration*	ShaderDeclaration;	// 024
	NiD3DRenderStateGroup*	RenderStateGroup;	// 028
	NiD3DShaderConstantMap* PixelConstantMap;	// 02C
	NiD3DShaderConstantMap* VertexConstantMap;	// 030
	UInt32					CurrentPassIndex;	// 034
	UInt32					PassCount;			// 038
	NiD3DPass*				CurrentPass;		// 03C
	NiTArray<NiD3DPass*>	Passes;				// 040
	UInt32					Unk050[8];			// 050
	
};
STATIC_ASSERT(sizeof(NiD3DShader) == 0x70);