#include "FormHook.h"
#include "Hooking\detours\detours.h"
#if defined(OBLIVION)
#define kLoadForm 0x00447050
#elif defined(SKYRIM)
#define kLoadForm 0x0043B4A0
#define typeID formType
#define m_data data
#define kFormType_REFR kFormType_Reference
#endif

bool (__cdecl * LoadForm)(TESForm*, UInt32) = (bool (__cdecl *)(TESForm*, UInt32))kLoadForm;
bool __cdecl TrackLoadForm(TESForm* Form, UInt32 Arg2) {
	
	bool r = LoadForm(Form, Arg2);
	switch (Form->typeID)
	{
		case FormType::kFormType_Sound:
			{
				#if defined(OBLIVION)
					TESSound* Sound = (TESSound*)Form;
					const char* Name = Sound->soundFile.editorID.m_data;
					if (Name) {
						if (!strcmp(Name, "ORBreathingF"))
							TheScriptManager->LowFSound->BreathingF = Sound;
						else if (!strcmp(Name, "ORBreathingM"))
							TheScriptManager->LowFSound->BreathingM = Sound;
						else if (!strcmp(Name, "ORHeartSlow"))
							TheScriptManager->LowHSound->HeartSlow = Sound;
					}
				#endif
			}
			break;
		case FormType::kFormType_Idle:
			if (TheSettingManager->SettingsMain.EquipmentMode) {
			#if defined(OBLIVION)
				TESIdleForm* Anim = (TESIdleForm*)Form;
				if (!strcmp(Anim->animModel.nifPath.m_data, "Characters\\_Male\\IdleAnims\\oronbackanim.kf")) TheEquipmentManager->OnBackAnim = Anim;
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
				Water->properties.fogNearUW = 9995.0f;
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