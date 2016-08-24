#pragma once

#include "GameTypes.h"
#include "GameInput.h"

#include "GameSettings.h"

#include "skse/NiObjects.h"
#include "skse/NiTypes.h"

class PlayerCamera;
class NiNode;

class TESCameraState
{
public:
	TESCameraState();
	virtual ~TESCameraState();

	virtual void OnStateStart();
	virtual void OnStateEnd();
	virtual void OnUpdate(void * unk1);
	virtual void Unk_04();
	virtual void Unk_05();
	virtual void Unk_06();
	virtual void Unk_07();
	virtual void Unk_08();

	BSIntrusiveRefCounted	refCount;		// 04
	PlayerCamera*			camera;			// 08
	UInt32					stateId;		// 0C
};

class DragonCameraState : public TESCameraState
{
public:
	DragonCameraState();
	virtual ~DragonCameraState();

	PlayerInputHandler		inputHandler;		// 10
	NiNode					* cameraNode;		// 18
	NiNode					* controllerNode;	// 1C
	float					unk20[0x03];		// 20
	UInt32					unk2C[0x07];		// 2C
	float					unk48[0x03];		// 48
	UInt32					unk54[0x11];		// 54
	float					unk98[0x03];		// 98
	UInt32					unkA4[0x04];		// A4
	UInt8					unkB4[0x07];		// B4
	UInt8					padBB;
	// More
};

class HorseCameraState : public TESCameraState
{
public:
	HorseCameraState();
	virtual ~HorseCameraState();

	PlayerInputHandler		inputHandler;		// 10
	NiNode					* cameraNode;		// 18
	NiNode					* controllerNode;	// 1C
	float					unk20[0x03];		// 20
	UInt32					unk2C[0x07];		// 2C
	float					unk48[0x03];		// 48
	UInt32					unk54[0x11];		// 54
	float					unk98[0x03];		// 98
	UInt32					unkA4[0x04];		// A4
	UInt8					unkB4[0x07];		// B4
	UInt8					padBB;
	// More
};

class TweenMenuCameraState : public TESCameraState
{
public:
	TweenMenuCameraState();
	virtual ~TweenMenuCameraState();

	UInt32	unk10[0x0B];	// 10
	UInt8	unk3C;			// 3C
	UInt8	pad3D;			// 3D
	UInt16	pad3E;			// 3E
};

class VATSCameraState : public TESCameraState
{
public:
	VATSCameraState();
	virtual ~VATSCameraState();

	UInt32	unk10[0x03];	// 10
};

class FreeCameraState : public TESCameraState
{
public:
	FreeCameraState();
	virtual ~FreeCameraState();

	PlayerInputHandler		inputHandler;		// 10
	NiPoint3				CameraPosition;		// 18
	UInt32					unk24[0x04];		// 24
	UInt16					unk34;				// 34
	UInt8					unk36;				// 36
	UInt8					unk37;				// 37
};

class AutoVanityState : public TESCameraState
{
public:
	AutoVanityState();
	virtual ~AutoVanityState();
};

class FurnitureCameraState : public TESCameraState
{
public:
	FurnitureCameraState();
	virtual ~FurnitureCameraState();

	UInt32	unk10;	// 10
	float	unk14;	// 14
	float	unk18;	// 18
	float	unk1C;	// 1C
	UInt32	unk20;	// 20
	UInt32	unk24;	// 24
	UInt32	unk28;	// 28
	UInt8	unk2C;	// 2C
	UInt8	unk2D;	// 2D
	UInt8	unk2E;	// 2E
	UInt8	pad2F;	// 2F
};

class IronSightsState : public TESCameraState
{
public:
	IronSightsState();
	virtual ~IronSightsState();

	UInt32	unk10;	// 10
};

class FirstPersonState : public TESCameraState
{
public:
	FirstPersonState();
	virtual ~FirstPersonState();

	PlayerInputHandler		inputHandler;		// 10
	NiNode					* cameraNode;		// 18
	NiNode					* controllerNode;	// 1C
	UInt32					unk20;				// 20
	float					unk24;				// 24
	float					unk28;				// 28
	UInt32					unk2C[0x03];		// 2C
	UInt8					unk38[0x05];		// 38
	UInt8					pad3D;				// 3D
	UInt16					pad3E;				// 3E
};

class ThirdPersonState : public TESCameraState
{
public:
	ThirdPersonState();
	virtual ~ThirdPersonState();
	virtual void Unk_09();
	virtual void Unk_0A();
	virtual void UpdateOverShoulder(bool IsWeaponOut);
	virtual void Unk_0C();
	virtual void PlayerToCamera(bool AllowVanityMode);
	virtual void Unk_0E();
	virtual void Unk_0F();

	PlayerInputHandler		inputHandler;				// 10
	NiNode					* cameraNode;				// 18
	NiNode					* controllerNode;			// 1C
	NiPoint3				CameraPosition;				// 20
	float					unk2C;						// 2C
	float					unk30;						// 30
	float					unk34;						// 34
	float					unk38;						// 38
	float					OverShoulderPosX;			// 3C
	float					OverShoulderPosY;			// 40
	float					OverShoulderPosZ;			// 44
	float					unk48;						// 48
	float					unk4C;						// 4C
	float					unk50;						// 50
	float					unk54;						// 54
	float					HeadDistance;				// 58
	float					unk5C;						// 5C
	UInt32					unk60;						// 60
	float					unk64;						// 64
	UInt32					unk68;						// 68
	float					unk6C;						// 6C
	NiPoint3				CameraPosition1;			// 70
	float					unk7C;						// 7C
	UInt32					unk80;						// 80
	UInt32					unk84;						// 84
	UInt32					unk88;						// 88
	UInt32					unk8C;						// 8C
	UInt32					unk90;						// 90
	UInt32					unk94;						// 94
	float					unkCameraOffsetX;			// 98
	float					unkCameraOffsetY;			// 9C
	float					unkCameraOffsetZ;			// A0
	UInt32					unkA4;						// A4
	UInt32					unkA8;						// A8
	float					CameraHeadRotZ;				// AC
	float					CameraHeadRotX;				// B0
	UInt8					unkCamMode;					// B4
	UInt8					unkB5;						// B5
	UInt8					unkB6;						// B6
	UInt8					unkB7;						// B7
	UInt8					AnimCamMode;				// B8
	UInt8					AllowCameraRotation;		// B9
	UInt8					TogglePOV;					// BA
	UInt8					padBA;
};
STATIC_ASSERT(offsetof(ThirdPersonState, HeadDistance) == 0x58);
STATIC_ASSERT(sizeof(ThirdPersonState) == 0xBC);

class BleedoutCameraState : public ThirdPersonState
{
public:
	BleedoutCameraState();
	virtual ~BleedoutCameraState();

	PlayerInputHandler		inputHandler;		// 10
	NiNode					* cameraNode;		// 18
	NiNode					* controllerNode;	// 1C
	float					unk20[0x03];		// 20
	UInt32					unk2C[0x07];		// 2C
	float					unk48[0x03];		// 48
	UInt32					unk54[0x11];		// 54
	float					unk98[0x03];		// 98
	UInt32					unkA4[0x04];		// A4
	UInt8					unkB4[0x07];		// B4
	UInt8					padBB;
	// More
};

class PlayerCameraTransitionState : public TESCameraState
{
public:
	PlayerCameraTransitionState();
	virtual ~PlayerCameraTransitionState();

	UInt32	unk10[0x05];	// 10
	UInt8	unk24;			// 24
	UInt8	unk25;			// 25
	UInt16	pad26;
};

class TESCamera
{
public:
	TESCamera();
	virtual ~TESCamera();

	virtual void SetNode(NiNode * node);
	virtual void Update();

	float			rotZ;			// 04
	float			rotX;			// 08
	NiPoint3		pos;			// 0C
	float			zoom;			// 18
	NiNode*			cameraNode;		// 1C - First child is usually NiCamera
	TESCameraState* cameraState;	// 20
	UInt8			unk24;			// 24
	UInt8			pad24[3];

	MEMBER_FN_PREFIX(TESCamera);
	DEFINE_MEMBER_FN(SetCameraState, UInt32, 0x006533D0, TESCameraState * cameraState);
};

STATIC_ASSERT(offsetof(TESCamera, cameraNode) == 0x1C);

class LocalMapCamera : public TESCamera
{
public:
	LocalMapCamera();
	virtual ~LocalMapCamera();

	class DefaultState : public TESCameraState
	{
	public:
		NiPoint3	someBoundMax;		// 10
		NiPoint3	someBoundMin;		// 1C
		float		zoomPercent;		// 28
		float		minFrustumWidth;	// 2C
		float		minFrustumHeight;	// 30
	};

	NiPoint3		areaBoundsMax;		// 28
	NiPoint3		areaBoundsMin;		// 34
	DefaultState*	defaultState;		// 40
	NiObject*		niCamera;			// 44
	float			northRotation;		// 48

	void SetDefaultStateMinFrustumDimensions(float width, float height);
	void SetAreaBounds(NiPoint3 * maxBound, NiPoint3 * minBound);
	void SetDefaultStateMaxBound(NiPoint3 * maxBound);
	void SetDefaultStateBounds(float x, float y, float z);

	MEMBER_FN_PREFIX(LocalMapCamera);
	DEFINE_MEMBER_FN(ctor, void, 0x00487420);
	DEFINE_MEMBER_FN(SetNorthRotation, void, 0x00486440, float northRotation);
};

STATIC_ASSERT(offsetof(LocalMapCamera, northRotation) == 0x48);

class MapCamera : public TESCamera
{
public:
	MapCamera();
	virtual ~MapCamera();
};

class RaceSexCamera : public TESCamera
{
public:
	RaceSexCamera();
	virtual ~RaceSexCamera();
};

class PlayerCamera : public TESCamera
{
public:
	PlayerCamera();
	virtual ~PlayerCamera();

	static PlayerCamera *	GetSingleton(void)
	{
		return *((PlayerCamera **)0x012E7288);
	}

	enum
	{
		kCameraState_FirstPerson = 0,
		kCameraState_AutoVanity,
		kCameraState_VATS,
		kCameraState_Free,
		kCameraState_IronSights,
		kCameraState_Furniture,
		kCameraState_Transition,
		kCameraState_TweenMenu,
		kCameraState_ThirdPerson1,
		kCameraState_ThirdPerson2,
		kCameraState_Horse,
		kCameraState_Bleedout,
		kCameraState_Dragon,
		kNumCameraStates
	};
	
	UInt32							unk28;										// 28
	UInt32							unk2C;										// 2C
	UInt32							unk30[(0x6C - 0x30) >> 2];					// 30
	FirstPersonState*				firstPersonState;							// 6C
	AutoVanityState*				autoVanityState;							// 70
	VATSCameraState*				vatsCameraState;							// 74
	FreeCameraState*				freeCameraState;							// 78
	IronSightsState*				ironSightsState;							// 7C
	FurnitureCameraState*			furnitureCameraState;						// 80
	PlayerCameraTransitionState*	playerCameraTransitionState;				// 84
	TweenMenuCameraState*			tweenMenuCameraState;						// 88
	ThirdPersonState*				thirdPersonState1;							// 8C
	ThirdPersonState*				thirdPersonState2;							// 90
	HorseCameraState*				horseCameraState;							// 94
	BleedoutCameraState*			bleedoutCameraState;						// 98
	DragonCameraState*				dragonCameraState;							// 9C
	UInt32							unkA0;										// A0
	UInt32							unkA4;										// A4
	UInt32							unkA8;										// A8
	float							worldFOV;									// AC
	float							firstPersonFOV;								// B0
	UInt32							unkB4[(0xD0 - 0xB4) >> 2];					// B4
	UInt8							EnableAutoVanityMode;						// D0
	UInt8							unkD1;										// D1
	UInt8							AllowVanityMode;							// D2
	UInt8							unkD3;										// D3
	UInt8							unkD4;										// D4
	UInt8							unkD5;										// D5
	UInt8							padD5[2];

	MEMBER_FN_PREFIX(PlayerCamera);
	DEFINE_MEMBER_FN(UpdateThirdPerson, void, 0x0083C7E0, bool weaponDrawn);
};

STATIC_ASSERT(offsetof(PlayerCamera, firstPersonFOV) == 0xB0);
STATIC_ASSERT(sizeof(PlayerCamera) == 0xD8);