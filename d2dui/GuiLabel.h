#pragma once
#include "GuiBase.h"
#include "GuiWindow.h"
class GuiLabel :public GuiBase
{
public:
	/// <summary>
	/// ��Ϣ��Ӧ����
	/// </summary>
	/// <param name="hwnd">���.</param>
	/// <param name="message">��Ϣ.</param>
	/// <param name="wparam">wparam.</param>
	/// <param name="lparam">lparam.</param>
	/// <returns>����1�������������Ϣ</returns>
	int WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam);
	void NewLabel(
		GuiWindow*      _window,
		LPCWSTR         _title = L"Title",
		int             _x = 0,
		int             _y = 0,
		int             _width = 200,
		int             _height = 90,
		D2D1_COLOR_F    _colorBorder = D2D1::ColorF(0x007ACC, 1.0F),
		D2D1_COLOR_F    _colorBg = D2D1::ColorF(0x1E1E1E, 1.0F),
		bool            _visible = true
	);
	void Refresh();
};