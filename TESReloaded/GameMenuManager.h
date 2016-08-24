#pragma once

#include <d3dx9core.h>

class GameMenuManager // Never disposed
{

public:
	GameMenuManager();

	void			Render();
	bool			Enabled;
	bool			EditingMode;
	UInt8			Pages[3];
	UInt8			SelectedPage[3];
	UInt8			Rows[3];
	UInt8			SelectedRow[3];
	UInt8			SelectedColumn;
	bool			SelectedStatus;
	float			SelectedValue;
	char			SettingValue[20];
	char			EditingValue[20];
	char			SelectedShader[40];
	char			SelectedSection[40];
	char			SelectedSetting[40];

private:
	ID3DXFont*		FontSelected;
	ID3DXFont*		FontNormal;
	D3DRECT*		RectLine;
	RECT			RectTitle;
	RECT			RectStatus;
	RECT			Rect;
	RECT			RectShadow;

};