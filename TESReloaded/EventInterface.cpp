#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#define wtype type
#elif defined(SKYRIM)
#include "skse\SafeWrite.h"
#define wtype gameData.type
#endif
#include "EventInterface.h"

static EventManager *m_events;

static const UInt32 kOnHitWithHook = 0x005FF61A;
static const UInt32 kOnHitHook = 0x005FF630;
#if defined(OBLIVION)
UInt8 (__cdecl * RaiseEvent)(TESForm*, BaseExtraList*, UInt32) = (UInt8(*)(TESForm*, BaseExtraList*, UInt32))0x004FBF90;

void OnHitWith(TESForm* baseForm, BaseExtraList* baseExtraList, UInt32 eventMask)
{
	
	//m_events->Target = (TESObjectREFR*)((int)baseExtraList - 0x44);
	m_events->Weapon = (TESObjectWEAP*)Oblivion_DynamicCast(baseForm, 0, RTTI_TESForm, RTTI_TESObjectWEAP, 0);

	RaiseEvent(baseForm, baseExtraList, eventMask);

}

void OnHit(TESForm* baseForm, BaseExtraList* baseExtraList, UInt32 eventMask)
{

	if (baseForm->refID == (*g_thePlayer)->refID)
		m_events->OnHitByPlayer();
	RaiseEvent(baseForm, baseExtraList, eventMask);

}
#endif
void CreateEventHook() {
#if defined(OBLIVION)
	WriteRelCall(kOnHitWithHook, (UInt32)OnHitWith);
	WriteRelCall(kOnHitHook, (UInt32)OnHit);
#endif
	m_events = EventManager::GetSingleton();

}

EventManager *EventManager::Singleton = NULL;

EventManager::EventManager() {

	//Target = NULL;
	Weapon = NULL;

}

EventManager::~EventManager() {

	Singleton = NULL;

}

EventManager *EventManager::GetSingleton() {

	if (!Singleton)
		Singleton = new EventManager();

	return Singleton;

}

void EventManager::OnHitByPlayer()
{

	if (Weapon && Weapon->wtype <= 3 && TheShaderManager->ShaderConst.BloodLens_Percent == 0) {
		float RandomPercent = (double)rand() / (RAND_MAX + 1) * (100 - 1) + 1;
		if (RandomPercent <= TheSettingManager->SettingsBlood.LensChance)
			TheShaderManager->ShaderConst.BloodLens_Percent = 1;
	}
	m_events->Weapon = NULL;

}