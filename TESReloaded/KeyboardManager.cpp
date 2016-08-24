#include "skse\GameInput.h"

KeyboardManager::KeyboardManager()
{

	_MESSAGE("Starting the keyboard manager...");

}

bool KeyboardManager::OnKeyDown(UInt8 KeyCode)
{

	UInt8* PreviousState = (*g_inputEventDispatcher)->keyboard->PreviousState;
	UInt8* CurrentState = (*g_inputEventDispatcher)->keyboard->CurrentState;

	if (CurrentState[KeyCode] && !PreviousState[KeyCode]) return true;
	return false;

}

bool KeyboardManager::OnKeyUp(UInt8 KeyCode)
{
	
	UInt8* PreviousState = (*g_inputEventDispatcher)->keyboard->PreviousState;
	UInt8* CurrentState = (*g_inputEventDispatcher)->keyboard->CurrentState;

	if (PreviousState[KeyCode] && !CurrentState[KeyCode]) return true;
	return false;

}
