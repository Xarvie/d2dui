#pragma once
#include "GuiBase.h"
class GuiLabel :public GuiBase
{
public:
	void WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam);
};