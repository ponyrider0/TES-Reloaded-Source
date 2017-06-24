#if defined(OBLIVION)
#include "obse\GameOSDepend.h"
#define KeyboardCurrentState (*g_osGlobals)->input->CurrentKeyState
#define KeyboardPreviousState (*g_osGlobals)->input->PreviousKeyState
#define MouseCurrentState (*g_osGlobals)->input->CurrentMouseState.rgbButtons
#define MousePreviousState (*g_osGlobals)->input->PreviousMouseState.rgbButtons
#elif defined(SKYRIM)
#include "skse\GameInput.h"
#define KeyboardCurrentState (*g_inputEventDispatcher)->keyboard->CurrentKeyState
#define KeyboardPreviousState (*g_inputEventDispatcher)->keyboard->PreviousKeyState
#define MouseCurrentState (*g_inputEventDispatcher)->mouse->CurrentMouseState.rgbButtons
#define MousePreviousState (*g_inputEventDispatcher)->mouse->PreviousMouseState.rgbButtons
#endif

KeyboardManager::KeyboardManager()
{

	_MESSAGE("Starting the keyboard manager...");
	TheKeyboardManager = this;

}

bool KeyboardManager::OnKeyDown(UInt16 KeyCode)
{

	if (KeyCode >= 256) return OnMouseDown(KeyCode - 256);
	if (KeyboardCurrentState[KeyCode] && !KeyboardPreviousState[KeyCode]) return true;
	return false;

}

bool KeyboardManager::OnKeyUp(UInt16 KeyCode)
{

	if (KeyCode >= 256) return OnMouseUp(KeyCode - 256);
	if (!KeyboardCurrentState[KeyCode] && KeyboardPreviousState[KeyCode]) return true;
	return false;

}

bool KeyboardManager::OnMouseDown(UInt8 ButtonIndex)
{

	if (ButtonIndex != 0xFF && MouseCurrentState[ButtonIndex] && !MousePreviousState[ButtonIndex]) return true;
	return false;

}

bool KeyboardManager::OnMouseUp(UInt8 ButtonIndex)
{

	if (ButtonIndex != 0xFF && !MouseCurrentState[ButtonIndex] && MousePreviousState[ButtonIndex]) return true;
	return false;

}
