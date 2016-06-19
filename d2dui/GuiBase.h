#pragma once
#include <d2d1.h>
#include <Windows.h>
#include <dwrite.h>
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
	int *image;
	bool through;
	bool visible;
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
	/// ��Ϣ��Ӧ����
	/// </summary>
	/// <param name="hwnd">���.</param>
	/// <param name="message">��Ϣ.</param>
	/// <param name="wparam">wparam.</param>
	/// <param name="lparam">lparam.</param>
	/// <returns>����1�������������Ϣ</returns>
	virtual int WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam) = 0;
	int MousePosState = StateMouseOut;
	int MouseLBState = StateMouseLBUp;
	int MouseRBState = StateMouseRBUp;
	int MouseMBState = StateMouseMBUp;
	GuiElement* Element;

};

