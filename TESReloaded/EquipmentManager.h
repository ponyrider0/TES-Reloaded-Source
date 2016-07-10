#pragma once
#if defined(OBLIVION)
#include "obse\NiExtraData.h"
#elif defined(SKYRIM)
#include "skse\NiExtraData.h"
#endif
#include <map>

class NiORExtraData : public NiExtraData
{
public:
	UInt8			vActionState;	// 0C
	UInt8			vShieldState;	// 0D
	UInt8			vPad0E;			// 0E
	UInt8			vPad0F;			// 0F
	TESObjectWEAP*	OriginalWeapon;	// 10
	TESObjectARMO*	LeftWeapon;		// 14
};

struct LeftWeaponData
{
	TESObjectARMO*	LeftWeapon;
	UInt32			Count;
};

typedef std::map<std::string, LeftWeaponData> LeftWeaponList;

class EquipmentManager // Never disposed
{

public:
	EquipmentManager();

	enum ActionState
	{
		Action_None,
		Action_In,
		Action_Out,
	};

	enum ShieldState
	{
		Shield_In,
		Shield_Out,
	};

	enum EquippingState
	{
		Equipping_None,
		Equipping_Shield_In,
		Equipping_Shield_Out,
		Equipping_Weapon_In,
		Equipping_Weapon_Out,
	};

	NiExtraData*				GetExtraData(NiObjectNET* AVObject, const char* Name);
	NiORExtraData*				GetORData(NiNode* DestinationNode);
	void						ResetORData(NiORExtraData* ORData);
	NiORExtraData*				LoadORData(Actor* Act);
	TESObjectARMO*				GetLeftWeapon(TESObjectWEAP* Weapon);
	void						AddLeftWeaponRef(TESObjectARMO* LeftWeapon);
	void						RemoveLeftWeaponRef(TESObjectARMO* LeftWeapon);

	TESObjectARMO*				LeftWeaponTemplate;
	TESIdleForm*				ShieldAnim;
	LeftWeaponList				LeftWeapons;
	UInt8						vEquippingState;

private:
	NiORExtraData*				CreateORData(NiNode* DestinationNode);

};

void CreateEquipmentHook();