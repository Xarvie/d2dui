#pragma once
#include "GuiBase.h"
class GuiWindow :public GuiBase
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
	void WriteText(const WCHAR* _String, float _x = 10.0f, float _y = 5.0f, float _width = 300.0f, float _height = 20.0f, float _Size = 15.0f, const WCHAR* _FontName = L"微软雅黑");

	void DelWindow();
	static LRESULT CALLBACK WndMsgProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	/// <summary>
	/// 消息响应函数
	/// </summary>
	/// <param name="hwnd">句柄.</param>
	/// <param name="message">消息.</param>
	/// <param name="wparam">wparam.</param>
	/// <param name="lparam">lparam.</param>
	/// <returns>返回1跳过处理后续消息</returns>
	int WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam);
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
	static 	IWICImagingFactory*		WICFactory;
	GuiElement* ElementHead;
	GuiElement* ElementBack;

};

void GuiMain();
class GuiButton;