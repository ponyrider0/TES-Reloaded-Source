#if defined(SKYRIM)
#include "skse\GameReferences.h"
#define posX pos.x
#define posY pos.y
#define posZ pos.z
#endif

FrameRateManager::FrameRateManager() {

	FrameCounter = 0;
	GridDistant = *SettingGridDistantCount;

}

void FrameRateManager::SetFrame(int FrameRate) {
	
	if (FrameRate < TheSettingManager->SettingsMain.FrameRateAverage - TheSettingManager->SettingsMain.FrameRateGap) {
		FrameCounter++;
		if (FrameCounter == TheSettingManager->SettingsMain.FrameRateDelay) {
			FrameCounter = 0;
			GridDistant -= TheSettingManager->SettingsMain.FrameRateGridStep;
			if (GridDistant < TheSettingManager->SettingsMain.FrameRateGridMin)
				GridDistant = TheSettingManager->SettingsMain.FrameRateGridMin;
			Request = TurnDown;
		}
	}
	else if (FrameRate > TheSettingManager->SettingsMain.FrameRateAverage + TheSettingManager->SettingsMain.FrameRateGap) {
		FrameCounter++;
		if (FrameCounter == TheSettingManager->SettingsMain.FrameRateDelay) {
			FrameCounter = 0;
			if (*SettingLODFadeOutMultActors == 15.0f && *SettingLODFadeOutMultObjects == 15.0f) {
				GridDistant += TheSettingManager->SettingsMain.FrameRateGridStep;
				if (GridDistant > *SettingGridDistantCount)
					GridDistant = *SettingGridDistantCount;
			}
			Request = TurnUp;
		}
	}
	else {
		FrameCounter = 0;
		Request = None;
	}
	SetNearObjects();

}

void FrameRateManager::SetNearObjects() {

	if (Request == TurnDown) {
		if (GridDistant == TheSettingManager->SettingsMain.FrameRateGridMin) {
			if (*SettingLODFadeOutMultObjects > TheSettingManager->SettingsMain.FrameRateFadeMinObjects) {
				*SettingLODFadeOutMultItems = *SettingLODFadeOutMultObjects = *SettingLODFadeOutMultObjects - TheSettingManager->SettingsMain.FrameRateFadeStep;
				if (*SettingLODFadeOutMultObjects < TheSettingManager->SettingsMain.FrameRateFadeMinObjects)
					*SettingLODFadeOutMultItems = *SettingLODFadeOutMultObjects = TheSettingManager->SettingsMain.FrameRateFadeMinObjects;
			}
			else if (*SettingLODFadeOutMultActors > TheSettingManager->SettingsMain.FrameRateFadeMinActors) {
				*SettingLODFadeOutMultActors = *SettingLODFadeOutMultActors - TheSettingManager->SettingsMain.FrameRateFadeStep;
				if (*SettingLODFadeOutMultActors < TheSettingManager->SettingsMain.FrameRateFadeMinActors)
					*SettingLODFadeOutMultActors = TheSettingManager->SettingsMain.FrameRateFadeMinActors;
			}
		}
	}
	else if (Request == TurnUp) {
		if (*SettingLODFadeOutMultActors < 15.0f) {
			*SettingLODFadeOutMultActors = *SettingLODFadeOutMultActors + TheSettingManager->SettingsMain.FrameRateFadeStep;
			if (*SettingLODFadeOutMultActors > 15.0f)
				*SettingLODFadeOutMultActors = 15.0f;
		}
		else if (*SettingLODFadeOutMultObjects < 15.0f) {
			*SettingLODFadeOutMultItems = *SettingLODFadeOutMultObjects = *SettingLODFadeOutMultObjects + TheSettingManager->SettingsMain.FrameRateFadeStep;
			if (*SettingLODFadeOutMultObjects > 15.0f)
				*SettingLODFadeOutMultItems = *SettingLODFadeOutMultObjects = 15;
		}
	}

}

bool FrameRateManager::IsOutGrid(NiAVObject* Object) {
	
	float Distance = (GridDistant + *SettingGridsToLoad / 2) * 4096;

	if (GetDistance(Object) > Distance)
		return true;
	else
		return false;

}

float FrameRateManager::GetDistance(NiAVObject* Object) {

	Vector3 v;
	
	v.x = Object->m_worldTransform.pos.x - (*g_thePlayer)->posX;
	v.y = Object->m_worldTransform.pos.y - (*g_thePlayer)->posY;
	v.z = Object->m_worldTransform.pos.z - (*g_thePlayer)->posZ;
	return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

}