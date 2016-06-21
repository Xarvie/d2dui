#pragma once
#include <d2d1.h>
#include <Windows.h>
#include <dwrite.h>
#include <wincodec.h>
#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}
enum MouseState {
	StateMouseOut,
	StateMouseIn,
	StateMouseLBUp,
	StateMouseLBDown,
	StateMouseRBUp,
	StateMouseRBDown,
	StateMouseMBUp,
	StateMouseMBDown,
};

class GuiBase;
class GuiWindow;
struct GuiElement
{
	int id;
	D2D_RECT_F* rc;
	LPCWSTR text;
	ID2D1Bitmap* image;
	bool through;
	bool visible;
	D2D1_COLOR_F Bg;
	D2D1_COLOR_F Bd;
	GuiBase* vfunc;
	GuiElement* last;
	GuiElement* next;
	GuiElement* parent;
	GuiElement* child;
	GuiWindow * window;
};
class GuiBase
{
public:
	/// <summary>
	/// 消息响应函数
	/// </summary>
	/// <param name="hwnd">句柄.</param>
	/// <param name="message">消息.</param>
	/// <param name="wparam">wparam.</param>
	/// <param name="lparam">lparam.</param>
	/// <returns>返回1跳过处理后续消息</returns>
	virtual int WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam) = 0;
	int MousePosState = StateMouseOut;
	int MouseLBState = StateMouseLBUp;
	int MouseRBState = StateMouseRBUp;
	int MouseMBState = StateMouseMBUp;
	GuiElement* Element;
};

