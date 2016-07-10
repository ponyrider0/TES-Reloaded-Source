#include "skse/NiRenderer.h"
#include "skse/NiTypes.h"

class BSShader : public NiRefObject
{

public:
	
	enum ShaderType {
		tBSGrassShader = 1,
		tBSSkyShader = 2,
		tBSWaterShader = 3,
		tBSBloodSplatterShader = 4,
		tBSUnknownShader = 5,
		tBSLightingShader = 6,
		tBSEffectShader = 7,
		tBSUtilityShader = 8,
		tBSDistantTreeShader = 9,
		tBSParticleShader = 10,
	};

	UInt32		unk08;		// 08
	ShaderType	shaderType; // 0C
	UInt32		unk10;		// 10
	UInt32		unk14;		// 14
	UInt32		unk18;		// 18
	UInt32		unk1C;		// 1C
	UInt32		unk20;		// 20
	UInt32		unk24;		// 24
	UInt32		unk28;		// 28
	UInt32		unk2C;		// 2C
	UInt32		unk30;		// 30
	UInt32		unk34;		// 34
	UInt32		unk38;		// 38
	UInt32		unk3C;		// 3C
	UInt32		unk40;		// 40
	UInt32		unk44;		// 44
	UInt32		unk48;		// 48
	UInt32		unk4C;		// 4C
	UInt32		unk50;		// 50

};
STATIC_ASSERT(sizeof(BSShader) == 0x54);

class NiD3DVertexShader // Unknown class name; no virtual table
{

public:
	UInt32					unk00;			// 00
	UInt32					unk04;			// 04
	UInt32					unk08;			// 08
	UInt32					unk0C;			// 0C
	UInt32					unk10;			// 10
	UInt32					unk14;			// 14
	UInt32					unk18;			// 18
	UInt32					unk1C;			// 1C
	IDirect3DVertexShader9*	ShaderHandle;	// 20
	UInt32					unk24;			// 24
	UInt32					unk28;			// 28
	UInt32					unk2C;			// 2C
	UInt16					StartRegister;	// 30
	UInt8					Vector4Count;	// 32
	UInt8					unk33;			// 33

};
STATIC_ASSERT(sizeof(NiD3DVertexShader) == 0x34);

class NiD3DPixelShader // Unknown class name; no virtual table
{

public:
	UInt32					unk00;			// 00
	UInt32					unk04;			// 04
	UInt32					unk08;			// 08
	UInt32					unk0C;			// 0C
	UInt32					unk10;			// 10
	UInt32					unk14;			// 14
	UInt32					unk18;			// 18
	UInt32					unk1C;			// 1C
	UInt32					unk20;			// 20
	UInt32					unk24;			// 24
	UInt32					unk28;			// 28
	UInt32					unk2C;			// 2C
	UInt32					unk30;			// 30
	UInt32					unk34;			// 34
	UInt32					unk38;			// 38
	UInt32					unk3C;			// 3C
	UInt32					unk40;			// 40
	UInt32					unk44;			// 44
	UInt32					unk48;			// 48
	UInt32					unk4C;			// 4C
	UInt32					unk50;			// 50
	UInt32					unk54;			// 54
	IDirect3DPixelShader9*	ShaderHandle;	// 58
	void*					ShaderFunction;	// 5C Deallocated after the ShaderHandle creation
	UInt16					StartRegister;	// 60
	UInt8					Vector4Count;	// 62
	UInt8					unk63;			// 63

};
STATIC_ASSERT(sizeof(NiD3DPixelShader) == 0x64);