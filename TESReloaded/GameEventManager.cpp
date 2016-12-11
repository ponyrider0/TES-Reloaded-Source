#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#define wtype type
#define formID refID
static const UInt32 kHitEventHook = 0x005FF613;
static const UInt32 kHitEventReturn = 0x005FF618;
#elif defined(SKYRIM)
#include "skse\SafeWrite.h"
#include "skse\GameReferences.h"
#define wtype gameData.type
static const UInt32 kHitEventHook = 0x006E4837;
static const UInt32 kHitEventReturn = 0x006E483E;
#endif

GameEventManager::GameEventManager() {

	_MESSAGE("Starting the event manager...");
	TheGameEventManager = this;

}

void GameEventManager::OnHitByPlayer() {

	float RandomPercent = 0.0f;

	if (TheShaderManager->ShaderConst.BloodLens_Percent == 0) {
		RandomPercent = (double)rand() / (RAND_MAX + 1) * (100 - 1) + 1;
		if (RandomPercent <= TheSettingManager->SettingsBlood.LensChance) TheShaderManager->ShaderConst.BloodLens_Percent = 1;
	}

}

void RaiseEvent(Actor* Act, Actor* TargetAct, GameEventManager::EventEnum GameEvent) {
	
	if (Act) {
		switch (GameEvent) {
			case GameEventManager::OnHit:
				if (Act->formID == (*g_thePlayer)->formID) {
#if defined(OBLIVION)
					HighProcess* Proc = (HighProcess*)Act->process;
					ExtraContainerChanges::EntryData* WeaponData = Proc->equippedWeaponData;
					TESForm* EquippedObject = WeaponData->type;
					if (EquippedObject && EquippedObject->typeID == FormType::kFormType_Weapon && ((TESObjectWEAP*)EquippedObject)->wtype <= TESObjectWEAP::kType_BluntTwoHand) TheGameEventManager->OnHitByPlayer();
#elif defined (SKYRIM)
					TESForm* EquippedObject = Act->GetEquippedObject(false);
					if (EquippedObject && EquippedObject->formType == FormType::kFormType_Weapon && ((TESObjectWEAP*)EquippedObject)->wtype <= TESObjectWEAP::GameData::kType_TwoHandAxe) TheGameEventManager->OnHitByPlayer();
#endif
				}
				break;
			default:
				break;
		}
	}

}

static __declspec(naked) void HitEventHook()
{
#if defined(OBLIVION)
	__asm
	{
		pushad
		push	1
		push	esi
		push	edi
		call	RaiseEvent
		add		esp, 12
		popad
		push	256
		jmp		kHitEventReturn
	}
#elif defined(SKYRIM)
	__asm
	{
		mov		ebp, [esp + 0x24]
		add		esp, 8
		pushad
		push	1
		push	esi
		push	ebp
		call	RaiseEvent
		add		esp, 12
		popad
		jmp		kHitEventReturn
	}
#endif
}

void CreateGameEventHook() {

	WriteRelJump(kHitEventHook, (UInt32)HitEventHook);

}