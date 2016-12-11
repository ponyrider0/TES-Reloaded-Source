#pragma once

class KeyboardManager // Never disposed
{

public:
	KeyboardManager();

	bool	OnKeyDown(UInt8 KeyCode);
	bool	OnKeyUp(UInt8 KeyCode);
	bool	OnMouseDown(UInt8 ButtonIndex);
	bool	OnMouseUp(UInt8 ButtonIndex);

};