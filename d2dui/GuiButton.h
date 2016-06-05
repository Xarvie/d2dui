#pragma once
#include "GuiWindow.h"

class GuiButton:public GuiWindow
{
public:
	HWND hwnd;
	GuiButton(GuiWindow * gw);
	GuiButton();
	void NewButton(GuiWindow* gw);
	virtual void WndMsgProc(UINT message, WPARAM wparam, LPARAM lparam);
};