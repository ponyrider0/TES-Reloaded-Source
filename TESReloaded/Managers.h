#pragma once

#if defined(OBLIVION)
#include "obse\GameObjects.h"
#elif defined(SKYRIM)
#include "skse\GameObjects.h"
#endif
#include "SettingManager.h"
#include "CommandManager.h"
#include "ShaderManager.h"
#include "EffectManager.h"
#include "TextureManager.h"
#include "EquipmentManager.h"
#include "FrameRateManager.h"
#include "UtilityManager.h"
#include "RenderManager.h"

extern SettingManager*		TheSettingManager;
extern RenderManager*		TheRenderManager;
extern ShaderManager*		TheShaderManager;
extern EffectManager*		TheEffectManager;
extern TextureManager*		TheTextureManager;
extern EquipmentManager*	TheEquipmentManager;
extern FrameRateManager*	TheFrameRateManager;
extern UtilityManager*		TheUtilityManager;
extern CommandManager*		TheCommandManager;