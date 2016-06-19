#pragma once
#include "GuiWindow.h"
#include "GuiLabel.h"
#include "GuiButton.h"
namespace GuiNew
{
	
	GuiWindow* NewWindow(
		LPCWSTR         _title = L"����",
		float             _x = 200.F,
		float             _y = 200.F,
		float             _width = 500.F,
		float             _height = 300.F,
		D2D1_COLOR_F    _colorBorder = D2D1::ColorF(0x007ACC, 1.0F),
		D2D1_COLOR_F    _colorBg = D2D1::ColorF(0x1E1E1E, 1.0F),
		int             _layer = 0,
		bool            _visible = true
	);
	GuiLabel* NewLabel(
		GuiWindow*      _window,
		LPCWSTR         _title = L"Title",
		float             _x = 0.F,
		float             _y = 0.F,
		float             _width = 200.F,
		float             _height = 90.F,
		D2D1_COLOR_F    _colorBorder = D2D1::ColorF(0x007ACC, 1.0F),
		D2D1_COLOR_F    _colorBg = D2D1::ColorF(0x1E1E1E, 1.0F),
		bool            _visible = true
	);
	GuiButton* NewButton(
		GuiWindow*      _window,
		LPCWSTR         _title = L"Title",
		float             _x = 0.F,
		float             _y = 0.F,
		float             _width = 200.F,
		float             _height = 90.F,
		D2D1_COLOR_F    _colorBorder = D2D1::ColorF(0x007ACC, 1.0F),
		D2D1_COLOR_F    _colorBg = D2D1::ColorF(0x1E1E1E, 1.0F),
		bool            _visible = true
	);
}