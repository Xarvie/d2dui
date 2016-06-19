#pragma once
#include "GuiWindow.h"
#include "GuiLabel.h"
namespace GuiNew
{
	
	GuiWindow* NewWindow(
		LPCWSTR         _title = L"БъЬт",
		int             _x = 200,
		int             _y = 200,
		int             _width = 500,
		int             _height = 300,
		D2D1_COLOR_F    _colorBorder = D2D1::ColorF(0x007ACC, 1.0F),
		D2D1_COLOR_F    _colorBg = D2D1::ColorF(0x1E1E1E, 1.0F),
		int             _layer = 0,
		bool            _visible = true
	);
	GuiLabel* NewLabel(
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
}