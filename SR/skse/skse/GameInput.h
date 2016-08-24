#pragma once

#include "GameTypes.h"
#include "GameEvents.h"
#include "InputMap.h"

// Note: These are different from those in Hooks_DI
enum
{
	kDeviceType_Keyboard = 0,
	kDeviceType_Mouse,
	kDeviceType_Gamepad
};

class BSInputDevice
{
public:
	virtual UInt32	Initialize();
	virtual	void	GetInput(float unk1);
	virtual	void	Unk_03(void);
	virtual bool	IsEnabled();
	virtual			~BSInputDevice();
	virtual void	Unk_06(void);
	
	UInt32			DeviceType;						// 04
	UInt32			unk008;							// 08
	UInt32			unk00C;							// 0C	
};

class BSWin32KeyboardDevice : public BSInputDevice
{
public:
	UInt32			unk010;							// 10
	UInt32			unk014;							// 14
	UInt32			unk018;							// 18
	UInt32			unk01C;							// 1C
	UInt32			unk020;							// 20
	UInt32			unk024;							// 24
	UInt32			unk028[(0x0F4 - 0x028) >> 2];	// 28 This (or part of this) is a class/struct
	UInt8			PreviousState[0x100];			// F4
	UInt8			CurrentState[0x100];			// 1F4
};
STATIC_ASSERT(sizeof(BSWin32KeyboardDevice) == 0x2F4);

class BSWin32MouseDevice : public BSInputDevice
{

};

class BSWin32GamepadDevice : public BSInputDevice
{

};

// 10
class InputEvent
{
public:
	enum
	{
		kEventType_Button = 0,
		kEventType_MouseMove,
		kEventType_Char,
		kEventType_Thumbstick,
		kEventType_DeviceConnect,
		kEventType_Kinect
	};

	virtual					~InputEvent();
	virtual bool			IsIDEvent();
	virtual BSFixedString *	GetControlID();

//	void			** _vtbl;	// 00
	UInt32			deviceType;	// 04
	UInt32			eventType;	// 08
	InputEvent		* next;		// 0C
};

class IDEvent
{
public:
	BSFixedString	controlID;	// 00
};

// 20
class ButtonEvent : public IDEvent, public InputEvent
{
public:
	virtual					~ButtonEvent();
	virtual bool			IsIDEvent();
	virtual BSFixedString *	GetControlID();

	UInt32			keyMask;	// 14
	UInt32			flags;		// 18 (00000038 when ALT is pressed, 0000001D when STRG is pressed)
	float			timer;		// 1C (hold duration)
};

class MouseMoveEvent : public IDEvent, public InputEvent
{
};

// 14
class CharEvent : public InputEvent
{
public:
	UInt32			keyCode;		// 10 (ascii code)
};

class ThumbstickEvent : public IDEvent, public InputEvent
{
public:
	virtual					~ThumbstickEvent();
	virtual bool			IsIDEvent();
	virtual BSFixedString *	GetControlID();

	UInt32	keyMask;	// 14 - b for left stick, c for right stick
	float	x;			// 18
	float	y;			// 1C
};

class DeviceConnectEvent : public InputEvent
{
};

class KinectEvent : public IDEvent, public InputEvent
{
};

class InputEventDispatcher : public EventDispatcher<InputEvent,InputEvent*>
{
public:
	UInt32					unk030;		// 030
	BSWin32KeyboardDevice*	keyboard;	// 034
	BSWin32MouseDevice*		mouse;		// 038
	BSWin32GamepadDevice*	gamepad;	// 03C
	
	bool	IsGamepadEnabled(void);
};
STATIC_ASSERT(offsetof(InputEventDispatcher, gamepad) == 0x03C);

extern InputEventDispatcher ** g_inputEventDispatcher;

template <>
class BSTEventSink <InputEvent>
{
public:
	virtual ~BSTEventSink() {}; // todo?
	virtual	EventResult ReceiveEvent(InputEvent ** evn, InputEventDispatcher * dispatcher) = 0;
};

// 9C
class InputManager
{
public:
	enum
	{
		kContext_Gameplay = 0,
		kContext_MenuMode,
		kContext_Console,
		kContext_ItemMenu,
		kContext_Inventory,
		kContext_DebugText,
		kContext_Favorites,
		kContext_Map,
		kContext_Stats,
		kContext_Cursor,
		kContext_Book,
		kContext_DebugOverlay,
		kContext_Journal,
		kContext_TFCMode,
		kContext_MapDebug,
		kContext_Lockpicking,
		kContext_Favor,
		kContextCount = 17
	};

	struct InputContext
	{
		// 10
		struct Mapping
		{
			BSFixedString	name;		// 00
			UInt32			buttonID;	// 04
			UInt32			sortIndex;	// 08
			UInt32			unk0C;		// 0C
		};

		tArray<Mapping>	keyboardMap;
		tArray<Mapping>	mouseMap;
		tArray<Mapping>	gamepadMap;
	};

	UInt8			pad00[0x34];				// 00
	InputContext	* context[kContextCount];	// 34
	UInt8			pad78[0x20];				// 78
	UInt8			allowTextInput;				// 98
	UInt8			pad99[3];					// 99

	static InputManager *	GetSingleton(void);

	UInt8			AllowTextInput(bool allow);

	UInt32			GetMappedKey(BSFixedString name, UInt32 deviceType, UInt32 contextIdx);

	BSFixedString	GetMappedControl(UInt32 buttonID, UInt32 deviceType, UInt32 contextIdx);
};
STATIC_ASSERT(sizeof(InputManager) == 0x9C);

class PlayerInputHandler
{
public:
	PlayerInputHandler();
	virtual ~PlayerInputHandler();

	virtual void Unk_01();
	virtual void Unk_02();
	virtual void Unk_03();
	virtual void ManageButtonEvent(ButtonEvent* Arg1, int Arg2);

	UInt8		unk04;			// 04
	UInt8		pad04[3];
};

class MovementHandler {};
class LookHandler {};
class SprintHandler {};			// Size: 0x10
class ReadyWeaponHandler {};
class AutoMoveHandler {};
class ToggleRunHandler {};
class ActivateHandler {};		// Size: 0x10
class JumpHandler {};
class ShoutHandler {};			// Size: 0x10
class AttackBlockHandler {};	// Size: 0x28
class RunHandler {};
class SneakHandler {};
class TogglePOVHandler {};		// Size: 0x10

//?
class PlayerControls
{
public:
	virtual			~PlayerControls();
	virtual UInt32	Unk_01();

	struct Modes
	{
		UInt32			LeftRight;		// 00 Left: 0xBF800000 - Right: 0x3F800000
		UInt32			ForwardBack;	// 04 Forward: 0x3F800000 - Back: 0xBF800000
		UInt32			unk08;			// 08
		UInt32			unk0C;			// 0C
		UInt32			unk10;			// 10
		UInt32			unk14;			// 14
		UInt32			unk18;			// 18
		UInt32			unk1C;			// 1C
		UInt32			unk20;			// 20
		UInt8			AutoMove;		// 24
		UInt8			AlwaysRun;		// 25
		UInt8			unk26;			// 26
		UInt8			TogglePOV;		// 27
		UInt16			unk28;			// 28
		UInt8			unk2A;			// 2A
		UInt8			unk2B;			// 2B
		UInt8			unk2C;			// 2C
		UInt8			pad2A[3];
	};

//	void			** _vtbl;					// 000
	BSTEventSink<void*> menuOpenCloseEvent;		// 004
	BSTEventSink<void*> menuModeChangeEvent;	// 008
	BSTEventSink<void*> furnitureEvent;			// 00C
	UInt32				pad010;					// 010
	Modes				modes;					// 014
	UInt32				unk044;					// 044
	UInt32				unk048;					// 048
	UInt32				unk04C;					// 04C
	UInt32				unk050;					// 050
	UInt32				unk054;					// 054
	UInt32				unk058;					// 058
	UInt32				unk05C;					// 05C
	UInt32				unk060;					// 060
	UInt32				unk064;					// 064
	UInt8				Processing;				// 068
	UInt8				pad068[3];
	UInt32				pad06C[(0x114 - 0x06C) >> 2];	// 06C
	UInt32				unk114;					// 114
	UInt32				unk118;					// 118
	UInt32				unk11C;					// 11C
	UInt32				unk120;					// 120
	UInt32				unk124;					// 124
	MovementHandler*	movementHandler;		// 128
	LookHandler*		lookHandler;			// 12C
	SprintHandler*		sprintHandler;			// 130
	ReadyWeaponHandler*	readyWeaponHandler;		// 134
	AutoMoveHandler*	autoMoveHandler;		// 138
	ToggleRunHandler*	toggleRunHandler;		// 13C
	ActivateHandler*	activateHandler;		// 140
	JumpHandler*		jumpHandler;			// 144
	ShoutHandler*		shoutHandler;			// 148
	AttackBlockHandler*	attackBlockHandler;		// 14C
	RunHandler*			runHandler;				// 150
	SneakHandler*		sneakHandler;			// 154
	TogglePOVHandler*	togglePOVHandler;		// 158
	UInt8				unk15C;					// 15C
	UInt8				pad15C[3];

	static PlayerControls *	GetSingleton(void);

	// used by Hooks_Event
	PlayerControls * ctor_Hook(void);
	MEMBER_FN_PREFIX(PlayerControls);
	DEFINE_MEMBER_FN(ctor, PlayerControls *, 0x00774F20);
};
STATIC_ASSERT(offsetof(PlayerControls, movementHandler) == 0x128);
STATIC_ASSERT(sizeof(PlayerControls) == 0x160);

// ?
class MenuControls
{
public:
	virtual			~MenuControls();
	virtual UInt32	Unk_01();

//	void			** _vtbl;		// 00
	UInt8			pad04[0x3C];	// 04
	UInt8			pad40;			// 40
	UInt8			pad41;			// 41
	bool			remapMode;		// 42
	UInt8			pad43;			// 43

	static MenuControls *	GetSingleton(void);
};
STATIC_ASSERT(offsetof(MenuControls, remapMode) == 0x42);

// 1A0
class InputStringHolder
{
public:
	void			* unk00;			// 000
	BSFixedString	forward;			// 004 "Forward"
	BSFixedString	back;				// 008 "Back"
	BSFixedString	strafeLeft;			// 00C "Strafe Left"
	BSFixedString	strafeRight;		// 010 "Strafe Right"
	BSFixedString	move;				// 014 "Move"
	BSFixedString	look;				// 018 "Look"
	BSFixedString	activate;			// 01C "Activate"
	BSFixedString	leftAttack;			// 020 "Left Attack/Block"
	BSFixedString	rightAttack;		// 024 "Right Attack/Block"
	BSFixedString	dualAttack;			// 028 "Dual Attack"
	BSFixedString	forceRelease;		// 02C "ForceRelease"
	BSFixedString	pause;				// 030 "Pause"
	BSFixedString	readyWeapon;		// 034 "Ready Weapon"
	BSFixedString	togglePOV;			// 038 "Toggle POV"
	BSFixedString	jump;				// 03C "Jump"
	BSFixedString	journal;			// 040 "Journal"
	BSFixedString	sprint;				// 044 "Sprint"
	BSFixedString	sneak;				// 048 "Sneak"
	BSFixedString	shout;				// 04C "Shout"
	BSFixedString	kinectShout;		// 050 "KinectShout"
	BSFixedString	grab;				// 054 "Grab"
	BSFixedString	run;				// 058 "Run"
	BSFixedString	toggleRun;			// 05C "Toggle Always Run"
	BSFixedString	autoMove;			// 060 "Auto-Move"
	BSFixedString	quicksave;			// 064 "Quicksave"
	BSFixedString	quickload;			// 068 "Quickload"
	BSFixedString	newSave;			// 06C "NewSave"
	BSFixedString	inventory;			// 070 "Inventory"
	BSFixedString	stats;				// 074 "Stats"
	BSFixedString	map;				// 078 "Map"
	BSFixedString	screenshot;			// 07C "Screenshot"
	BSFixedString	multiScreenshot;	// 080 "Multi-Screenshot"
	BSFixedString	console;			// 084 "Console"
	BSFixedString	cameraPath;			// 088 "CameraPath"
	BSFixedString	tweenMenu;			// 08C "Tween Menu"
	BSFixedString	takeAll;			// 090 "Take All"
	BSFixedString	accept;				// 094 "Accept"
	BSFixedString	cancel;				// 098 "Cancel"
	BSFixedString	up;					// 09C "Up"
	BSFixedString	down;				// 0A0 "Down"
	BSFixedString	left;				// 0A4 "Left"
	BSFixedString	right;				// 0A8 "Right"
	BSFixedString	pageUp;				// 0AC "PageUp"
	BSFixedString	pageDown;			// 0B0 "PageDown"
	BSFixedString	pick;				// 0B4 "Pick"
	BSFixedString	pickNext;			// 0B8 "PickNext"
	BSFixedString	pickPrevious;		// 0BC "PickPrevious"
	BSFixedString	cursor;				// 0C0 "Cursor"
	BSFixedString	kinect;				// 0C4 "Kinect"
	BSFixedString	sprintStart;		// 0C8 "SprintStart"
	BSFixedString	sprintStop;			// 0CC "SprintStop"
	BSFixedString	sneakStart;			// 0D0 "sneakStart"
	BSFixedString	sneakStop;			// 0D4 "sneakStop"
	BSFixedString	blockStart;			// 0D8 "blockStart"
	BSFixedString	blockStop;			// 0DC "blockStop"
	BSFixedString	blockBash;			// 0E0 "blockBash"
	BSFixedString	attackStart;		// 0E4 "attackStart"
	BSFixedString	attackPowerStart;	// 0E8 "attackPowerStart"
	BSFixedString	reverseDirection;	// 0EC "reverseDirection"
	BSFixedString	unequip;			// 0F0 "Unequip"
	BSFixedString	zoomIn;				// 0F4 "Zoom In"
	BSFixedString	zoomOut;			// 0F8 "Zoom Out"
	BSFixedString	rotateItem;			// 0FC "RotateItem"
	BSFixedString	leftStick;			// 100 "Left Stick"
	BSFixedString	prevPage;			// 104 "PrevPage"
	BSFixedString	nextPage;			// 108 "NextPage"
	BSFixedString	prevSubPage;		// 10C "PrevSubPage"
	BSFixedString	nextSubPage;		// 110 "NextSubPage"
	BSFixedString	leftEquip;			// 114 "LeftEquip"
	BSFixedString	rightEquip;			// 118 "RightEquip"
	BSFixedString	toggleFavorite;		// 11C "ToggleFavorite"
	BSFixedString	favorites;			// 120 "Favorites"
	BSFixedString	hotkey1;			// 124 "Hotkey1"
	BSFixedString	hotkey2;			// 128 "Hotkey2"
	BSFixedString	hotkey3;			// 12C "Hotkey3"
	BSFixedString	hotkey4;			// 130 "Hotkey4"
	BSFixedString	hotkey5;			// 134 "Hotkey5"
	BSFixedString	hotkey6;			// 138 "Hotkey6"
	BSFixedString	hotkey7;			// 13C "Hotkey7"
	BSFixedString	hotkey8;			// 140 "Hotkey8"
	BSFixedString	quickInventory;		// 144 "Quick Inventory"
	BSFixedString	quickMagic;			// 148 "Quick Magic"
	BSFixedString	quickStats;			// 14C "Quick Stats"
	BSFixedString	quickMap;			// 150 "Quick Map"
	BSFixedString	toggleCursor;		// 154 "ToggleCursor"
	BSFixedString	wait;				// 158 "Wait"
	BSFixedString	click;				// 15C "Click"
	BSFixedString	mapLookMode;		// 160 "MapLookMode"
	BSFixedString	equip;				// 164 "Equip"
	BSFixedString	dropItem;			// 168 "DropItem"
	BSFixedString	rotate;				// 16C "Rotate"
	BSFixedString	nextFocus;			// 170 "NextFocus"
	BSFixedString	prevFocus;			// 174 "PreviousFocus"
	BSFixedString	setActiveQuest;		// 178 "SetActiveQuest"
	BSFixedString	placePlayerMarker;	// 17C "PlacePlayerMarker"
	BSFixedString	xButton;			// 180 "XButton"
	BSFixedString	yButton;			// 184 "YButton"
	BSFixedString	chargeItem;			// 188 "ChargeItem"
	UInt32			unk18C;				// 18C
	BSFixedString	playerPosition;		// 190 "PlayerPosition"
	BSFixedString	localMap;			// 194 "LocalMap"
	BSFixedString	localMapMoveMode;	// 198 "LocalMapMoveMode"
	BSFixedString	itemZoom;			// 19C "Item Zoom"

	static InputStringHolder *	GetSingleton(void)
	{
		return *((InputStringHolder **)0x01B2E388);
	}
};
STATIC_ASSERT(sizeof(InputStringHolder) == 0x1A0);
