#pragma once

#if defined(OBLIVION)
class EquipmentManager // Never disposed
{
public:
	EquipmentManager();

	enum PositionRotationType
	{
		None,
		Shield,
		Weapon,
		Light,
	};

	void			SetRotationPosition(NiAVObject* Object, PositionRotationType PositionRotation);
	void			SetTorchLight(NiAVObject* Torch, UInt8 Enable);
	void			SetAnimations(HighProcess* Process, UInt8 LeftWeapon);
	void			SetAnimGroup(HighProcess* Process, UInt8 AnimGroup, UInt8 AnimType, const char* Filename);
	void			ResetAnimGroup(HighProcess* Process, UInt8 AnimGroup, UInt8 AnimType);

	TESIdleForm*	OnBackAnim;
};

class HighProcessEx : public HighProcess
{
public:
	enum State
	{
		None,
		In,
		Out,
	};

	ExtraContainerChanges::EntryData*	EquippedLeftWeaponData;
	NiNode*								LeftWeaponObject;		// This should be storage extending the SkinInfo class, but the SkinInfo is saved in the savegame so i avoid to touch it for compatibility
	NiNode*								LeftWeaponObjectFirst;	// Same as above
	UInt8								OnBackActionState;
	UInt8								OnBackState;
	UInt8								EquippingState;
	UInt8								LeftEquippingState;
	UInt8								OnBeltActionState;
	UInt8								OnBeltState;
};

void CreateEquipmentHook();
#elif defined(SKYRIM)
class EquipmentManager // Never disposed
{
public:
	EquipmentManager();
};
#endif