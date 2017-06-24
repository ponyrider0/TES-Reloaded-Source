#pragma once

class KeyboardManager // Never disposed
{

public:
	KeyboardManager();

	bool	OnKeyDown(UInt16 KeyCode);
	bool	OnKeyUp(UInt16 KeyCode);
	bool	OnMouseDown(UInt8 ButtonIndex);
	bool	OnMouseUp(UInt8 ButtonIndex);

};