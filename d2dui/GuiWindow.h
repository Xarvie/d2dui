#pragma once
#include <d2d1.h>
#include <Windows.h>
#include <dwrite.h>
class GuiWindow
{
public:
	GuiWindow();
	~GuiWindow();
	ID2D1HwndRenderTarget*      hwndRenderTarget;
	ID2D1SolidColorBrush*       BrushWhite;
	long MouseX = 0;
	long MouseY = 0;
	bool MouseDown = false;
	void Refresh();
	void Resize();
	void WriteText(const WCHAR* _String, const WCHAR* _FontName = L"微软雅黑", float _Size = 15.0f, int _x = 10, int _y = 5, int _width = 300, int _height = 20);
	void NewWindow(
		GuiWindow*      _window,
		LPCWSTR         _title = L"标题",
		int             _x = 200,
		int             _y = 200,
		int             _width = 500,
		int             _height = 300,
		D2D1_COLOR_F    _colorBorder = D2D1::ColorF(0x007ACC, 1.0F),
		D2D1_COLOR_F    _colorBg = D2D1::ColorF(0x1E1E1E, 1.0F),
		int             _layer = 0,
		bool            _visible = true
	);
	void DelWindow();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	virtual void WndMsgProc(UINT message, WPARAM wparam, LPARAM lparam);
	HINSTANCE GuiRegisterClass(LPCWSTR _lpszClassName);
	RECT     MainRc;
	HWND     hwnd;
	bool     CanMove = true;

	static int                  D2DInit();
	static MSG                  msg;
	static ID2D1Factory*        Factory;
	static IDWriteFactory*      DWriteFactory;
	static GuiWindow**          Window;
	static int                  NumBerOfMainWindow;

};

void GuiMain();
class GuiButton;