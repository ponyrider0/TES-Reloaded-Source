#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#elif defined(SKYRIM)
#include "skse\SafeWrite.h"
#endif
#include "WindowedMode.h"

#if defined(OBLIVION)
#define kPopupValue 0x0040E7C0
#define kPopupVisibleValue 0x0040E805
#elif defined(SKYRIM)
#define kPopupValue 0x0069D832
#define kPopupVisibleValue 0x0069D877
#endif

void CreateWindowedModeHook() {

#if defined(SKYRIM)
	SafeWrite32(0x012CF5F8, 0);
	SafeWrite32(0x012CF604, 0);
#endif

	SafeWrite32(kPopupValue, WS_POPUP);
	SafeWrite32(kPopupVisibleValue, WS_POPUP | WS_VISIBLE);
	*SettingFullScreen = 0;

}