#pragma once

#if defined(OBLIVION)
#include "obse\GameObjects.h"
#elif defined(SKYRIM)
#include "skse\GameObjects.h"
#endif
#include "SettingManager.h"
#include "CommandManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "EquipmentManager.h"
#include "FrameRateManager.h"
#include "UtilityManager.h"
#include "GameEventManager.h"
#include "GameMenuManager.h"
#include "KeyboardManager.h"
#include "ScriptManager.h"
#include "RenderManager.h"

extern SettingManager*		TheSettingManager;
extern RenderManager*		TheRenderManager;
extern ShaderManager*		TheShaderManager;
extern TextureManager*		TheTextureManager;
extern EquipmentManager*	TheEquipmentManager;
extern FrameRateManager*	TheFrameRateManager;
extern UtilityManager*		TheUtilityManager;
extern CommandManager*		TheCommandManager;
extern GameEventManager*	TheGameEventManager;
extern GameMenuManager*		TheGameMenuManager;
extern KeyboardManager*		TheKeyboardManager;
extern ScriptManager*		TheScriptManager;