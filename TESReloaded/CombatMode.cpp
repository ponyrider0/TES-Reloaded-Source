#if defined(OBLIVION)
#include "CombatMode.h"
#include "obse_common\SafeWrite.h"

static const UInt32 kPlayerReadyWeaponHook = 0x00671E37;
static const UInt32 kActorReadyWeaponHook = 0x005FDA4F;
static const UInt32 kActorReadyWeaponSittingHook = 0x005FD904;
static const UInt32 kActorReadyWeaponSittingReturn = 0x005FD910;
static const UInt32 kPlayerAttackHook = 0x00672612;
static const UInt32 kPlayerAttackReturn = 0x0067261F;
static const UInt32 kHittingMountedCreatureHook = 0x005FF017;
static const UInt32 kHittingMountedCreatureReturn = 0x005FF01C;
static const UInt32 kWeaponRotationHook = 0x006042D4;
static const UInt32 kWeaponRotationReturn = 0x006042DB;
static const UInt32 kBowEquipHook = 0x004E1AC4;
static const UInt32 kBowEquipReturn = 0x004E1AC9;
static const UInt32 kAnimControllerHook = 0x004732F4;
static const UInt32 kAnimControllerReturn1 = 0x004732FA;
static const UInt32 kAnimControllerReturn2 = 0x00473314;
static const UInt32 kHorsePaletteHook = 0x007165B9;
static const UInt32 kHorsePaletteReturn1 = 0x007165C2;
static const UInt32 kHorsePaletteReturn2 = 0x0071661B;
static const UInt32 kBowUnequipHook = 0x005F34AB;
static const UInt32 kBowUnequipReturn = 0x005F34B0;

static bool EquippingBow = 0;

// This function is the bool 0x005E3350.
// It cannot be detoured and modified at global level. We need to modify it only when it is called from the specific hook.
static __declspec(naked) void ReadyWeaponHook()
{

	__asm
	{
		push    esi
		mov     esi, ecx
		mov     eax, dword ptr [esi]
		mov     edx, dword ptr [eax+0x1A0]
		call    edx
		test    al, al
		jnz     short loc_5E33A6
		mov     eax, dword ptr [esi]
		mov     edx, dword ptr [eax+0x198]
		push    0
		mov     ecx, esi
		call    edx
		test    al, al
		jnz     short loc_5E33A6
		mov     eax, dword ptr [esi]
		mov     edx, dword ptr [eax+0x19C]
		mov     ecx, esi
		call    edx
		test    al, al
		jnz     short loc_5E33A6
		mov     eax, dword ptr [esi+0x0B0]
		cmp     eax, 5
		jz      short loc_5E33A6
		cmp     eax, 3
		jz      short loc_5E33A6
		mov     eax, dword ptr [esi]
		mov     edx, dword ptr [eax+0x18C]
		mov     ecx, esi
		call    edx
		
		cmp		eax, 4
		jnz		short loc_jumphack
		xor		eax, eax

	loc_jumphack:
		test    eax, eax
		setz    al
		pop     esi
		retn
	loc_5E33A6:
		xor     al, al
		pop     esi
		retn
	}

}

static __declspec(naked) void ActorReadyWeaponSittingHook()
{
	
	__asm
	{
		mov     edx, dword ptr [esi]
		mov     eax, dword ptr [edx+0x18C]
		mov     ecx, esi
		call	eax

		cmp		eax, 4
		jnz		short loc_jumphack
		xor		eax, eax

	loc_jumphack:
		jmp		kActorReadyWeaponSittingReturn
	}
	
}

static __declspec(naked) void PlayerAttackHook()
{

	__asm
	{
		mov		ecx, dword ptr [ebx+0x058]
		mov		edx, dword ptr [ecx]
		mov		eax, dword ptr [edx+0x36C]
		call	eax

		cmp		eax, 4
		jnz		short loc_jumphack
		mov		ecx, dword ptr [ebx+0x0D4]
		test	ecx, ecx
		jz		short loc_jumphack
		xor		eax, eax

	loc_jumphack:
		jmp		kPlayerAttackReturn
	}

}

int HittingMountedCreatureCheck(Actor* Attacker, TESObjectREFR* Target)
{

	int result = 0;

	if (Target) {
		Creature* pCreature = Attacker->GetMountedHorse();
		if (pCreature && pCreature->refID == Target->refID)
			result = 1;
	}
	return result;

}

static __declspec(naked) void HittingMountedCreatureHook()
{

	__asm
	{
		add		esp, 8
		mov		esi, eax
		pushad
		push	esi
		push	edi
		call	HittingMountedCreatureCheck
		pop		edi
		pop		esi
		test	eax, eax
		jz		short loc_jumphack

		popad
		xor		esi, esi
		jmp		kHittingMountedCreatureReturn

loc_jumphack:
		popad
		jmp		kHittingMountedCreatureReturn
	}

}

void UpdateWeaponRotation(Actor* Act)
{

	if (Act->process->GetProcessLevel() <= 1) {
		MiddleHighProcess* Proc = (MiddleHighProcess*)Act->process;
		if (!Proc->WeaponState) {
			NiAVObject* BackWeaponNode = Act->niNode->GetObjectByName("BackWeapon");
			if (BackWeaponNode) {
				NiMatrix33* m = &BackWeaponNode->m_localTransform.rot;
				NiPoint3* v = &BackWeaponNode->m_localTransform.pos;
				NiPoint3 Rotation = { 90.0f, 30.0f, 120.0f };
				TheUtilityManager->GenerateRotationMatrixXYZ(m, &Rotation, 1);
				v->y += 3;
			}
		}
	}

}

static __declspec(naked) void WeaponRotationHook()
{

	__asm
	{
		pushad
		push ebp
		call UpdateWeaponRotation
		pop ebp
		popad
		fld dword ptr [edi+0x32C]
		push ecx
		jmp kWeaponRotationReturn
	}

}

static __declspec(naked) void BowEquipHook()
{

	__asm
	{
		mov		EquippingBow, 1
		mov		eax, 0x005E13D0
		call	eax
		mov		EquippingBow, 0
		jmp		kBowEquipReturn
	}

}

static __declspec(naked) void BowUnequipHook()
{

	__asm
	{
		mov		eax, 0x005E13D0
		call	eax
		mov		eax, [ebp+0x0D4]
		test	eax, eax
		jz		short skip_blend
		push	1
		mov		ecx, eax
		mov		eax, 0x005E13D0
		call	eax
	skip_blend:
		jmp		kBowUnequipReturn
	}

}

static __declspec(naked) void AnimControllerHook()
{

	__asm
	{
		mov		edi, eax
		cmp		edi, ebp
		jz		short skip_controller
		cmp		EquippingBow, 1
		jz		short skip_controller
		jmp		kAnimControllerReturn1

	skip_controller:
		jmp		kAnimControllerReturn2
	}

}


int CheckHorsePalette(NiNode* node, NiDefaultAVObjectPalette* palette) {

	int result = 0;
	Creature* pCreature = (*g_thePlayer)->GetMountedHorse();

	if (pCreature && pCreature->niNode == palette->niNode && !memcmp(node->m_pcName, "Player1stPerson", 15))
		result = 1;

	return result;

}

static __declspec(naked) void HorsePaletteHook()
{

	__asm
	{
		push	ebx
		mov		ebx, dword ptr ss:[esp+0x10]
		test	ebx, ebx
		jz		short skip_node
		pushad
		push	ebx
		push	esi
		call	CheckHorsePalette
		add		esp, 8
		test	al, al
		popad
		jne		short skip_node
		jmp		kHorsePaletteReturn1

	skip_node:
		jmp		kHorsePaletteReturn2
	}

}

void CreateCombatModeHook()
{
	
	WriteRelCall(kPlayerReadyWeaponHook, (UInt32)ReadyWeaponHook);
	WriteRelCall(kActorReadyWeaponHook, (UInt32)ReadyWeaponHook);
	WriteRelJump(kActorReadyWeaponSittingHook, (UInt32)ActorReadyWeaponSittingHook);
	WriteRelJump(kPlayerAttackHook, (UInt32)PlayerAttackHook);
	WriteRelJump(kHittingMountedCreatureHook, (UInt32)HittingMountedCreatureHook);
	WriteRelJump(kWeaponRotationHook, (UInt32)WeaponRotationHook);
	WriteRelJump(kBowEquipHook,(UInt32)BowEquipHook);
	WriteRelJump(kBowUnequipHook,(UInt32)BowUnequipHook);
	WriteRelJump(kAnimControllerHook,(UInt32)AnimControllerHook);
	WriteRelJump(kHorsePaletteHook,(UInt32)HorsePaletteHook);
	WriteRelJump(0x005FB089, 0x005FB0AD); // Enables the possibility to equip the weapon when sitting/mounting
	WriteRelJump(0x005F2F97, 0x005F2FE8); // Enables the possibility to unequip the weapon when sitting/mounting
	WriteRelJump(0x00654DC2, 0x00654E5F); // Skips to hide the backweapon and quiver when sitting/mounting

	SafeWrite16(0x005F4E55, 0xC031); // Enables blockhit animation when mounting
	SafeWrite16(0x005F4F45, 0xC031); // Enables recoil animation when mounting
	SafeWrite16(0x005F4FEF, 0xC031); // Enables stagger animation when mounting

}
#endif