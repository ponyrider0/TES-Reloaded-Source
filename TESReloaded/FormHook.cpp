#include "FormHook.h"
#include "Hooking\detours\detours.h"
#if defined(OBLIVION)
#define kLoadForm 0x00447050
#elif defined(SKYRIM)
#define kLoadForm 0x0043B4A0
#define typeID formType
#define m_data data
#endif

bool (__cdecl * LoadForm)(TESForm*, UInt32) = (bool (__cdecl *)(TESForm*, UInt32))kLoadForm;
bool __cdecl TrackLoadForm(TESForm* Form, UInt32 Arg2) {
	
	bool r = LoadForm(Form, Arg2);
	switch (Form->typeID)
	{
		case FormType::kFormType_Armor:
			if (TheSettingManager->SettingsMain.EquipmentMode) {
				TESObjectARMO* Armor = (TESObjectARMO*)Form;
				const char* Name = Armor->fullName.name.m_data; 
				if (Name && !memcmp(Name, "ORLeftWeapon", 12)) TheEquipmentManager->LeftWeaponTemplate = Armor;
			}
			break;
		case FormType::kFormType_Idle:
			if (TheSettingManager->SettingsMain.EquipmentMode) {
#if defined(OBLIVION) // TO DO for Skyrim: find animModel
				TESIdleForm* Idle = (TESIdleForm*)Form;
				if (!memcmp(Idle->animModel.nifPath.m_data, ShieldAnimFile, 39)) TheEquipmentManager->ShieldAnim = Idle;
#endif
			}
			break;
		case FormType::kFormType_Water:
			if (TheSettingManager->SettingsMain.WaterManagement) {
				TESWaterForm* Water = (TESWaterForm*)Form;
#if defined(OBLIVION)
				enum WaterType
				{
					WaterType_Blood,
					WaterType_Lava,
					WaterType_Normal,
				};
				switch (Water->waterType)
				{
					case WaterType_Blood:
						Water->texture.ddsPath.Set("");
						Water->textureBlend = 25;
						break;
					case WaterType_Lava:
						Water->texture.ddsPath.Set("Water\\alternatelavaX.dds");
						Water->textureBlend = 50;
						break;
					case WaterType_Normal:
						Water->texture.ddsPath.Set("");
						Water->textureBlend = 0;
						break;
					default:
						Water->texture.ddsPath.Set("");
						Water->textureBlend = 0;
						break;
				}
				Water->waterSimVals[TESWaterForm::kWaterVal_FogDistNear] = 163830.0f;
				Water->waterSimVals[TESWaterForm::kWaterVal_FogDistFar] = 163835.0f;
#elif defined(SKYRIM)
				Water->properties.fogAmountUW = 0.0f;
				Water->properties.fogNearUW = 10000.0f;
				Water->properties.fogFarUW = 10000.0f;
#endif
			}
			break;
		default:
			break;
	}
	return r;

}

void CreateFormLoadHook()
{

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)LoadForm,		&TrackLoadForm);
	DetourTransactionCommit();

}