#include "obse/NiRenderer.h"
#include "obse/NiTypes.h"

class NiD3DRenderStateGroup;
class NiD3DShaderConstantMap;
class NiD3DTextureStage;

class NiD3DRenderState : public NiRefObject
{
public:
	NiD3DRenderState();
	virtual ~NiD3DRenderState();

	virtual void					SetAlpha(void *AlphaProperty);					// 02
	virtual void					SetDither(void *DitherProperty);				// 03
	virtual void					SetFog(NiFogProperty *FogProperty);				// 04
	virtual void					SetMaterial(void *u1);							// 05
	virtual void					SetShadeMode(void *ShadeProperty);				// 06
	virtual void					SetSpecular(void *SpecularProperty);			// 07
	virtual void					SetStencil(void *StencilProperty);				// 08
	virtual void					SetWireframe(void *WireframeProperty);			// 09
	virtual void 					SetZBuffer(void *ZBufferProperty);				// 0A
	virtual void					RestoreAlpha(void);								// 0B
	virtual void					SetVertexBlending(UInt16 u1);					// 0C
	virtual void					SetNormalization(void *u1);						// 0D
	virtual void					func_0E(UInt32 u1);								// 0E - doesn't do anything
	virtual float					GetVar088(void);								// 0F
	virtual void					func_10(float u1);								// 10
	virtual void					func_11(float u1, float u2);					// 11
	virtual BOOL					func_12(void);									// 12
	virtual void					func_13(BOOL u1);								// 13
	virtual void					func_14(UInt32 u1);								// 14
	virtual void					InitializeRenderStates(void);					// 15
	virtual void					BackUpAllStates(void);							// 16
	virtual void 					RestoreRenderState(D3DRENDERSTATETYPE state);	// 17
	virtual void					RestoreAllRenderStates(void);					// 18
	virtual void 					SetRenderState(D3DRENDERSTATETYPE state, UInt32 value, UInt8 BackUp);	// 19
	virtual UInt32					GetRenderState(D3DRENDERSTATETYPE state);		// 1A
	virtual void					ClearPixelShaders(void);						// 1B
	virtual void					BackUpPixelShader(void);						// 1C
	virtual void					func_1D(void);									// 1D
	virtual void					SetPixelShader(IDirect3DPixelShader9* PixelShader, UInt8 BackUp);		// 1E
	virtual IDirect3DPixelShader9*	GetPixelShader(void);							// 1F
	virtual	void					RestorePixelShader(void);						// 20
	virtual void					RemovePixelShader(IDirect3DPixelShader9* PixelShader);					// 21
	virtual void					SetVertexShader(IDirect3DVertexShader9* pShader, UInt8 BackUp);			// 22 = 0
	virtual IDirect3DVertexShader9*	GetVertexShader(void);							// 23 = 0
	virtual void					RestoreVertexShader(void);						// 24 = 0
	virtual void					RemoveVertexShader(IDirect3DVertexShader9* pShader);						// 25 = 0
	virtual void					SetFVF(UInt32 FVF, UInt8 BackUp);					// 26 = 0
	virtual UInt32					GetFVF(void);									// 27 = 0
	virtual void					RestoreFVF(void);								// 28 = 0
	virtual void					RemoveFVF(UInt32 FVF);							// 29 = 0
	virtual void					SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl, UInt8 BackUp);	// 2A = 0
	virtual IDirect3DVertexDeclaration9*	GetVertexDeclaration(void);				// 2B = 0
	virtual void					RestoreVertexDeclaration(void);					// 2C = 0
	virtual void					RemoveVertexDeclatation(IDirect3DVertexDeclaration9* pDecl);			// 2D = 0
	virtual void					InitTextureStageState();						// 2E
	virtual void					func_2F(void);									// 2F
	virtual void					func_30(UInt32 u1, UInt32 u2);					// 30
	virtual void					func_31(void);									// 31
	virtual void 					SetTextureStageState(UInt32 Stage, D3DTEXTURESTAGESTATETYPE Type, UInt32 Value, UInt8 BackUp);	// 32
	virtual UInt32					func_33(UInt32 u1, UInt32 u2);					// 33
	virtual void					SetSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type, UInt32 Value, UInt8 BackUp);				// 34 = 0
	virtual UInt32 					GetSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type);				// 35 = 0
	virtual void					RestoreSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type);			// 36 = 0
	virtual void					ClearTextureList(void);							// 37
	virtual void					SetTexture(UInt32 Sampler, IDirect3DBaseTexture9* pTexture);	// 38
	virtual IDirect3DBaseTexture9*	GetTexture(UInt32 Sampler);						// 39
	virtual void					RemoveTexture(IDirect3DBaseTexture9* pTexture);	// 3A
	virtual void					SetSoftwareVertexProcessing(UInt8 bSoftware);	// 3B
	virtual UInt8					GetSoftwareVertexProcessing(void);				// 3C
	virtual void					SetVar_0FF4(UInt8 u1);							// 3D
	virtual UInt8					GetVar_0FF4(void);								// 3E
	virtual void					SetVar_0FF5(UInt8 u1);							// 3F
	virtual UInt8					GetVar_0FF5(void);								// 40
	virtual void					Reset(void);									// 41
	virtual void					func_042(void);									// 42 = 0

	UInt32				Flags;
	D3DVERTEXBLENDFLAGS VertexBlendMappings[5];
	UInt32				AlphaMapping[11];
	UInt32				AlphaTestMapping[8];
	UInt32				SrcAlphaCaps;
	UInt32				DestAlphaCaps;
	NiAlphaProperty*	DisabledAlphaProperty;
	float				CameraNear;
	float				CameraFar;
	float				CameraDepthRange;
	float				MaxFogFactor;
	float				MaxFogValue;
	NiColor				CurrentFogColor;
	UInt32				pCurrentFogColor;

};

class NiDX9RenderState : public NiD3DRenderState
{
public:
	NiDX9RenderState();
	virtual ~NiDX9RenderState();

};

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
	NiD3DRenderState*			RenderState;		// 24

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