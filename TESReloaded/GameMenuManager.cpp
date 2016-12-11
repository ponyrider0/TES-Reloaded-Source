#include <string>
#include <ctime>
#define TextColorNormal D3DCOLOR_XRGB(TheSettingManager->SettingsMain.MenuTextColorNormal[0], TheSettingManager->SettingsMain.MenuTextColorNormal[1], TheSettingManager->SettingsMain.MenuTextColorNormal[2])
#define TextShadowColorNormal D3DCOLOR_XRGB(TheSettingManager->SettingsMain.MenuTextShadowColorNormal[0], TheSettingManager->SettingsMain.MenuTextShadowColorNormal[1], TheSettingManager->SettingsMain.MenuTextShadowColorNormal[2])
#define TextColorSelected D3DCOLOR_XRGB(TheSettingManager->SettingsMain.MenuTextColorSelected[0], TheSettingManager->SettingsMain.MenuTextColorSelected[1], TheSettingManager->SettingsMain.MenuTextColorSelected[2])
#define TextShadowColorSelected D3DCOLOR_XRGB(TheSettingManager->SettingsMain.MenuTextShadowColorSelected[0], TheSettingManager->SettingsMain.MenuTextShadowColorSelected[1], TheSettingManager->SettingsMain.MenuTextShadowColorSelected[2])
#define TextColorEditing D3DCOLOR_XRGB(TheSettingManager->SettingsMain.MenuTextColorEditing[0], TheSettingManager->SettingsMain.MenuTextColorEditing[1], TheSettingManager->SettingsMain.MenuTextColorEditing[2])
#define TextShadowColorEditing D3DCOLOR_XRGB(TheSettingManager->SettingsMain.MenuTextShadowColorEditing[0], TheSettingManager->SettingsMain.MenuTextShadowColorEditing[1], TheSettingManager->SettingsMain.MenuTextShadowColorEditing[2])
#define TextFont TheSettingManager->SettingsMain.MenuFont
#define TextSizeNormal TheSettingManager->SettingsMain.MenuTextSize
#define MaxRows TheSettingManager->SettingsMain.MenuRowsPerPage
#define RowSpace TheSettingManager->SettingsMain.MenuRowSpace
#define TitleRectX TheSettingManager->SettingsMain.MenuTitleX
#define TitleRectY TheSettingManager->SettingsMain.MenuTitleY
#define TitleRectSize TheSettingManager->SettingsMain.MenuTitleRect
#define MenuRectX TheSettingManager->SettingsMain.MenuX
#define MenuRectY TheSettingManager->SettingsMain.MenuY
#define MenuRectSize TheSettingManager->SettingsMain.MenuRect
#define MenuRectSizeExtra TheSettingManager->SettingsMain.MenuExtraRect
#if defined(OBLIVION)
	static char* Title = "Oblivion Reloaded - Settings";
#elif defined(SKYRIM)
	static char* Title = "Skyrim Reloaded - Settings";
#endif

GameMenuManager::GameMenuManager()
{

	_MESSAGE("Starting the menu manager...");
	TheGameMenuManager = this;

	SelectedColumn = 0;
	SelectedStatus = false;
	SelectedRow[0] = SelectedRow[1] = SelectedRow[2] = 0;
	SelectedPage[0] = SelectedPage[1] = SelectedPage[2] = 0;
	Enabled = false;
	EditingMode = false;
	LastTime = 0;
	FrameRate = 0;
	RectTitle = { TitleRectX, TitleRectY, TitleRectX + TitleRectSize, TitleRectY + TextSizeNormal };
	RectLine = new D3DRECT { TitleRectX, TitleRectY + TextSizeNormal + 2, TitleRectX + TitleRectSize, TitleRectY + TextSizeNormal + 4 };
	SetRect(&RectStatus, RectLine->x1, RectLine->y1 + 4, RectLine->x2, RectLine->y2 + TextSizeNormal);
	D3DXCreateFont(TheRenderManager->device, TextSizeNormal, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, TextFont, &FontNormal);
	D3DXCreateFont(TheRenderManager->device, TextSizeNormal, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, TextFont, &FontSelected);
	memcpy(IntValues, "ScreenshotKeyGrassDensityLightShaftPasses\0", 42);
	memcpy(BoolValues, "EnabledDialogModeRestDistantBlurSunGlareEnabledFPSOverlay\0", 58);

}

void GameMenuManager::Render()
{

	if (!TheUtilityManager->IsConsole() && TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeyEnable)) {
		Enabled = !Enabled;
		EditingMode = false;
	}

	if (Enabled) {
		const char* Text = NULL;
		char SettingFirst[40];
		size_t ListSize = 0;

		if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeyEditing) && SelectedColumn == 2) {
			if (!EditingMode)
				sprintf(EditingValue, TheSettingManager->SettingsMain.MenuValueFormat, SelectedValue);
			else
				TheSettingManager->SetSetting(SelectedShader, SelectedSection, SelectedSetting, atof(EditingValue));
			EditingMode = !EditingMode;
		}

		if (!EditingMode) {
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeyUp) && SelectedRow[SelectedColumn] > 0) SelectedRow[SelectedColumn] -= 1;
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeyDown) && SelectedRow[SelectedColumn] < Rows[SelectedColumn] - 1) SelectedRow[SelectedColumn] += 1;
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeyLeft)) {
				if (SelectedColumn > 0) SelectedColumn -= 1;
				SelectedRow[SelectedColumn + 1] = 0;
			}
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeyRight) && SelectedColumn < 2) SelectedColumn += 1;
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeyPageUp) && SelectedPage[SelectedColumn] > 0) {
				SelectedPage[SelectedColumn] -= 1;
				SelectedRow[SelectedColumn] = 0;
			}
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeyPageDown) && SelectedPage[SelectedColumn] < Pages[SelectedColumn]) {
				SelectedPage[SelectedColumn] += 1;
				SelectedRow[SelectedColumn] = 0;
			}
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeyAdd) && SelectedColumn == 0) TheShaderManager->EnableEffect(SelectedShader, !SelectedStatus);
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeySubtract) && SelectedColumn == 0) TheShaderManager->EnableEffect(SelectedShader, !SelectedStatus);
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeyAdd) && SelectedColumn == 2) {
				if (strstr(BoolValues, SelectedSetting))
					SelectedValue = !SelectedValue;
				else if (strstr(IntValues, SelectedSetting))
					SelectedValue += 1.0f;
				else
					SelectedValue += TheSettingManager->SettingsMain.MenuStepValue;
				TheSettingManager->SetSetting(SelectedShader, SelectedSection, SelectedSetting, SelectedValue);
			}
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeySubtract) && SelectedColumn == 2) {
				if (strstr(BoolValues, SelectedSetting))
					SelectedValue = !SelectedValue;
				else if (strstr(IntValues, SelectedSetting))
					SelectedValue -= 1.0f;
				else
					SelectedValue -= TheSettingManager->SettingsMain.MenuStepValue;
				TheSettingManager->SetSetting(SelectedShader, SelectedSection, SelectedSetting, SelectedValue);
			}
			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.MenuKeySave)) {
				TheSettingManager->SaveSettings(SelectedShader);
				TheUtilityManager->ShowMessage("Settings saved.");
			}
		}
		else {
			if (strlen(EditingValue) < 19) {
				if (TheKeyboardManager->OnKeyDown(82))
					strcat(EditingValue, "0");
				else if (TheKeyboardManager->OnKeyDown(79))
					strcat(EditingValue, "1");
				else if (TheKeyboardManager->OnKeyDown(80))
					strcat(EditingValue, "2");
				else if (TheKeyboardManager->OnKeyDown(81))
					strcat(EditingValue, "3");
				else if (TheKeyboardManager->OnKeyDown(75))
					strcat(EditingValue, "4");
				else if (TheKeyboardManager->OnKeyDown(76))
					strcat(EditingValue, "5");
				else if (TheKeyboardManager->OnKeyDown(77))
					strcat(EditingValue, "6");
				else if (TheKeyboardManager->OnKeyDown(71))
					strcat(EditingValue, "7");
				else if (TheKeyboardManager->OnKeyDown(72))
					strcat(EditingValue, "8");
				else if (TheKeyboardManager->OnKeyDown(73))
					strcat(EditingValue, "9");
				else if (TheKeyboardManager->OnKeyDown(83))
					strcat(EditingValue, ".");
			}
			if (strlen(EditingValue) > 0 && TheKeyboardManager->OnKeyDown(14)) EditingValue[strlen(EditingValue) - 1] = NULL;
		}

		FontNormal->DrawTextA(NULL, Title, -1, &RectTitle, DT_LEFT, TextColorNormal);
		TheRenderManager->device->Clear(1, RectLine, D3DCLEAR_TARGET, TextColorNormal, 0, NULL);

		Rows[0] = 0;
		SetRect(&Rect, MenuRectX + MenuRectSize * 0, MenuRectY, MenuRectX + MenuRectSize * 1, MenuRectY + TextSizeNormal);
		ShadersList Shaders = TheSettingManager->GetShaders();
		ListSize = Shaders.size();
		Pages[0] = ListSize / MaxRows;
		ShadersList::iterator Shader = Shaders.begin();
		for (int i = 0; i < ListSize; i++) {
			if (i >= MaxRows * SelectedPage[0] && i < MaxRows * (SelectedPage[0] + 1)) {
				Text = Shader->second.c_str();
				Rect.top += TextSizeNormal + RowSpace;
				Rect.bottom += TextSizeNormal + RowSpace;
				SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
				if (SelectedRow[0] == Rows[0]) {
					strcpy(SelectedShader, Shader->first.c_str());
					SelectedStatus = TheSettingManager->GetEnabled(SelectedShader);
					if (strcmp(SelectedShader, " Main")) {
						if (SelectedStatus)
							FontNormal->DrawTextA(NULL, "Status: ENABLED", -1, &RectStatus, DT_LEFT, TextColorSelected);
						else
							FontNormal->DrawTextA(NULL, "Status: DISABLED", -1, &RectStatus, DT_LEFT, TextColorSelected);
					}
					FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorSelected);
					FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorSelected);
				}
				else {
					FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
					FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
				}
				Rows[0]++;
			}
			Shader++;
		}

		Rows[1] = 0;
		SetRect(&Rect, MenuRectX + MenuRectSize * 1, MenuRectY, MenuRectX + MenuRectSize * 2, MenuRectY + TextSizeNormal);
		SectionsList Sections = TheSettingManager->GetSections(SelectedShader);
		ListSize = Sections.size();
		Pages[1] = ListSize / MaxRows;
		SectionsList::iterator Section = Sections.begin();
		for (int i = 0; i < ListSize; i++) {
			if (i >= MaxRows * SelectedPage[1] && i < MaxRows * (SelectedPage[1] + 1)) {
				Text = Section->c_str();
				Rect.top += TextSizeNormal + RowSpace;
				Rect.bottom += TextSizeNormal + RowSpace;
				SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
				if (SelectedRow[1] == Rows[1]) {
					strcpy(SelectedSection, Text);
					if (SelectedColumn >= 1) {
						FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorSelected);
						FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorSelected);
					}
					else {
						FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
						FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
					}
				}
				else {
					FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
					FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
				}
				Rows[1]++;
			}
			Section++;
		}

		Rows[2] = 0;
		SetRect(&Rect, MenuRectX + MenuRectSize * 2, MenuRectY, MenuRectX + MenuRectSize * 3 + MenuRectSizeExtra, MenuRectY + TextSizeNormal);
		SettingsList Settings = TheSettingManager->GetSettings(SelectedShader, SelectedSection);
		ListSize = Settings.size();
		Pages[2] = ListSize / MaxRows;
		SettingsList::iterator Setting = Settings.begin();
		for (int i = 0; i < ListSize; i++) {
			if (i >= MaxRows * SelectedPage[2] && i < MaxRows * (SelectedPage[2] + 1)) {
				strcpy(SettingFirst, Setting->first.c_str());
				if (strstr(IntValues, SettingFirst) || strstr(BoolValues, SettingFirst))
					sprintf(SettingValue, "%.0f", Setting->second);
				else
					sprintf(SettingValue, TheSettingManager->SettingsMain.MenuValueFormat, Setting->second);
				std::string t = Setting->first + " = " + SettingValue;
				Text = t.c_str();
				Rect.top += TextSizeNormal + RowSpace;
				Rect.bottom += TextSizeNormal + RowSpace;
				SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
				if (SelectedRow[2] == Rows[2]) {
					strcpy(SelectedSetting, SettingFirst);
					SelectedValue = Setting->second;
					if (SelectedColumn == 2) {
						if (EditingMode) {
							t = Setting->first + " = " + EditingValue;
							Text = t.c_str();
							FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorEditing);
							FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorEditing);
						}
						else {
							FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorSelected);
							FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorSelected);
						}
					}
					else {
						FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
						FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
					}
				}
				else {
					FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
					FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
				}
				Rows[2]++;
			}
			Setting++;
		}
	}
	
	if (TheSettingManager->SettingsMain.FPSOverlay) {
		char Text[4];

		if (difftime(TheFrameRateManager->CurrentTime, LastTime) >= 1.0f) FrameRate = TheFrameRateManager->FrameRate;
		sprintf(Text, "%d", FrameRate);
		SetRect(&Rect, 9, 6, 9 + TitleRectSize, 6 + TextSizeNormal);
		SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
		FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorSelected);
		FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorSelected);
		LastTime = TheFrameRateManager->CurrentTime;
	}

}
